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

#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

#ifndef TIMER_TICK_FREQ
/*! Tick frequency in Hz - defines timer resolution */
#define TIMER_TICK_FREQ			50
#endif

/*! Helper macro calculates number of ticks in specified ms (rounds up) */
#define TIMER_MILLIS(ms)		(((uint32_t)(ms) * (TIMER_TICK_FREQ) + 999ul) / 1000ul)

/*! Helper macro calculates number of ticks in specified s */
#define TIMER_SECONDS(s)		((uint32_t)(s) * (TIMER_TICK_FREQ))

/*! Helper macro converts ticks back to milliseconds (rounds down) */
#define TIMER_TO_MILLIS(ticks)	((uint32_t)(ticks) * 1000ul / (TIMER_TICK_FREQ))

/*! Helper macro converts ticks back to seconds (rounds down) */
#define TIMER_TO_SECONDS(ticks)	((uint32_t)(ticks) / (TIMER_TICK_FREQ))

/*!
 * Returns current 32-bit tick counter in
 * increments of TIMER_TICK_FREQ
 *
 * \return				Current tick count
 */
uint32_t timer_get_tick_count(void);
void timer_delay(uint32_t ticks);

#endif /* TIMER_H_ */
