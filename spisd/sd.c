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

#include <stdint.h>
#include <string.h>

//#include "common.h"
#include "spi.h"
#include "sd.h"
#include "timer.h"

#define FUNCTION_TRACE
#define INFO(...)
#define ERROR(...)
#define TRACE(...)

#define SLOW_CLOCK			400000
#define FAST_CLOCK			3000000

#define READY_TIMEOUT_MS	500
#define INIT_TIMEOUT_MS		1000
#define MAX_RESPONSE_POLLS	10

/* MMC/SD command */
#define CMD0	(0)			/* GO_IDLE_STATE */
#define CMD1	(1)			/* SEND_OP_COND (MMC) */
#define	ACMD41	(0x80+41)	/* SEND_OP_COND (SDC) */
#define CMD8	(8)			/* SEND_IF_COND */
#define CMD9	(9)			/* SEND_CSD */
#define CMD10	(10)		/* SEND_CID */
#define CMD12	(12)		/* STOP_TRANSMISSION */
#define ACMD13	(0x80+13)	/* SD_STATUS (SDC) */
#define CMD16	(16)		/* SET_BLOCKLEN */
#define CMD17	(17)		/* READ_SINGLE_BLOCK */
#define CMD18	(18)		/* READ_MULTIPLE_BLOCK */
#define CMD23	(23)		/* SET_BLOCK_COUNT (MMC) */
#define	ACMD23	(0x80+23)	/* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24	(24)		/* WRITE_BLOCK */
#define CMD25	(25)		/* WRITE_MULTIPLE_BLOCK */
#define CMD32	(32)		/* ERASE_ER_BLK_START */
#define CMD33	(33)		/* ERASE_ER_BLK_END */
#define CMD38	(38)		/* ERASE */
#define CMD55	(55)		/* APP_CMD */
#define CMD58	(58)		/* READ_OCR */

static sd_card_info_t sd_card_info;

/*! Utility function for parsing CSD fields */
static int sd_parse_csd(sd_card_info_t *ci, const uint32_t *bits)
{
	sd_card_csd_t *csd = &ci->csd;
	memset(csd, 0, sizeof(sd_card_csd_t));

	TRACE("CSD: %08X %08X %08X %08X\n",
			(unsigned int)bits[0],
			(unsigned int)bits[1],
			(unsigned int)bits[2],
			(unsigned int)bits[3]);

	csd->csd_structure = (bits[0] >> 30) & 0x2;
	csd->taac = (bits[0] >> 16) & 0xff;
	csd->nsac = (bits[0] >> 8) & 0xff;
	csd->max_transfer_rate = (bits[0] >> 0) & 0xff;
	csd->card_command_classes = (bits[1] >> 20) & 0xfff;
	csd->read_block_len = (bits[1] >> 16) & 0xf;
	csd->read_partial_blocks = (bits[1] >> 15) & 0x1;
	csd->write_block_misalign = (bits[1] >> 14) & 0x1;
	csd->read_block_misalign = (bits[1] >> 13) & 0x1;
	csd->dsr_implemented = (bits[1] >> 12) & 0x1;

	if (ci->type == sdCardType_SD1_x || ci->type == sdCardType_SD2_0) {
		csd->device_size = (bits[1] << 2) & 0xffc;
		csd->device_size |= (bits[2] >> 30) & 0x3;

		csd->max_read_current_vdd_min = (bits[2] >> 27) & 0x7;
		csd->max_read_current_vdd_max = (bits[2] >> 24) & 0x7;
		csd->max_write_current_vdd_min = (bits[2] >> 21) & 0x7;
		csd->max_write_current_vdd_max = (bits[2] >> 18) & 0x7;
		csd->device_size_mult = (bits[2] >> 15) & 0x7;

		//ci->capacity = (uint64_t)(csd->device_size + 1) << (csd->device_size_mult + csd->read_block_len + 2);
		ci->total_sectors = (uint32_t)(csd->device_size + 1) << (csd->device_size_mult + 2);
	} else if (ci->type == sdCardType_SDHC) {
		csd->device_size = (bits[1] << 16) & 0x3f;
		csd->device_size |= (bits[2] >> 16) & 0xffff;

		//ci->capacity = (uint64_t)(csd->device_size + 1) << 19;
		ci->total_sectors = (uint32_t)(csd->device_size + 1) << (19 - csd->read_block_len);
	} else {
		ERROR("Card type not supported for CSD decode\n");
		return sdError_Unsupported;
	}

	csd->erase_single_block = (bits[2] >> 14) & 0x1;
	csd->erase_sector_size = (bits[2] >> 7) & 0x7f;
	csd->write_protect_group_size = (bits[2] >> 0) & 0x7f;
	csd->write_protect_group = (bits[3] >> 31) & 0x1;
	csd->write_speed_factor = (bits[3] >> 26) & 0x7;
	csd->write_block_len = (bits[3] >> 22) & 0xf;
	csd->write_partial_blocks = (bits[3] >> 21) & 0x1;
	csd->file_format_group = (bits[3] >> 15) & 0x1;
	csd->copy_flag = (bits[3] >> 14) & 0x1;
	csd->perm_write_prot = (bits[3] >> 13) & 0x1;
	csd->temp_write_prot = (bits[3] >> 12) & 0x1;
	csd->file_format = (bits[3] >> 10) & 0x3;
	csd->crc = (bits[3] >> 1) & 0x7f;

	if (csd->read_block_len != csd->write_block_len) {
		ERROR("Different read/write block sizes not supported\n");
		return sdError_Unsupported;
	}

	ci->block_size = csd->read_block_len;
	INFO("capacity %u MiB block size = %u bytes\n",
			(unsigned int)(ci->capacity / 1024 / 1024),
			1 << ci->block_size);

	/* FIXME: Check CRC */

	return 0;
}

