The parallel port (centronics interface) pinout for A1000, A500/A2000 and PC. On the Amiga 1000 some lines are used differently and the connector is of db25 female type.

              PIN     A1000                 A500/A2000        PC
    ---------------------------------------------------------------------             
    Output      1     /DRDY (data ready)    /STROBE           /STROBE
    I/O         2     Data 0                Data 0            Data 0
    I/O         3     Data 1                Data 1            Data 1
    I/O         4     Data 2                Data 2            Data 2
    I/O         5     Data 3                Data 3            Data 3
    I/O         6     Data 4                Data 4            Data 4
    I/O         7     Data 5                Data 5            Data 5
    I/O         8     Data 6                Data 6            Data 6
    I/O         9     Data 7                Data 7            Data 7
    Input      10     /ACK (data received)  /ACK              /ACK
    I/O        11     BUSY (data)           BUSY              BUSY
    I/O        12     POUT (clk, Paper Out) POUT              POUT
    I/O        13     SEL (printer online)  SEL               SEL
               14     GND                   +5V Pullup        /AUTOFDXT
               15     GND                   NC (Unused)       /ERROR
               16     GND                   /RESET*           /INIT                  
               17     GND                   GND               /SLCT IN                           
            18-22     GND                   GND               GND
               23     +5V Pullup            GND               GND
               24     NC (Unused)           GND               GND
               25     /RESET*               GND               GND
    
    * Buffered reset line from the Amiga (Output)
***    

The Centronics interface on the Amiga should bring joy to any hacker's heart. It is completely PC compatible. Unfortunately, the Amiga 1000 Centronics port does not conform to the PC standard. First, a female connector was used instead of the usual male db25 connector, and second pin 23 is +5V instead of ground as it is usually on most printer cables. If such a cable is used with the A1000, a short occurs and the Amiga can be damaged.

Internally all the Centronics port lines (except for +5V and Reset) are connected directly to the port lines of the individual CIA's. The exact assignment are as follows:

***

    Centronics Pin No.     Function        CIA     Pin  Pin designation
                1           Strobe          A       18      /PC
                2           Data bit 0      A       10      PB0
                3           Data bit 1      A       11      PB1
                4           Data bit 2      A       12      PB2
                5           Data bit 3      A       13      PB3
                6           Data bit 4      A       14      PB4
                7           Data bit 5      A       15      PB5
                8           Data bit 6      A       16      PB6
                9           Data bit 7      A       17      PB7
               10           Acknowledge     A       24      /FLAG
               11           Busy            B        2      PA0
                                                and 39      SP
               12           Paper Out       B        3      PA1
                                                and 40      CNT
               13           Select          B        4      PA2
 
***
Datasheet: http://archive.6502.org/datasheets/mos_6526_cia_recreated.pdf
* 8520 is functionally equivalent to the 6526 except for the simplified TOD circuitry. 
***

The Centronics interface is a parallel interface. The data byte lies on the eight data lines. When the computer has placed a valid byte on the data lines it clears the STROBE line to 0 for 1.4 microseconds, signalling the printer that a valid byte is ready for it. The printer must then acknowledge this by pulling the Ack line low for at least one microsecond. The computer then place the next byte on the bus.

The printer uses the BUSY line to indicate that it is occupied and can not accept any more data at the moment. This occurs when the printer buffer is full, for example. The computer then waits until BUSY goes high again before it continues sending data. With the Paper Out line the printer tells the computer that it is out of paper. The select line is also controlled by the printer and indicates whether it is ONLINE (selected, SEL high) or OFFLINE (unselected, SEL low).

The Centronics port is well suited as a universal interface for connecting home-built expansions such as an audio digitizer or an EPROM burner, since almost all of the lines can be programmed to be either inputs or outputs.

***

8520A CIA-A  | (Complex Interface Adapter, ODD CIA at U7)
------------ | -------------
/IRQ         | /INT2 input from Paula, Low Priority Interrrupt Request
/RES         | System reset line
D0-D7        | Processor data bus bits 0-7
A0-A3        | Processor address bus bits 8-11
φ2           | Processor E clock
R/W          | Processor R/W
PA7          | Game port 1 pin 6 (fire button)
PA6          | Game port 0 pin 6 (fire button)
PA5          | /RDY    "disk ready" signal from the disk drive
PA4          | /TK0    "disk track 00" signal from disk drive
PA3          | /WPRO   "write protect" signal from disk drive
PA2          | /CHNG   "disk change" signal from disk drive
PA1          | LED     Control over the power LED (0=on, 1=off)
PA0          | OVL     memory overlay bit (do not change!)
SP           | KDAT    Serial keyboard data
CNT          | KCLK    Clock for keyboard data
&#x1F535;  **_PB0-PB7_**| **_Centronics port data lines_**
&#x1F535;  **_PC_**     | **_/DRDY     Centronics handshake signal: data ready (Strobe)_**
&#x1F535;  **_FLAG_**  | **_/ACK       Centronics handshake signal: data acknowledge_**

***

