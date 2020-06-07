/*
 * Amiga parallel to SPI adapter
 * Written in the end of April 2020 by Niklas Ekström
 */

	.global		_spi_read_fast
	.global		_spi_write_fast

CIAB_PRTRSEL	=	(2)
CIAB_PRTRPOUT	=	(1)
CIAB_PRTRBUSY	=	(0)

CIAA_BASE	=	0xbfe001
CIAB_BASE	=	0xbfd000

CIAPRA		=	0x0000
CIAPRB		=	0x0100
CIADDRA		=	0x0200
CIADDRB		=	0x0300

Disable		=	-120
Enable		=	-126

CS_BIT		=	CIAB_PRTRSEL
CLOCK_BIT	=	CIAB_PRTRPOUT
IDLE_BIT	=	CIAB_PRTRBUSY

/*
 * a0 = unsigned char *buf
 * d0 = unsigned int size
 * assert: 1 <= size < 2^13 (three top bits are zeros)
 */

_spi_write_fast:
	and		#0x1fff,d0
	bne.b		.not_zero1
	rts

.not_zero1:
	movem.l		d2/a5-a6,-(a7)
	move.l		4.w,a6
	jsr		Disable(a6)

	lea.l		CIAA_BASE+CIAPRB,a1	| Data
	lea.l		CIAB_BASE+CIAPRA,a5	| Control pins

.idle_wait1:
	move.b		(a5),d2
	btst		#IDLE_BIT,d2
	beq.b		.is_idle1

	bchg		#CLOCK_BIT,d2
	move.b		d2,(a5)
	bra.b		.idle_wait1

.is_idle1:
	move.b		#0xff,0x200(a1)		| Start driving data pins

	subq		#1,d0			| d0 = size - 1

	cmp		#63,d0
	ble.b		.one_byte_cmd1

	| WRITE2 = 100xxxxx xxxxxxxx
	move		d0,d1
	lsr		#8,d1
	or.b		#0x80,d1
	move.b		d1,(a1)
	bchg		#CLOCK_BIT,d2
	move.b		d2,(a5)

	move.b		d0,(a1)
	bchg		#CLOCK_BIT,d2
	move.b		d2,(a5)
	bra.b		.cmd_sent1

.one_byte_cmd1:
	| WRITE1 = 00xxxxxx
	move.b		d0,(a1)
	bchg		#CLOCK_BIT,d2
	move.b		d2,(a5)

.cmd_sent1:
	addq		#1,d0			| d0 = size

	btst		#0,d0
	beq.b		.even1

	move.b		(a0)+,(a1)
	bchg		#CLOCK_BIT,d2
	move.b		d2,(a5)

.even1:
	lsr		#1,d0
	beq.b		.done1
	subq		#1,d0

	move.b		d2,d1
	bchg		#CLOCK_BIT,d1

.loop1:
	move.b		(a0)+,(a1)
	move.b		d1,(a5)
	move.b		(a0)+,(a1)
	move.b		d2,(a5)
	dbra		d0,.loop1

.done1:
	move.b		#0,0x200(a1)		| Stop driving data pins

	jsr		Enable(a6)
	movem.l		(a7)+,d2/a5-a6
	rts

/*
* a0 = unsigned char *buf
* d0 = unsigned int size
* assert: 1 <= size < 2^13 (three top bits are zeros)
*/

_spi_read_fast:
	and		#0x1fff,d0
	bne.b		.not_zero2
	rts

.not_zero2:
	movem.l		d2/a5-a6,-(a7)
	move.l		4.w,a6
	jsr		Disable(a6)

	lea.l		CIAA_BASE+CIAPRB,a1	| Data
	lea.l		CIAB_BASE+CIAPRA,a5	| Control pins

.idle_wait2:
	move.b		(a5),d2
	btst		#IDLE_BIT,d2
	beq.b		.is_idle2

	bchg		#CLOCK_BIT,d2
	move.b		d2,(a5)
	bra.b		.idle_wait2

.is_idle2:
	move.b		#0xff,0x200(a1)		| Start driving data pins

	subq		#1,d0			| d0 = size - 1

	cmp		#63,d0
	ble.b		.one_byte_cmd2

	| READ2 = 101xxxxx xxxxxxxx
	move		d0,d1
	lsr		#8,d1
	or.b		#0xa0,d1
	move.b		d1,(a1)
	bchg		#CLOCK_BIT,d2
	move.b		d2,(a5)

	move.b		d0,(a1)
	bchg		#CLOCK_BIT,d2
	move.b		d2,(a5)
	bra.b		.cmd_sent2

.one_byte_cmd2:
	| READ1 = 01xxxxxx
	move.b		d0,d1
	or.b		#0x40,d1
	move.b		d1,(a1)
	bchg		#CLOCK_BIT,d2
	move.b		d2,(a5)

.cmd_sent2:
	move.b		#0,0x200(a1)		| Stop driving data pins

	addq		#1,d0			| d0 = size

	btst		#0,d0
	beq.b		.even2

	bchg		#CLOCK_BIT,d2
	move.b		d2,(a5)
	move.b		(a1),(a0)+

.even2:
	lsr		#1,d0
	beq.b		.done2
	subq		#1,d0

	move.b		d2,d1
	bchg		#CLOCK_BIT,d1

.loop2:
	move.b		d1,(a5)
	move.b		(a1),(a0)+
	move.b		d2,(a5)
	move.b		(a1),(a0)+
	dbra		d0,.loop2

.done2:
	bchg		#CLOCK_BIT,d2
	move.b		d2,(a5)

	jsr		Enable(a6)
	movem.l		(a7)+,d2/a5-a6
	rts
