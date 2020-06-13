/*
 *  SPI SD device driver for K1208/Amiga 1200
 *
 *  Copyright (C) 2018 Mike Stirling
 *  Modified in 2020 by Niklas Ekström to work with parallel port to SPI adapter
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <exec/devices.h>
#include <exec/errors.h>

#include <dos/dos.h>
#include <dos/dostags.h>

#include <libraries/expansion.h>

#include <devices/trackdisk.h>
#include <devices/scsidisk.h>

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/alib.h>
#include <proto/disk.h>
#include <proto/expansion.h>

#include <stabs.h>

#include "common.h"
#include "sd.h"
#include "spi-par.h"

#define K1208_MFG		12345
#define K1208_PROD		0
#define K1208_IO_BASE	0xe90000

/* These must be globals and the variable names are important */
const char DevName[] = "spisd.device";
const char DevIdString[] = "spisd 0.3 (3 May 2020)";

const UWORD DevVersion = 0;
const UWORD DevRevision = 2;

typedef struct {
	struct Device		*device;
	struct Unit			unit;
} device_ctx_t;

/*! Global device context allocated on device init */
static device_ctx_t *ctx;
struct ExecBase *SysBase;

static uint32_t device_get_geometry(struct IOStdReq *iostd)
{
	struct DriveGeometry *geom = (struct DriveGeometry*)iostd->io_Data;
	const sd_card_info_t *ci = sd_get_card_info();

	if (ci->type != sdCardType_None) {
		geom->dg_SectorSize = 1 << ci->block_size;
		geom->dg_TotalSectors = ci->capacity >> ci->block_size;
		geom->dg_Cylinders = geom->dg_TotalSectors;
		geom->dg_CylSectors = 1;
		geom->dg_Heads = 1;
		geom->dg_TrackSectors = 1;
		geom->dg_BufMemType = MEMF_PUBLIC;
		geom->dg_DeviceType = DG_DIRECT_ACCESS;
		geom->dg_Flags = DGF_REMOVABLE;
		return 0;
	} else {
		return TDERR_DiskChanged;
	}
}

int __UserDevInit(struct Device *device)
{
	/* Open libraries */
	SysBase = *(struct ExecBase**)4l;

	/* Allocate driver context */
	ctx = AllocMem(sizeof(device_ctx_t), MEMF_PUBLIC | MEMF_CLEAR);
	if (ctx == NULL) {
		ERROR("Memory allocation failed\n");
		goto error;
	}
	ctx->device = device;

	/* Initialise hardware */
	spi_init();

	/* Return success */
	return 1;

error:
	/* Clean up after failed open */
	return 0;
}

void __UserDevCleanup(void)
{
	if (ctx) {
		spi_shutdown();

		/* Free context memory */
		FreeMem(ctx, sizeof(device_ctx_t));
		ctx = NULL;
	}

	/* Clean up libs */
}

int __UserDevOpen(struct IORequest *ioreq, uint32_t unit, uint32_t flags)
{
	struct IOStdReq *iostd = (struct IOStdReq*)ioreq;
	int err = IOERR_OPENFAIL;

	if (iostd && unit == 0) {
		if (sd_open() == 0) {
			/* Device is open */
			iostd->io_Unit = &ctx->unit;
			ctx->unit.unit_flags = UNITF_ACTIVE;
			ctx->unit.unit_OpenCnt = 1;
			err = 0;
		}
	}

	iostd->io_Error = err;
	return err;
}

int __UserDevClose(struct IORequest *ioreq)
{
	return 0;
}

ADDTABL_1(__BeginIO,a1);

void __BeginIO(struct IORequest *ioreq)
{
	struct IOStdReq *iostd = (struct IOStdReq*)ioreq;

	if (ctx == NULL || ioreq == NULL) {
		/* Driver not initialised */
		return;
	}

	iostd->io_Error = 0;

	/* Do IO */
	switch (iostd->io_Command) {
	case CMD_RESET:
	case CMD_CLEAR:
	case CMD_UPDATE:
	case TD_MOTOR:
	case TD_REMOVE:
		/* NULL commands */
		iostd->io_Actual = 0;
		break;
	case TD_PROTSTATUS:
		/* Should return a non-zero value if the card is write protected */
		iostd->io_Actual = 0;
		break;
	case TD_CHANGESTATE:
		/* Should return a non-zero value if the card is invalid or not inserted */
		iostd->io_Actual = 0;
		break;
	case TD_CHANGENUM:
		/* This should increment each time a disk is inserted */
		iostd->io_Actual = 1;
		break;
	case TD_GETDRIVETYPE:
		iostd->io_Actual = DG_DIRECT_ACCESS;
		break;
	case TD_GETGEOMETRY:
		iostd->io_Actual = 0;
		iostd->io_Error = device_get_geometry(iostd);
		break;
#if 0
	case HD_SCSI_CMD:
		break;
	case NSCMD_DEVICEQUERY:
		break;
	case NSCMD_TD_READ64:
		break;
	case NSCMD_WRITE64:
		break;
#endif

	case TD_FORMAT:
	case CMD_WRITE:
		/* FIXME: Should be deferred to task but this did not work reliably - investigate */
		if (sd_write(iostd->io_Data, iostd->io_Offset >> SD_SECTOR_SHIFT, iostd->io_Length >> SD_SECTOR_SHIFT) == 0) {
			iostd->io_Actual = iostd->io_Length;
			iostd->io_Error = 0;
		} else {
			iostd->io_Actual = 0;
			iostd->io_Error = TDERR_NotSpecified;
		}
		break;
	case CMD_READ:
		if (sd_read(iostd->io_Data, iostd->io_Offset >> SD_SECTOR_SHIFT, iostd->io_Length >> SD_SECTOR_SHIFT) == 0) {
			iostd->io_Actual = iostd->io_Length;
			iostd->io_Error = 0;
		} else {
			iostd->io_Actual = 0;
			iostd->io_Error = TDERR_NotSpecified;
		}
		break;
	default:
		iostd->io_Error = IOERR_NOCMD;
	}

	if (iostd && !(iostd->io_Flags & IOF_QUICK)) {
		/* Reply to message now unless it was deferred to the task or is IOF_QUICK */
		ReplyMsg(&iostd->io_Message);
	}
}

ADDTABL_1(__AbortIO,a1);

void __AbortIO(struct IORequest *ioreq)
{
	if (ioreq == NULL) {
		return;
	}

	ioreq->io_Error = IOERR_ABORTED;
	return;
}

ADDTABL_END();