8520A CIA-B  | (Complex Interface Adapter, EVEN CIA at U8)
------------ | -------------
/IRQ         | /INT6 input from Paula, High Priority Interrupt Request
/RES         | System reset line
D0-D7        | Processor data bus bits 8-15
A0-A3        | Processor address bus bits 8-11
φ2           | Processor E clock
R/W          | Processor R/W
PA7          | /DTR serial interface, /DTR signal
PA6          | /RTS serial interface, /RTS signal
PA5          | /CD serial interface, /CD signal
PA4          | /CTS serial interface, /CTS signal
PA3          | /DSR serial interface, /DSR signal
&#x1F535; **_PA2_**| **_SEL Centronics "select" signal_**
&#x1F535; **_PA1_**| **_POUT Centronics "paper out" signal_**
&#x1F535; **_PA0_**| **_BUSY Centronics "busy" signal_**
&#x1F535; **_SP_** | **_BUSY connected directly to PA0_**
&#x1F535; **_CNT_**| **_POUT connected directly to PA1_**
PB7          | /MTR "motor" signal to disk drive
PB6          | /SEL3 "drive select" for drive 3
PB5          | /SEL2 "drive select" for drive 2
PB4          | /SEL1 "drive select" for drive 1
PB3          | /SEL0 "drive select" for drive 0 (internal)
PB2          | /SIDE "side select" signal to disk drive
PB1          | DIR "direction" signal to disk drive
PB0          | /STEP "step" signal to disk drive
FLAG         | /INDEX "index" signal from disk drive
PC           | not used

***

The 8250 has the following features: two programmable 8-bit parallel ports (PA and PB), two 16-bit timers (A and B), a bidirectional serial port (SP) and a 24-bit counter (event counter) with an alarm function upon reaching a programmed value. All of the functions can generate interrupts.

The addresses `$BFE001` for CIA-A and `$BFD000` for CIA-B are the base of the CIAs specified by Commodore. The selection between the two CIAs is made with address line A12 and A13. CIA-A is selected when `A12=0` and CIA-B is selected when `A13=0`. Since the data bus of CIA-A is connected to processor data lines `D0-D7 (odd addresses)` and CIA-B to `D8-D15 (even addresses)`, the two can be accessed together in one word access if `A12` and `A13` are both 0.

The addressing scheme for the CIA's can be summarized as follows. CIA-A is selected by the following addresses (binary):

    101x xxxx xxx0 rrrr xxxx xxx1
    
and CIA-B by:

    101x xxxx xx0x rrrr xxxx xxx0
    
The function of the 8250 are organixed in 16 registers. The four bits designated rrrr select the corresponding registers. The various registers of the CIAs appear multiple times in the range from `$A00000` to `$BFFFFF`.  To the processor they look like ordinary memory locations, since all peripheral components in a 68000 system are memory mapped. The registers of a chip are accessed like memory locations.

The E clock on the 68000 is connected to the ⌀2 input of the 8250. The 16 internal registers are selected with the four address inputs A0-A3. Here is a explanation of the 16 registers (actually only 15 registers, since register 11 is unsused):

       CIA-A                                     CIA-B                                     Name    Function
       ---------------------------------------------------------------------------------------------------------------------
    0  $BFE001 : 1011 1111 1110 0000 0000 0001   $BFD000 : 1011 1111 1101 0000 0000 0000   PA      Port register A
    1  $BFE101 : 1011 1111 1110 0001 0000 0001   $BFD100 : 1011 1111 1101 0001 0000 0000   PB      Port register B
    2  $BFE201 : 1011 1111 1110 0010 0000 0001   $BFD200 : 1011 1111 1101 0010 0000 0000   DDRA    Data direction register A
    3  $BFE301 : 1011 1111 1110 0011 0000 0001   $BFD300 : 1011 1111 1101 0011 0000 0000   DDRB    Data direction register B
    4  $BFE401 : 1011 1111 1110 0100 0000 0001   $BFD400 : 1011 1111 1101 0100 0000 0000   TALO    Timer A low byte
    5  $BFE501 : 1011 1111 1110 0101 0000 0001   $BFD500 : 1011 1111 1101 0101 0000 0000   TAHI    Timer A high value
    6  $BFE601 : 1011 1111 1110 0110 0000 0001   $BFD600 : 1011 1111 1101 0110 0000 0000   TBLO    Timer B low byte
    7  $BFE701 : 1011 1111 1110 0111 0000 0001   $BFD700 : 1011 1111 1101 0111 0000 0000   TBHI    Timer B high byte
    8  $BFE801 : 1011 1111 1110 1000 0000 0001   $BFD800 : 1011 1111 1101 1000 0000 0000   E. LSB  Event counter bits 0-7
    9  $BFE901 : 1011 1111 1110 1001 0000 0001   $BFD900 : 1011 1111 1101 1001 0000 0000   E. MID  Event counter bits 8-15
    10 $BFEA01 : 1011 1111 1110 1010 0000 0001   $BFDA00 : 1011 1111 1101 1010 0000 0000   E. MSB  Event counter bits 16-23
    11 $BFEB01 : 1011 1111 1110 1011 0000 0001   $BFDB00 : 1011 1111 1101 1011 0000 0000   -       Unused
    12 $BFEC01 : 1011 1111 1110 1100 0000 0001   $BFDC00 : 1011 1111 1101 1100 0000 0000   SP      Serial port register
    13 $BFED01 : 1011 1111 1110 1101 0000 0001   $BFDD00 : 1011 1111 1101 1101 0000 0000   ICR     Interrupt control register
    14 $BFEE01 : 1011 1111 1110 1110 0000 0001   $BFDE00 : 1011 1111 1101 1110 0000 0000   CRA     Control register A
    15 $BFEF01 : 1011 1111 1110 1111 0000 0001   $BFDF00 : 1011 1111 1101 1111 0000 0000   CRB     Control register B

