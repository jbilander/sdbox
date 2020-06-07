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

#ifndef COMMON_COMMON_H_
#define COMMON_COMMON_H_

/*
 * Define standard types
 */

#if USE_C_STDLIBS
#include <stdint.h>
#include <stdbool.h>
#else
/* Older Amiga toolchain doesn't support C standard libraries, so we define some integer typedefs here */
#include <machine/types.h>

typedef unsigned char uint8_t;
//typedef char int8_t;
typedef unsigned short	uint16_t;
//typedef short int16_t;
typedef unsigned long uint32_t;
//typedef long int32_t;
typedef unsigned long long uint64_t;
//typedef long long int64_t;

#ifndef bool
typedef unsigned int bool;
#endif

#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif
#endif

#ifndef NULL
//! A NULL pointer value
#define NULL				(void*)(0)
#endif

#if DEBUG
#include <stdio.h>
#endif

#if DEBUG > 0
#define ERROR(a,...)		{ printf(__FILE__ "(%d): " a, __LINE__ , ##__VA_ARGS__); }
#else
#define ERROR(...)
#endif

#if DEBUG > 1
#define INFO(a,...)			{ printf(__FILE__ "(%d): " a, __LINE__ , ##__VA_ARGS__); }
#else
#define INFO(...)
#endif

#if DEBUG > 2
#define TRACE(a,...)		{ printf(__FILE__ "(%d): " a, __LINE__ , ##__VA_ARGS__); }
#else
#define TRACE(...)
#endif

#define FUNCTION_TRACE		TRACE("%s\n", __FUNCTION__)


/* Various utility macros */

/*! Returns the minimum of two values */
#define MIN(a,b)			((a) < (b) ? (a) : (b))

/*! Returns the maximum of two values */
#define MAX(a,b)			((a) > (b) ? (a) : (b))

/*! Returns the number of elements in a pre-declared array */
#define ARRAY_SIZE(n)				(sizeof(n) / sizeof(n[0]))

/*! Swap the byte order of a 16-bit word */
#define SWAP16(a)			(((a) >> 8) | ((a) << 8))

/*! Swap the byte order of a 32-bit word */
#define SWAP32(a)			( (((a) >> 24) & 0x000000fful) | \
							  (((a) >> 8) & 0x0000ff00ul) | \
							  (((a) << 8) & 0x00ff0000ul) | \
							  (((a) << 24) & 0xff000000ul) )



#endif /* COMMON_COMMON_H_ */

