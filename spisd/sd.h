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

#ifndef SD_H_
#define SD_H_

#include <stdint.h>

#define SD_SECTOR_SIZE		512
#define SD_SECTOR_SHIFT		9

typedef enum {
	sdError_OK = 0,
	sdError_NoCard = -1,
	sdError_Timeout = -2,
	sdError_BadResponse = -3,
	sdError_Unsupported = -4,
} sd_error_t;

typedef enum {
	sdCardType_None = 0,
	sdCardType_SD1_x,
	sdCardType_SD2_0,
	sdCardType_SDHC,
	sdCardType_MMC,
} sd_card_type_t;

typedef enum {
	sdBlockSize_1 = 0,
	sdBlockSize_2,
	sdBlockSize_4,
	sdBlockSize_8,
	sdBlockSize_16,
	sdBlockSize_32,
	sdBlockSize_64,
	sdBlockSize_128,
	sdBlockSize_256,
	sdBlockSize_512,
	sdBlockSize_1024,
	sdBlockSize_2048,
	sdBlockSize_4096,
	sdBlockSize_8192,
	sdBlockSize_16384,
} sd_blocksize_t;

typedef struct {
	uint8_t		csd_structure;				/*!< CSD structure */
	uint8_t		spec_version;				/*!< MMC spec version (not SD) */
	uint8_t		taac;						/*!< data read access time 1 */
	uint8_t		nsac;						/*!< data read access time 2 in CLK cycles */
	uint8_t		max_transfer_rate;			/*!< max data transfer rate in MHz */
	uint16_t	card_command_classes;		/*!< card command classes */
	uint8_t		read_block_len;				/*!< maximum read data block length */
	uint8_t		read_partial_blocks;		/*!< partial blocks for read allowed */
	uint8_t		write_block_misalign;		/*!< write block misalignment */
	uint8_t		read_block_misalign;		/*!< read block misalignment */
	uint8_t		dsr_implemented;			/*!< DSR implemented */
	uint32_t	device_size;				/*!< device size */
	uint8_t		max_read_current_vdd_min;	/*!< max read current at Vdd min (not SDHC) */
	uint8_t		max_read_current_vdd_max;	/*!< max read current at Vdd max (not SDHC) */
	uint8_t		max_write_current_vdd_min;	/*!< max write current at Vdd min (not SDHC) */
	uint8_t		max_write_current_vdd_max;	/*!< max write current at Vdd max (not SDHC) */
	uint8_t		device_size_mult;			/*!< device size multiplier (not SDHC) */
	uint8_t		erase_single_block;			/*!< erase single block enable */
	uint8_t		erase_sector_size;			/*!< erase sector size */
	uint8_t		write_protect_group_size;	/*!< write protect group size */
	uint8_t		write_protect_group;		/*!< write protect group enable */
	uint8_t		write_speed_factor;			/*!< write speed factor */
	uint8_t		write_block_len;			/*!< max write data block length */
	uint8_t		write_partial_blocks;		/*!< partial blocks for write allowed */
	uint8_t		file_format_group;			/*!< file format group */
	uint8_t		copy_flag;					/*!< copy flag */
	uint8_t		perm_write_prot;			/*!< permanent write protection */
	uint8_t		temp_write_prot; 			/*!< temporary write protection */
	uint8_t		file_format;				/*!< file format */
	uint8_t		crc;						/*!< CRC7 */
} sd_card_csd_t;

typedef struct {
	uint8_t		manufacturer_id;			/*!< manufacturer ID byte */
	uint8_t		app_id[2];					/*!< OEM/application ID, ASCII */
	uint8_t		product_name[5];			/*!< product name, ASCII */
	uint8_t		product_rev;				/*!< product revision, BCD */
	uint32_t	product_sn;					/*!< product serial number */
	uint16_t	mfg_date;					/*!< date of manufacture (0xYYM offset from 2000, e.g. 0x014 = Apr 2001) */
	uint8_t		crc;						/*!< CRC7 */
} sd_card_cid_t;

typedef struct {
	sd_card_type_t		type;
	//uint64_t			capacity;
	uint32_t		total_sectors;
	sd_blocksize_t		block_size;
	sd_card_csd_t		csd;
	sd_card_cid_t		cid;
} sd_card_info_t;

int sd_open(void);
void sd_close(void);
int sd_read(uint8_t *buf, uint32_t sector, uint32_t count);
int sd_write(const uint8_t *buf, uint32_t sector, uint32_t count);
const sd_card_info_t* sd_get_card_info(void);

#endif