***

### The parallel ports

    Reg.  Name  D7    D6    D5    D4    D3    D2    D1    D0
    ---------------------------------------------------------
    0     PA    PA7   PA6   PA5   PA4   PA3   PA2   PA1   PA0
    1     PB    PB7   PB6   PB5   PB4   PB3   PB2   PB1   PB0
    2     DDRA  DPA7  DPA6  DPA5  DPA4  DPA3  DPA2  DPA1  DPA0
    3     DDRB  DPB7  DPB6  DPB5  DPB4  DPB3  DPB2  DPB1  DPB0


The 8250 has two 8-bit parallel ports, PA and PB, each of which is assigned a data register. The chip has 16 port lines, PA0-PA7 and PB0-PB7. Each port line can be used as either an input or an output. The input or output of a port line is called the data direction. The 8250 allows the data direction of each line to be controlled individually. Each port has data direction registers, DDRA and DDRB. 

If a bit in the data direction register is 0, the corresponding line is an input. The state of the port lines can be determined by reading the appropriate bits of the data direction register. If a bit in DDR is set to 1, then the corresponding port line becomes an output.

In general, writing to the data registers always stores the value in it, while reading always returns the state of the port lines. The bits in the data registers is placed in the port lines. Therefore when reading the port which is configured as an output, the contents of the data register is returned, while when writing to an input the value is stored in the data register, but doesn't appear on the port lines until the port is configured as output.

To simplify the data transfer through the parallel ports, the 8250 has two handshake lines, PC and FLAG.

The PC output goes low for one clock period on each access to the data register B (PB, reg 1). The FLAG input responds to such downward transitions. Every time the state of the FLAG line changes from 1 to 0, the FLAG bit is set in the interrupt controller register (ICR, reg. $D). These two lines allow a simple handshaking in which the FLAG and PC lines of two CIA's are cross-connected.

The sender need only write its data to the port register and then wait for a FLAG signal before sending each additional byte. Since FLAG can generate an interrupt, the sender can even perform other tasks while it is waiting. The same applies to the receiver, except that it reads the data from the port instead of writing it.

***

The simplest way to program this interface is directly through the hardware registers. This has the disadvantage that problems can occur with multitasking if another program wants to access this interface, thus it is better to access it through Amiga DOS. The data format is then predefined but you lose the ability to program individual bits as inputs and outputs, however.

***

### Digital I/O

Digital data can be both sent and received. It is also possible to program individual bits of the eight bit data lines as inputs and the rest as outputs. This is only possible with direct programming of the hardware register `$BFE301`.

Here we'll just look at using the entire port as input or output. There is also a device for this: the `parallel.device`. An extension must be made to the normal I/O structure in order to use this device. This extension has the following construction:

    Offset                 Name                  Contents
    --------------------------------------------------------------------------------
    48                     PWBufLen              Length of the output buffer
    52                     ParStatus             Status of the device
    53                     ParFlags              Parallel flags
    54-61                  PTermArray            Termination mask
    
    The status byte contains the following status bits:
    
    Bit                    Name                  Meaning, if set
    --------------------------------------------------------------------------------
    0                      PBUSY                 Printer busy
    1                      PAPEROUT              No more paper
    2                      PSEL                  Printer selected
    3                      RWDIR                 Data direction (0 = read, 1 = write)
    
    The following bits represent the parallel flags:
    
    Bit                    Name                  Meaning, if set
    --------------------------------------------------------------------------------
    1                      EOFMODE               EOF mode enabled
    2                      ACKMODE               Set this bit if you want to use ACK handshaking.
    3                      FASTMODE              High-speed mode. Will send out data as long as the BUSY signal is low.
    4                      SLOWMODE              Slow-speed mode for transfers to very slow printers.      
    5                      SHARED                Access possible for other tasks. Don't use with FASTMODE
    
If you want to read from the parallel port, the question arises as to how the receiver is to recognize the end of the transfer. It is possible to use a given byte sequence to stop the reception. This sequence is stored in the two long words of `TermArray`. This termination sequence is activated if bit 1 of the flag byte is set `(EOFMODE)` and the `SETPARAMS` command `(10)` is then called.

Please Note:

* Strobe functions as input data accepted from Amiga in input mode (similar to ACK* in output mode).
* Ack functions as input-data-ready from parallel device in input mode (similar to Strobe* in output mode).

http://www.theflatnet.de/pub/cbm/amiga/AmigaDevDocs/hard_e.html#e-2-1

***
