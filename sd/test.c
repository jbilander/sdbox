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

#include <stdio.h>

#include "common.h"
#include "sd.h"
#include "spi.h"

extern volatile uint32_t* device_get_spi_base(void);

static void hexdump(const uint8_t *buf, unsigned int size)
{
	unsigned int n;

	for (n = 0; n < size; n++) {
		printf("%02X ", (unsigned int)buf[n]);
		if ((n & 15) == 15) {
			printf("\n");
		}
	}
	printf("\n");
}

int main(void)
{
	volatile uint32_t *spi_base;
	static uint8_t buf[SD_SECTOR_SIZE * 2];

	spi_base = device_get_spi_base();
	if (spi_base == NULL) {
		printf("No board found\n");
		return 1;
	}
	printf("Board found at 0x%08x\n", spi_base);
	spi_init(spi_base);
	sd_open();

	printf("Read single sector\n");
	printf("%d\n", sd_read(buf, 0, 1));
	hexdump(buf, SD_SECTOR_SIZE);

	printf("Read multiple sectors\n");
	printf("%d\n", sd_read(buf, 0, 2));
	hexdump(buf, SD_SECTOR_SIZE * 2);

	printf("Read multiple sectors\n");
	printf("%d\n", sd_read(buf, 4, 2));
	hexdump(buf, SD_SECTOR_SIZE * 2);

	printf("Read single sector\n");
	printf("%d\n", sd_read(buf, 0, 1));
	hexdump(buf, SD_SECTOR_SIZE);

	return 0;
}
