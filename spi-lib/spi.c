/*
 * Written in the end of April 2020 by Niklas Ekström.
 */
#include <exec/types.h>
#include <proto/exec.h>

#include "spi.h"

#define	CIAB_PRTRSEL	2
#define	CIAB_PRTRPOUT	1
#define	CIAB_PRTRBUSY	0

#define CS_BIT		CIAB_PRTRSEL
#define CLOCK_BIT	CIAB_PRTRPOUT
#define IDLE_BIT	CIAB_PRTRBUSY

#define CS_MASK		(1 << CS_BIT)
#define CLOCK_MASK	(1 << CLOCK_BIT)
#define IDLE_MASK	(1 << IDLE_BIT)

static volatile UBYTE *cia_a_prb = (volatile UBYTE *)0xbfe101;
static volatile UBYTE *cia_a_ddrb = (volatile UBYTE *)0xbfe301;

static volatile UBYTE *cia_b_pra = (volatile UBYTE *)0xbfd000;
static volatile UBYTE *cia_b_ddra = (volatile UBYTE *)0xbfd200;

static long current_speed = SPI_SPEED_SLOW;

void spi_initialize()
{
	// Should allocate the parallel port in a system friendly way?

	*cia_b_pra = (*cia_b_pra & ~IDLE_MASK) | (CS_MASK | CLOCK_MASK);
	*cia_b_ddra = (*cia_b_ddra & ~IDLE_MASK) | (CS_MASK | CLOCK_MASK);

	*cia_a_prb = 0xff;
	*cia_a_ddrb = 0;
}

void spi_shutdown()
{
	*cia_b_ddra &= 0xf8;
	*cia_a_ddrb = 0;
}

static void wait_until_idle()
{
	// This will block forever if the adapter is not present.
	// TODO: Should eventually timeout and give up.

	UBYTE ctrl = *cia_b_pra;
	while (ctrl & IDLE_MASK)
	{
		ctrl ^= CLOCK_MASK;
		*cia_b_pra = ctrl;
		ctrl = *cia_b_pra;
	}
}

void spi_set_speed(long speed)
{
	wait_until_idle();

	*cia_a_ddrb = 0xff;
	*cia_a_prb = speed == SPI_SPEED_FAST ? 0xc1 : 0xc0;
	*cia_b_pra ^= CLOCK_MASK;
	*cia_a_ddrb = 0;

	current_speed = speed;
}

void spi_select(void)
{
	*cia_b_pra &= ~CS_MASK;
}

void spi_deselect(void)
{
	*cia_b_pra |= CS_MASK;
}

// A slow SPI transfer takes 32 us (8 bits times 4us (250kHz)).
// An E-cycle is 1.4 us.
static void wait_40_us()
{
	UBYTE tmp;
	for (int i = 0; i < 32; i++)
		tmp = *cia_b_pra;
}

static void spi_write_slow(__reg("a0") const UBYTE *buf, __reg("d0") ULONG size)
{
	wait_until_idle();

	*cia_a_ddrb = 0xff;

	UBYTE ctrl = *cia_b_pra;

	if (size <= 64) // WRITE1: 00xxxxxx
	{
		*cia_a_prb = (size - 1) & 0x3f;

		ctrl ^= CLOCK_MASK;
		*cia_b_pra = ctrl;
	}
	else // WRITE2: 100xxxxx xxxxxxxx
	{
		*cia_a_prb = 0x80 | (((size - 1) >> 8) & 0x1f);

		ctrl ^= CLOCK_MASK;
		*cia_b_pra = ctrl;

		*cia_a_prb = (size - 1) & 0xff;

		ctrl ^= CLOCK_MASK;
		*cia_b_pra = ctrl;
	}

	for (int i = 0; i < size; i++)
	{
		*cia_a_prb = *buf++;

		ctrl ^= CLOCK_MASK;
		*cia_b_pra = ctrl;

		wait_40_us();
	}

	*cia_a_ddrb = 0;
}

static void spi_read_slow(__reg("a0") UBYTE *buf, __reg("d0") ULONG size)
{
	wait_until_idle();

	*cia_a_ddrb = 0xff;

	UBYTE ctrl = *cia_b_pra;

	if (size <= 64) // READ1: 01xxxxxx
	{
		*cia_a_prb = 0x40 | ((size - 1) & 0x3f);

		ctrl ^= CLOCK_MASK;
		*cia_b_pra = ctrl;
	}
	else // READ2: 101xxxxx xxxxxxxx
	{
		*cia_a_prb = 0xa0 | (((size - 1) >> 8) & 0x1f);

		ctrl ^= CLOCK_MASK;
		*cia_b_pra = ctrl;

		*cia_a_prb = (size - 1) & 0xff;

		ctrl ^= CLOCK_MASK;
		*cia_b_pra = ctrl;
	}

	*cia_a_ddrb = 0;

	for (int i = 0; i < size; i++)
	{
		wait_40_us();

		ctrl ^= CLOCK_MASK;
		*cia_b_pra = ctrl;

		*buf++ = *cia_a_prb;
	}

	ctrl ^= CLOCK_MASK;
	*cia_b_pra = ctrl;
}

extern void spi_read_fast(__reg("a0") UBYTE *buf, __reg("d0") ULONG size);
extern void spi_write_fast(__reg("a0") const UBYTE *buf, __reg("d0") ULONG size);

void spi_read(__reg("a0") UBYTE *buf, __reg("d0") ULONG size)
{
	if (current_speed == SPI_SPEED_FAST)
		spi_read_fast(buf, size);
	else
		spi_read_slow(buf, size);
}

void spi_write(__reg("a0") const UBYTE *buf, __reg("d0") ULONG size)
{
	if (current_speed == SPI_SPEED_FAST)
		spi_write_fast(buf, size);
	else
		spi_write_slow(buf, size);
}
