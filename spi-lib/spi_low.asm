; Written in the end of April 2020 by Niklas Ekström.

        XDEF        _spi_read_fast
        XDEF        _spi_write_fast
        CODE

CIAB_PRTRSEL	equ	(2)
CIAB_PRTRPOUT	equ	(1)
CIAB_PRTRBUSY	equ	(0)

CIAA_BASE       equ     $bfe001
CIAB_BASE       equ     $bfd000

CIAPRA	        equ	$0000
CIAPRB	        equ	$0100
CIADDRA	        equ	$0200
CIADDRB	        equ	$0300

Disable         equ     -120
Enable          equ     -126

CS_BIT          equ     CIAB_PRTRSEL
CLOCK_BIT       equ     CIAB_PRTRPOUT
IDLE_BIT        equ     CIAB_PRTRBUSY

                ; a0 = unsigned char *buf
                ; d0 = unsigned int size
                ; assert: 1 <= size < 2^13 (three top bits are zeros)

_spi_write_fast:
                and     #$1fff,d0
                bne.b   .not_zero
                rts
.not_zero:
                movem.l d2/a5-a6,-(a7)
                move.l  4.w,a6
                jsr     Disable(a6)

                lea.l   CIAA_BASE+CIAPRB,a1     ; Data
                lea.l   CIAB_BASE+CIAPRA,a5     ; Control pins

.idle_wait:     move.b  (a5),d2
                btst    #IDLE_BIT,d2
                beq.b   .is_idle

                bchg    #CLOCK_BIT,d2
                move.b  d2,(a5)
                bra.b   .idle_wait

.is_idle:       move.b  #$ff,$200(a1)           ; Start driving data pins

                subq    #1,d0                   ; d0 = size - 1

                cmp     #63,d0
                ble.b   .one_byte_cmd

                ; WRITE2 = 100xxxxx xxxxxxxx
                move    d0,d1
                lsr     #8,d1
                or.b    #$80,d1
                move.b  d1,(a1)
                bchg    #CLOCK_BIT,d2
                move.b  d2,(a5)

                move.b  d0,(a1)
                bchg    #CLOCK_BIT,d2
                move.b  d2,(a5)
                bra.b   .cmd_sent

.one_byte_cmd:  ; WRITE1 = 00xxxxxx
                move.b  d0,(a1)
                bchg    #CLOCK_BIT,d2
                move.b  d2,(a5)

.cmd_sent:      addq    #1,d0                   ; d0 = size

                btst    #0,d0
                beq.b   .even

                move.b  (a0)+,(a1)
                bchg    #CLOCK_BIT,d2
                move.b  d2,(a5)

.even:          lsr     #1,d0
                beq.b   .done
                subq    #1,d0

                move.b  d2,d1
                bchg    #CLOCK_BIT,d1

.loop:          move.b  (a0)+,(a1)
                move.b  d1,(a5)
                move.b  (a0)+,(a1)
                move.b  d2,(a5)
                dbra    d0,.loop

.done:          move.b  #0,$200(a1)             ; Stop driving data pins

                jsr     Enable(a6)
                movem.l (a7)+,d2/a5-a6
                rts

                ; a0 = unsigned char *buf
                ; d0 = unsigned int size
                ; assert: 1 <= size < 2^13 (three top bits are zeros)

_spi_read_fast:
                and     #$1fff,d0
                bne.b   .not_zero
                rts
.not_zero:
                movem.l d2/a5-a6,-(a7)
                move.l  4.w,a6
                jsr     Disable(a6)

                lea.l   CIAA_BASE+CIAPRB,a1      ; Data
                lea.l   CIAB_BASE+CIAPRA,a5      ; Control pins

.idle_wait:     move.b  (a5),d2
                btst    #IDLE_BIT,d2
                beq.b   .is_idle

                bchg    #CLOCK_BIT,d2
                move.b  d2,(a5)
                bra.b   .idle_wait

.is_idle:       move.b  #$ff,$200(a1)           ; Start driving data pins

                subq    #1,d0                   ; d0 = size - 1

                cmp     #63,d0
                ble.b   .one_byte_cmd

                ; READ2 = 101xxxxx xxxxxxxx
                move    d0,d1
                lsr     #8,d1
                or.b    #$a0,d1
                move.b  d1,(a1)
                bchg    #CLOCK_BIT,d2
                move.b  d2,(a5)

                move.b  d0,(a1)
                bchg    #CLOCK_BIT,d2
                move.b  d2,(a5)
                bra.b   .cmd_sent

.one_byte_cmd:  ; READ1 = 01xxxxxx
                move.b  d0,d1
                or.b    #$40,d1
                move.b  d1,(a1)
                bchg    #CLOCK_BIT,d2
                move.b  d2,(a5)

.cmd_sent:      move.b  #0,$200(a1)             ; Stop driving data pins

                addq    #1,d0                   ; d0 = size

                btst    #0,d0
                beq.b   .even

                bchg    #CLOCK_BIT,d2
                move.b  d2,(a5)
                move.b  (a1),(a0)+

.even:          lsr     #1,d0
                beq.b   .done
                subq    #1,d0

                move.b  d2,d1
                bchg    #CLOCK_BIT,d1

.loop:          move.b  d1,(a5)
                move.b  (a1),(a0)+
                move.b  d2,(a5)
                move.b  (a1),(a0)+
                dbra    d0,.loop

.done:          bchg    #CLOCK_BIT,d2
                move.b  d2,(a5)

                jsr     Enable(a6)
                movem.l (a7)+,d2/a5-a6
                rts