/*! Utility function for parsing CID fields */
static int sd_parse_cid(sd_card_info_t *ci, const uint32_t *bits)
{
	sd_card_cid_t *cid = &ci->cid;
	memset(cid, 0, sizeof(sd_card_cid_t));

	TRACE("CID: %08X %08X %08X %08X\n",
			(unsigned int)bits[0],
			(unsigned int)bits[1],
			(unsigned int)bits[2],
			(unsigned int)bits[3]);

	cid->manufacturer_id = (bits[0] >> 24) & 0xff;
	cid->app_id[0] = (bits[0] >> 16) & 0xff;
	cid->app_id[1] = (bits[0] >> 8) & 0xff;
	cid->product_name[0] = (bits[0] >> 0) & 0xff;
	cid->product_name[1] = (bits[1] >> 24) & 0xff;
	cid->product_name[2] = (bits[1] >> 16) & 0xff;
	cid->product_name[3] = (bits[1] >> 8) & 0xff;
	cid->product_name[4] = (bits[1] >> 0) & 0xff;
	cid->product_rev = (bits[2] >> 24) & 0xff;
	cid->product_sn = (bits[2] << 8) & 0xffffff00;
	cid->product_sn |= (bits[3] >> 24) & 0xff;
	cid->mfg_date = (bits[3] >> 8) & 0xfff;
	cid->crc = (bits[3] >> 1) & 0x7f;

	INFO("SD mfg %02X app '%c%c' product '%.5s' rev %02X sn %08X mfg %02u/%04u\n",
			cid->manufacturer_id,
			cid->app_id[0], cid->app_id[1],
			cid->product_name,
			cid->product_rev,
			(unsigned int)cid->product_sn,
			(unsigned int)(cid->mfg_date & 0xf),
			(unsigned int)((cid->mfg_date >> 8) + 2000));

	/* FIXME: Check CRC */

	return 0;
}


static int sd_wait_ready(void)
{
	uint32_t timeout;
	uint8_t in;

	timeout = timer_get_tick_count() + TIMER_MILLIS(READY_TIMEOUT_MS);
	do {
		spi_read(&in, 1);
	} while (in != 0xff && (int32_t)(timer_get_tick_count() - timeout) < 0);

	return (in == 0xff) ? 0 : sdError_Timeout;
}

static void sd_deselect(void)
{
	spi_deselect();
}

static int sd_select(void)
{
	spi_select();
	if (sd_wait_ready() == 0) {
		return 0;
	}
	spi_deselect();

	ERROR("Timeout waiting for card ready\n");
	return sdError_Timeout;
}

static int sd_read_block(uint8_t *buf, unsigned int size)
{
	uint32_t timeout;
	uint8_t token, crc[2];

	/* Wait for data start token */
	timeout = timer_get_tick_count() + TIMER_MILLIS(READY_TIMEOUT_MS);
	do {
		spi_read(&token, 1);
	} while (token == 0xff && (int32_t)(timer_get_tick_count() - timeout) < 0);
	if (token != 0xfe) {
		ERROR("No data token received\n");
		return sdError_Timeout;
	}

	/* Read data */
	spi_read(buf, size);
	spi_read(crc, 2);

	return 0;
}

