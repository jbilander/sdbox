/*
 *  SPI NET ENC28J60 device driver for K1208/Amiga 1200
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

#ifndef SPI_H_
#define SPI_H_

//#include <stdint.h>

typedef enum {
	spiSpeed_Fast = 0,
	spiSpeed_Slow,
} spi_speed_t;

typedef volatile uint32_t *spi_base_t;

void spi_init(spi_base_t port);
void spi_set_speed(spi_speed_t speed);

void spi_select(void);
void spi_deselect(void);
void spi_ext_int_enable(void);
void spi_ext_int_disable(void);
void spi_read(uint8_t *buf, unsigned int size);
void spi_write(const uint8_t *buf, unsigned int size);

#endif
