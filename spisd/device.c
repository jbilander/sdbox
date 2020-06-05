/*
 *  SPI SD device driver for K1208/Amiga 1200
 *
 *  Copyright (C) 2018 Mike Stirling
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

/*
 * Parts of this file were changed in order for it to compile with VBCC.
 * The procedures device_get_geometry() and begin_io() are intact.
 */

#include <exec/types.h>
#include <exec/execbase.h>
#include <exec/devices.h>
#include <exec/errors.h>
#include <exec/ports.h>
#include <libraries/dos.h>
#include <devices/trackdisk.h>
#include <proto/exec.h>

#include "sd.h"
#include "spi.h"

#ifndef TD_GETGEOMETRY
// Needed to compile with AmigaOS 1.3 headers.
#define TD_GETGEOMETRY	(CMD_NONSTD+13)

struct DriveGeometry
{
	ULONG	dg_SectorSize;
	ULONG	dg_TotalSectors;
	ULONG	dg_Cylinders;
	ULONG	dg_CylSectors;
	ULONG	dg_Heads;
	ULONG	dg_TrackSectors;
	ULONG	dg_BufMemType;
	UBYTE	dg_DeviceType;
	UBYTE	dg_Flags;
	UWORD	dg_Reserved;
};
#endif

char device_name[] = "spisd.device";
char id_string[] = "spisd 0.3 (3 May 2020)";

struct ExecBase *SysBase;
BPTR saved_seg_list;

BOOL is_open;

static struct Library *init_device(__reg("a6") struct ExecBase *sys_base, __reg("a0") BPTR seg_list, __reg("d0") struct Library *dev)
{
	SysBase = *(struct ExecBase **)4;
	saved_seg_list = seg_list;

	dev->lib_Node.ln_Type = NT_DEVICE;
	dev->lib_Node.ln_Name = device_name;
	dev->lib_Flags = LIBF_SUMUSED | LIBF_CHANGED;
	dev->lib_Version = 0;
	dev->lib_Revision = 2;
	dev->lib_IdString = (APTR)id_string;

	is_open = FALSE;

	return dev;
}

static BPTR expunge(__reg("a6") struct Library *dev)
{
	if (dev->lib_OpenCnt != 0)
	{
		dev->lib_Flags |= LIBF_DELEXP;
		return 0;
	}

	spi_shutdown();

	BPTR seg_list = saved_seg_list;
	Remove(&dev->lib_Node);
	FreeMem((char *)dev - dev->lib_NegSize, dev->lib_NegSize + dev->lib_PosSize);
	return seg_list;
}

static void open(__reg("a6") struct Library *dev, __reg("a1") struct IORequest *ior, __reg("d0") ULONG unitnum, __reg("d1") ULONG flags)
{
	ior->io_Error = IOERR_OPENFAIL;
	ior->io_Message.mn_Node.ln_Type = NT_REPLYMSG;

	if (unitnum != 0)
		return;

	if (!is_open)
	{
		spi_initialize();
		if (sd_open() != 0)
			return;
		is_open = TRUE;
	}

	dev->lib_OpenCnt++;
	ior->io_Error = 0;
}

static BPTR close(__reg("a6") struct Library *dev, __reg("a1") struct IORequest *ior)
{
	ior->io_Device = NULL;
	ior->io_Unit = NULL;

	dev->lib_OpenCnt--;

	if (dev->lib_OpenCnt == 0 && (dev->lib_Flags & LIBF_DELEXP))
		return expunge(dev);

	return 0;
}

static uint32_t device_get_geometry(struct IOStdReq *iostd)
{
	struct DriveGeometry *geom = (struct DriveGeometry*)iostd->io_Data;
	const sd_card_info_t *ci = sd_get_card_info();

	if (ci->type != sdCardType_None) {
		geom->dg_SectorSize = 1 << ci->block_size;
		geom->dg_TotalSectors = ci->total_sectors; //ci->capacity >> ci->block_size;
		geom->dg_Cylinders = geom->dg_TotalSectors;
		geom->dg_CylSectors = 1;
		geom->dg_Heads = 1;
		geom->dg_TrackSectors = 1;
		geom->dg_BufMemType = MEMF_PUBLIC;
		geom->dg_DeviceType = 0; //DG_DIRECT_ACCESS;
		geom->dg_Flags = 1; //DGF_REMOVABLE;
		return 0;
	} else {
		return TDERR_DiskChanged;
	}
}

static void begin_io(__reg("a6") struct Library *dev, __reg("a1") struct IORequest *ioreq)
{
	struct IOStdReq *iostd = (struct IOStdReq*)ioreq;

	if (ioreq == NULL) {
		return;
	}

	iostd->io_Error = 0;

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
		iostd->io_Actual = 0; //DG_DIRECT_ACCESS;
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
		if (sd_write((uint8_t *)iostd->io_Data, iostd->io_Offset >> SD_SECTOR_SHIFT, iostd->io_Length >> SD_SECTOR_SHIFT) == 0) {
			iostd->io_Actual = iostd->io_Length;
			iostd->io_Error = 0;
		} else {
			iostd->io_Actual = 0;
			iostd->io_Error = TDERR_NotSpecified;
		}
		break;
	case CMD_READ:
		if (sd_read((uint8_t *)iostd->io_Data, iostd->io_Offset >> SD_SECTOR_SHIFT, iostd->io_Length >> SD_SECTOR_SHIFT) == 0) {
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

static ULONG abort_io(__reg("a6") struct Library *dev, __reg("a1") struct IORequest *ior)
{
	// There is no asynchronous processing of requests so this is always a no-op.
	return IOERR_NOCMD;
}

static ULONG device_vectors[] =
{
	(ULONG)open,
	(ULONG)close,
	(ULONG)expunge,
	0,
	(ULONG)begin_io,
	(ULONG)abort_io,
	-1,
};

ULONG auto_init_tables[] =
{
	sizeof(struct Library),
	(ULONG)device_vectors,
	0,
	(ULONG)init_device,
};