static int sd_write_block(const uint8_t *buf, uint8_t token)
{
	uint8_t crc[2] = {0xff, 0xff};
	uint8_t resp;

	if (sd_wait_ready() < 0) {
		ERROR("Card not ready\n");
		return sdError_Timeout;
	}

	/* Send token */
	spi_write(&token, 1);
	if (token != 0xfd) {
		/* Send data, except for STOP_TRAN */
		spi_write(buf, SD_SECTOR_SIZE);
		spi_write(crc, 2); /* dummy */

		/* Receive data response */
		spi_read(&resp, 1);
		if ((resp & 0x1f) != 0x05) {
			ERROR("Bad response\n");
			return sdError_BadResponse;
		}
	}

	return 0;
}

static uint8_t sd_send_cmd(uint8_t cmd, uint32_t arg)
{
	uint8_t res;
	uint8_t buf[6];
	int n;

	if (cmd & 0x80) {
		/* Send CMD55 prior to ACMD */
		cmd &= 0x7f;
		res = sd_send_cmd(CMD55, 0);
		if (res > 1) {
			return res;
		}
	}

	/* Select the card and wait for ready except for abort */
	if (cmd != CMD12) {
		sd_deselect();
		if (sd_select() < 0) {
			return 0xff;
		}
	}

	/* Build command */
	buf[0] = 0x40 | cmd;
	buf[1] = (uint8_t)(arg >> 24);
	buf[2] = (uint8_t)(arg >> 16);
	buf[3] = (uint8_t)(arg >> 8);
	buf[4] = (uint8_t)(arg >> 0);
	if (cmd == CMD0) {
		buf[5] = 0x95; /* CRC for CMD0 */
	} else if (cmd == CMD8) {
		buf[5] = 0x87; /* CRC for CMD8 */
	} else {
		buf[5] = 0x01; /* Dummy CRC and stop */
	}
	spi_write(buf, sizeof(buf));

	/* Receive command response */
	if (cmd == CMD12) {
		/* Skip first byte */
		spi_read(&res, 1);
	}

	for (n = 0; n < MAX_RESPONSE_POLLS; n++) {
		spi_read(&res, 1);
		if (!(res & 0x80)) {
			break;
		}
	}

	return res;
}

static uint32_t sd_get_r7_resp(void)
{
	uint8_t buf[4];

	spi_read(buf, 4);
	return ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) | ((uint32_t)buf[2] << 8) | ((uint32_t)buf[3] << 0);
}

int sd_open(void)
{
	sd_card_info_t *ci = &sd_card_info;
	uint32_t timeout;
	uint8_t cmd;
	uint32_t resp[4];
	int err;

	FUNCTION_TRACE;

	spi_set_speed(SPI_SPEED_SLOW);
	ci->type = sdCardType_None;
	//ci->capacity = 0;
	ci->total_sectors = 0;
	ci->block_size = sdBlockSize_512;

	/* Send dummy clocks with CS high (doing this sends 96 clocks) */
	sd_deselect();
	sd_get_r7_resp();
	sd_get_r7_resp();
	sd_get_r7_resp();

	if (sd_send_cmd(CMD0,0) == 1) {
		if (sd_send_cmd(CMD8, 0x1aa) == 1) {
			uint32_t ocr = sd_get_r7_resp();

			if (ocr == 0x000001aa) {
				TRACE("SDv2 - R7 resp = 0x%08X\n", (unsigned int) ocr);
				ci->type = sdCardType_SD2_0;

				/* Wait for card ready */
				timeout = timer_get_tick_count() + TIMER_MILLIS(INIT_TIMEOUT_MS);
				while (sd_send_cmd(ACMD41, (1ul << 30)) > 0) {
					if ((int32_t)(timer_get_tick_count() - timeout) >= 0) {
						/* Init timed out - invalidate card */
						ERROR("Init timed out\n");
						ci->type = sdCardType_None;
					}
				}

				if (ci->type) {
					/* Read OCR */
					if (sd_send_cmd(CMD58, 0) == 0) {
						ocr = sd_get_r7_resp();
						if (ocr & (1ul << 30)) {
							/* Card is high capacity */
							TRACE("SDHC\n");
							ci->type = sdCardType_SDHC;
						}
					} else {
						ERROR("Failed to read OCR\n");
						ci->type = sdCardType_None;
					}
				}
			}
		} else {
			/* Not SDv2 */
			if (sd_send_cmd(ACMD41, 0) <= 1) {
				TRACE("SDv1\n");
				ci->type = sdCardType_SD1_x;
				cmd = ACMD41;
			} else {
				TRACE("MMCv3\n");
				ci->type = sdCardType_MMC;
				cmd = CMD1;
			}

			/* Wait for card ready */
			timeout = timer_get_tick_count() + TIMER_MILLIS(INIT_TIMEOUT_MS);
			while (sd_send_cmd(cmd, 0) > 0) {
				if ((int32_t)(timer_get_tick_count() - timeout) >= 0) {
					/* Init timed out - invalidate card */
					ERROR("Init timed out\n");
					ci->type = sdCardType_None;
				}
			}

			if (ci->type) {
				/* Set block length */
				if (sd_send_cmd(CMD16, SD_SECTOR_SIZE) > 0) {
					ERROR("Failed to set block length\n");
					ci->type = sdCardType_None;
				}
			}
		}
	}

	if (ci->type) {
		INFO("SD card ready (type %u)\n", ci->type);

		/* Read and decode card info */
		if (sd_send_cmd(CMD10, 0) == 0) {
			err = sd_read_block((uint8_t*)&resp, sizeof(resp));
			if (err < 0) {
				ERROR("Read CID failed\n");
			}
		} else {
			err = sdError_BadResponse;
		}
		if (err == 0) {
			err = sd_parse_cid(ci, resp);
		}
		if (err == 0) {
			if (sd_send_cmd(CMD9, 0) == 0) {
				err = sd_read_block((uint8_t*)&resp, sizeof(resp));
				if (err < 0) {
					ERROR("Read CSD failed\n");
				}
			} else {
				err = sdError_BadResponse;
			}
		}
		if (err == 0) {
			err = sd_parse_csd(ci, resp);
		}

		/* Switch to fast clock */
		spi_set_speed(SPI_SPEED_FAST);
	} else {
		/* Card not present */
		err = sdError_NoCard;
	}

	sd_deselect();

	return err;
}

