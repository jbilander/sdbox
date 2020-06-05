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

#include <stdint.h>

//#include "common.h"
#include "timer.h"

static volatile uint8_t * const todl = (volatile uint8_t*)0xbfe801;
static volatile uint8_t * const todm = (volatile uint8_t*)0xbfe901;
static volatile uint8_t * const todh = (volatile uint8_t*)0xbfea01;

uint32_t timer_get_tick_count(void)
{
	uint8_t l,m,h;

	/* TOD registers latch on reading MSB, unlatch on reading LSB */
	h = *todh;
	m = *todm;
	l = *todl;
	return ((uint32_t)h << 16) | ((uint32_t)m << 8) | (uint32_t)l;
}

void timer_delay(uint32_t ticks)
{
	uint32_t timeout = timer_get_tick_count() + ticks;
	while ((int32_t)(timer_get_tick_count() - timeout) < 0) {

	}
}
