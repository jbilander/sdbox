/*
 * Written in the end of April 2020 by Niklas Ekström.
 */
#include <avr/io.h>

// Amiga pins:
// D0 = A0 = PC0
// D1 = A1 = PC1
// D2 = A2 = PC2
// D3 = A3 = PC3
// D4 = A4 = PC4
// D5 = A5 = PC5
// D6 = D6 = PD6
// D7 = D7 = PD7
// BUSY = D4 = PD4
// POUT = D5 = PD5

// SPI pins in port B.
#define SCK_BIT     5
#define MISO_BIT    4
#define MOSI_BIT    3
#define SS_BIT      2

// These bits are in port D.
#define IDLE_BIT    4
#define CLOCK_BIT   5

int main()
{
    DDRB = (1 << SCK_BIT) | (1 << MOSI_BIT) | (1 << SS_BIT);
    PORTB = (1 << SS_BIT);

    // SPI enabled, master, fosc/64 = 250 kHz
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);
    SPSR |= (1 << SPI2X);

    DDRD = (1 << IDLE_BIT);
    DDRC = 0;

    PORTD = 0;
    PORTC = 0;

    uint8_t v;
    uint8_t w;
    uint8_t next_port_d;
    uint8_t next_port_c;
    uint16_t byte_count;

main_loop:
    if (PIND & (1 << CLOCK_BIT))
    {
        while (PIND & (1 << CLOCK_BIT))
            ;
    }
    else
    {
        while (!(PIND & (1 << CLOCK_BIT)))
            ;
    }

    if (!(PIND & 0x80))
    {
        if (PIND & 0x40)
            goto do_read1;
        else
            goto do_write1;
    }

    v = PIND;
    w = PINC;

    if (!(v & 0x40)) // READ2 or WRITE2
    {
        byte_count = (w & 0x1f) << 8;

        if (v & (1 << CLOCK_BIT))
        {
            while (PIND & (1 << CLOCK_BIT))
                ;
        }
        else
        {
            while (!(PIND & (1 << CLOCK_BIT)))
                ;
        }

        v = PIND;

        byte_count |= (v & 0xc0) | PINC;
        if (w & 0x20)
            goto do_read;
        else
            goto do_write;
    }
    else if ((w & 0x3e) == 0) // SPEED
    {
        if (w & 1) // Fast
            SPCR = (1 << SPE) | (1 << MSTR);
        else // Slow
            SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);
    }

    goto main_loop;

do_read1:
    byte_count = PINC;

do_read:
    SPDR = 0xff;

    v = PIND;

    PORTD = (v & 0xc0) | (1 << IDLE_BIT);
    DDRD = 0xc0 | (1 << IDLE_BIT);

    PORTC = byte_count & 0x3f;
    DDRC = 0x3f;

read_loop:
    while (!(SPSR & (1 << SPIF)))
        ;

    next_port_c = SPDR;
    next_port_d = (next_port_c & 0xc0) | (1 << IDLE_BIT);

    if (v & (1 << CLOCK_BIT))
    {
        while (PIND & (1 << CLOCK_BIT))
            ;
    }
    else
    {
        while (!(PIND & (1 << CLOCK_BIT)))
            ;
    }

    PORTD = next_port_d;
    PORTC = next_port_c;

    v = PIND;

    if (byte_count)
    {
        byte_count--;
        SPDR = 0xff;
        goto read_loop;
    }

    if (v & (1 << CLOCK_BIT))
    {
        while (PIND & (1 << CLOCK_BIT))
            ;
    }
    else
    {
        while (!(PIND & (1 << CLOCK_BIT)))
            ;
    }

    DDRD = (1 << IDLE_BIT);
    DDRC = 0;

    PORTD = 0;
    PORTC = 0;

    goto main_loop;

do_write1:
    byte_count = PINC;
    v = PIND;

do_write:
    PORTD = (1 << IDLE_BIT);

write_loop:
    if (v & (1 << CLOCK_BIT))
    {
        while (PIND & (1 << CLOCK_BIT))
            ;
    }
    else
    {
        while (!(PIND & (1 << CLOCK_BIT)))
            ;
    }

    v = PIND;
    SPDR = (v & 0xc0) | PINC;

    while (!(SPSR & (1 << SPIF)))
        ;

    (void) SPDR;

    if (byte_count)
    {
        byte_count--;
        goto write_loop;
    }

    PORTD = 0;

    goto main_loop;

    return 0;
}
