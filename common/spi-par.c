/*
 * Amiga parallel to SPI adapter
 * Written in the end of April 2020 by Niklas Ekström
 */

#include "common.h"
#include "spi-par.h"

#define	CIAB_PRTRSEL	2
#define	CIAB_PRTRPOUT	1
#define	CIAB_PRTRBUSY	0

#define CS_BIT		CIAB_PRTRSEL
#define CLOCK_BIT	CIAB_PRTRPOUT
#define IDLE_BIT	CIAB_PRTRBUSY

#define CS_MASK		(1 << CS_BIT)
#define CLOCK_MASK	(1 << CLOCK_BIT)
#define IDLE_MASK	(1 << IDLE_BIT)

#define DEVICE_TIMEOUT_MS	50

static volatile uint8_t *cia_a_prb = (volatile uint8_t *)0xbfe101;
static volatile uint8_t *cia_a_ddrb = (volatile uint8_t *)0xbfe301;

static volatile uint8_t *cia_b_pra = (volatile uint8_t *)0xbfd000;
static volatile uint8_t *cia_b_ddra = (volatile uint8_t *)0xbfd200;

static spi_speed_t current_speed = spiSpeed_Slow;

void spi_init(void)
{
	// Should allocate the parallel port in a system friendly way?

	*cia_b_pra = (*cia_b_pra & ~IDLE_MASK) | (CS_MASK | CLOCK_MASK);
	*cia_b_ddra = (*cia_b_ddra & ~IDLE_MASK) | (CS_MASK | CLOCK_MASK);

	*cia_a_prb = 0xff;
	*cia_a_ddrb = 0;
}

void spi_shutdown(void)
{
	*cia_b_ddra &= 0xf8;
	*cia_a_ddrb = 0;
}

static void wait_until_idle(void)
{
	// Timeout so that this will not block forever if the adapter is not present.
	uint32_t timeout = timer_get_tick_count() + TIMER_MILLIS(DEVICE_TIMEOUT_MS);
	
	uint8_t ctrl = *cia_b_pra;
	while (ctrl & IDLE_MASK)
	{
		ctrl ^= CLOCK_MASK;
		*cia_b_pra = ctrl;
		ctrl = *cia_b_pra;
		// Timeout if adapter is not present.
		if((int32_t)(timer_get_tick_count()-timeout) >= 0) {
			break;
		}
	}
}

void spi_set_speed(spi_speed_t speed)
{
	wait_until_idle();

	*cia_a_ddrb = 0xff;
	*cia_a_prb = speed == spiSpeed_Fast ? 0xc1 : 0xc0;
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
static void wait_40_us(void)
{
	int i;
	uint8_t tmp;
	for (i = 0; i < 32; i++)
		tmp = *cia_b_pra;
}

static void spi_write_slow(const uint8_t *buf, unsigned int size)
{
	int i;
	uint8_t ctrl;

	wait_until_idle();

	*cia_a_ddrb = 0xff;

	ctrl = *cia_b_pra;

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

	for (i = 0; i < size; i++)
	{
		*cia_a_prb = *buf++;

		ctrl ^= CLOCK_MASK;
		*cia_b_pra = ctrl;

		wait_40_us();
	}

	*cia_a_ddrb = 0;
}

static void spi_read_slow(uint8_t *buf, unsigned int size)
{
	int i;
	uint8_t ctrl;

	wait_until_idle();

	*cia_a_ddrb = 0xff;

	ctrl = *cia_b_pra;

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

	for (i = 0; i < size; i++)
	{
		wait_40_us();

		ctrl ^= CLOCK_MASK;
		*cia_b_pra = ctrl;

		*buf++ = *cia_a_prb;
	}

	ctrl ^= CLOCK_MASK;
	*cia_b_pra = ctrl;
}

extern void spi_read_fast(register uint8_t *buf __asm("a0"), register unsigned int size __asm("d0"));
extern void spi_write_fast(register const uint8_t *buf __asm("a0"), register unsigned int size __asm("d0"));

void spi_read(uint8_t *buf, unsigned int size)
{
	if (current_speed == spiSpeed_Fast)
		spi_read_fast(buf, size);
	else
		spi_read_slow(buf, size);
}

void spi_write(const uint8_t *buf, unsigned int size)
{
	if (current_speed == spiSpeed_Fast)
		spi_write_fast(buf, size);
	else
		spi_write_slow(buf, size);
}