int sd_read(uint8_t *buf, uint32_t sector, uint32_t count)
{
	sd_card_info_t *ci = &sd_card_info;
	int err = 0;

	if (ci->type == sdCardType_None) {
		ERROR("No card\n");
		return sdError_NoCard;
	}
	if (ci->type != sdCardType_SDHC) {
		/* Convert sector to byte addressing (x512) */
		sector <<= 9;
	}

	if (count == 1) {
		/* Read single sector */
		if (sd_send_cmd(CMD17, sector) == 0) {
			err = sd_read_block(buf, SD_SECTOR_SIZE);
		} else {
			err = sdError_BadResponse;
		}
	} else {
		/* Read multiple sectors */
		if (sd_send_cmd(CMD18, sector) == 0) {
			do {
				err = sd_read_block(buf, SD_SECTOR_SIZE);
				if (err < 0) {
					break;
				}
				buf += SD_SECTOR_SIZE;
			} while (--count);

			/* Send CMD12 stop transmission */
			if (err == 0) {
				err = sd_send_cmd(CMD12, 0);
			}
		} else {
			err = sdError_BadResponse;
		}
	}

	sd_deselect();

	return err;
}

int sd_write(const uint8_t *buf, uint32_t sector, uint32_t count)
{
	sd_card_info_t *ci = &sd_card_info;
	int err = 0;

	if (ci->type == sdCardType_None) {
		ERROR("No card\n");
		return sdError_NoCard;
	}
	if (ci->type != sdCardType_SDHC) {
		/* Convert sector to byte addressing (x512) */
		sector <<= 9;
	}

	if (count == 1) {
		/* Write single sector */
		if (sd_send_cmd(CMD24, sector) == 0) {
			err = sd_write_block(buf, 0xfe);
		} else {
			err = sdError_BadResponse;
		}
	} else {
		if (ci->type == sdCardType_SD1_x || ci->type == sdCardType_SD2_0 || ci->type == sdCardType_SDHC) {
			/* Pre-defined sector count */
			sd_send_cmd(ACMD23, count);
		}
		/* Write multiple sectors */
		if (sd_send_cmd(CMD25, sector) == 0) {
			do {
				err = sd_write_block(buf, 0xfc);
				if (err < 0) {
					break;
				}
				buf += SD_SECTOR_SIZE;
			} while (--count);

			/* Send STOP_TRAN */
			if (err == 0) {
				err = sd_write_block(0, 0xfd);
			}
		} else {
			err = sdError_BadResponse;
		}
	}

	sd_deselect();

	return err;
}

const sd_card_info_t* sd_get_card_info(void)
{
	return &sd_card_info;
}
