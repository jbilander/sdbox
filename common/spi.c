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

//#include <stdint.h>

#include "common.h"
#include "spi.h"

#define SPI_CR_CPHA		(1 << 0)
#define SPI_CR_CPOL		(1 << 1)
#define SPI_CR_SLOW		(1 << 2)
#define SPI_CR_CS0		(1 << 4)
#define SPI_CR_CS1		(1 << 5)
#define SPI_CR_INTENA	(1 << 6)

#define SPI_SR_BUSY		(1 << 0)

/*!
 * Define if the hardware is built to insert wait states after starting a
 * SPI cycle (means we don't need to poll BUSY)
 */
#define SPI_WITH_WAIT_STATES		1
/*!
 * Define to use assembly versions of the low-level read/write functions
 * (requires SPI_WITH_WAIT_STATES)
 */
#define SPI_WITH_ASM				1

typedef struct {
	uint8_t		cr;
	uint8_t		dummy1[3];
	uint8_t		sr;
	uint8_t		dummy2[3];
	uint8_t		clkdiv;
	uint8_t		dummy3[3];
	uint8_t		dr;
	uint8_t		dummy4[3];
} spi_t;

static volatile spi_t *spi_base;

#if SPI_WITH_WAIT_STATES && SPI_WITH_ASM
static void spi_read_asm(volatile spi_t *base, uint8_t *buf, unsigned int size)
{
	__asm volatile (
			"	move.l		4(sp), a0\n"				/* IO base */
			"	move.l		8(sp), a1\n"				/* buffer pointer */
			"	move.l		12(sp), d0\n"				/* size */
			"	add			#12, a0\n"					/* offset IO base to data register (faster than doing 12(a0) below) */
			"	moveq		#-1, d1\n"					/* 255 for SPI MOSI */
			"   bra			spi_read_loop_enter\n"
			"spi_read_loop:\n"
			"	move.b		d1,(a0)\n"					/* Initiate read cycle (output 255) */
			"	move.b		(a0),(a1)+\n"				/* Store read value in buffer and post increment */
			"spi_read_loop_enter:\n"
			"   dbra		d0,spi_read_loop\n"
			);
}

static void spi_write_asm(volatile spi_t *base, const uint8_t *buf, unsigned int size)
{
	__asm volatile (
			"	move.l		4(sp), a0\n"
			"	move.l		8(sp), a1\n"
			"	move.l		12(sp), d0\n"
			"	add			#12, a0\n"
			"	bra			spi_write_loop_enter\n"
			"spi_write_loop:\n"
			"	move.b		(a1)+,(a0)\n"
			"spi_write_loop_enter:\n"
			"   dbra		d0,spi_write_loop\n"
			);
}

#endif

static inline void spi_wait(void)
{
	while (spi_base->sr & SPI_SR_BUSY) {}
}

void spi_init(spi_base_t port)
{
	/* Set global base address and clear chip selects */
	spi_base = (volatile spi_t*)port;
	spi_base->cr = 0;
}

void spi_set_speed(spi_speed_t speed)
{
	if (speed == spiSpeed_Fast) {
		spi_base->cr &= ~SPI_CR_SLOW;
	} else {
		spi_base->cr |= SPI_CR_SLOW;
	}
}

void spi_select(void)
{
	spi_base->cr |= SPI_CR_CS0;
}

void spi_deselect(void)
{
	spi_base->cr &= ~SPI_CR_CS0;
}

void spi_ext_int_enable(void)
{
	spi_base->cr |= SPI_CR_INTENA;
}

void spi_ext_int_disable(void)
{
	spi_base->cr &= ~SPI_CR_INTENA;
}

/* NOTE: d0,d1,a0,a1 are scratch and can be trashed. a4 contains data segment pointer.
 * Function args are passed on the stack */

void spi_read(uint8_t *buf, unsigned int size)
{
#if SPI_WITH_WAIT_STATES && SPI_WITH_ASM
	spi_read_asm(spi_base, buf, size);
#else
	while (size--) {
		spi_base->dr = 0xff;
#if !SPI_WITH_WAIT_STATES
		spi_wait();
#endif
		*buf++ = spi_base->dr;
	}
#endif
}

void spi_write(const uint8_t *buf, unsigned int size)
{
#if SPI_WITH_WAIT_STATES && SPI_WITH_ASM
	spi_write_asm(spi_base, buf, size);
#else
	while (size--) {
		spi_base->dr = *buf++;
#if !SPI_WITH_WAIT_STATES
		spi_wait();
#endif
	}
#endif
}
