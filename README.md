# sdbox
### A parallel to sd-card project for the Amiga

Work-In-Progress...

The parallel port (centronics interface) pinout for A500/A2000: 

    Connector d-sub db25 male:

    Output      1     Strobe (data ready)
    I/O         2     Data 0
    I/O         3     Data 1
    I/O         4     Data 2
    I/O         5     Data 3
    I/O         6     Data 4
    I/O         7     Data 5
    I/O         8     Data 6
    I/O         9     Data 7
    Input      10     Ack (Acknowledge - data received)
    I/O        11     Busy (Printer Busy)
    I/O        12     POut (Paper Out)
    I/O        13     Sel (Select - printer online)
    Power      14     +5V for Pullup-resistor use only
    -          15     Unused
    Output     16     Reset (Buffered reset line from the Amiga)
    Ground  17-25     GND
    
    On the Amiga 1000 some lines are used differently and the connector is db25 female type
    
    Ground  14-22     GND
    Power      23     +5V for Pullup-resistor use only
    -          24     Unused
    Output     25     Reset (Buffered reset line from the Amiga)
***    

The Centronics interface on the Amiga should bring joy to any hacker's heart. It is completely PC compatible. Unfortunately, the Amiga 1000 Centronics port does not conform to the PC standard. First, a female connector was used instead of the usual male db25 connector, and second pin 23 is +5V instead of ground as it is usually on most printer cables. If such a cable is used with the A1000, a short occurs and the Amiga can be damaged.

Internally all the Centronics port lines (except for +5V and Reset) are connected directly to the port lines of the individual CIA's. The exact assignment are as follows:

***

    Centronics Pin No.     Function        CIA     Pin  Pin designation
                1           Strobe          A       18      PC
                2           Data bit 0      A       10      PB0
                3           Data bit 1      A       11      PB1
                4           Data bit 2      A       12      PB2
                5           Data bit 3      A       13      PB3
                6           Data bit 4      A       14      PB4
                7           Data bit 5      A       15      PB5
                8           Data bit 6      A       16      PB6
                9           Data bit 7      A       17      PB7
               10           Acknowledge     A       24      PB8
               11           Busy            B        2      PA0
                                                and 39      SP
               12           Paper Out       B        3      PA1
                                                and 40      CNT
               13           Select          B        4      PA2
 
***

The Centronics interface is a parallel interface. The data byte lies on the eight data lines. When the computer has placed a valid byte on the data lines it clears the STROBE line to 0 for 1.4 microseconds, signalling the printer that a valid byte is read for it. The printer must then acknowledge this by pulling the Ack line low for at least one microsecond. The computer then place the next byte on the bus.

The printer uses the BUSY line to indicate that it is occupied and can not accept any more data at the moment. This occurs when the printer buffer is full, for example. The computer then waits until BUSY goes high again before it continues sending data. With the Paper Out line the printer tells the computer that it is out of paper. The select line is also controlled by the printer and indicates whether it is ONLINE (selected, SEL high) or OFFLINE (unselected, SEL low).

The Centronics port is well suited as a universal interface for connecting home-built expansions or an audio digitizer or an EPROM burner, since almost all of the lines can be programmed to be either inputs or outputs.

***

8520A CIA-A  | (Complex Interface Adapter, ODD CIA at U7)
------------ | -------------
/IRQ         | /INT2 input from Paula
/RES         | System reset line
D0-D7        | Processor data bus bits 0-7
A0-A3        | Processor address bus bits 8-11
f2           | Processor E clock
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
/IRQ         | /INT6 input from Paula
/RES         | System reset line
D0-D7        | Processor data bus bits 8-15
A0-A3        | Processor address bus bits 8-11
f2           | Processor E clock
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

The addresses `$BFE001` for CIA-A and `$BFD000` for CIA-B are the base of the CIAs specified by Commodore. The selection between the two CIAs is made with address line A12 and A13. CIA-A is selected when `A12=0` and CIA-B is selected when `A13=0`. Since the data bus of CIA-A is connected to processor data lines `D0-D7 (odd addresses)` and CIA-B to `D8-D15 (even addresses)`, the two can be accessed together in one word access if `A12` and `A13` are both 0.

The addressing scheme for the CIA's can be summarized as follows. CIA-A is selected by the following addresses (binary):

    101x xxxx xxx0 rrrr xxxx xxx1
    
and CIA-B by:

    101x xxxx xx0x rrrr xxxx xxx0
    
The four bits designated rrrr select the corresponding registers. The various registers of the CIAs appear multiple times in the range from `$A00000` to `$BFFFFF`. Here are the addresses of the individual registers at their normal positions:

    CIA-A                                     CIA-B                                     Name    Function
    ---------------------------------------------------------------------------------------------------------------------
    $BFE001 : 1011 1111 1110 0000 0000 0001   $BFD000 : 1011 1111 1101 0000 0000 0000   PA      Port register A
    $BFE101 : 1011 1111 1110 0001 0000 0001   $BFD100 : 1011 1111 1101 0001 0000 0000   PB      Port register B
    $BFE201 : 1011 1111 1110 0010 0000 0001   $BFD200 : 1011 1111 1101 0010 0000 0000   DDRA    Data direction register A
    $BFE301 : 1011 1111 1110 0011 0000 0001   $BFD300 : 1011 1111 1101 0011 0000 0000   DDRB    Data direction register B
    $BFE401 : 1011 1111 1110 0100 0000 0001   $BFD400 : 1011 1111 1101 0100 0000 0000   TALO    Timer A low byte
    $BFE501 : 1011 1111 1110 0101 0000 0001   $BFD500 : 1011 1111 1101 0101 0000 0000   TAHI    Timer A high value
    $BFE601 : 1011 1111 1110 0110 0000 0001   $BFD600 : 1011 1111 1101 0110 0000 0000   TBLO    Timer B low byte
    $BFE701 : 1011 1111 1110 0111 0000 0001   $BFD700 : 1011 1111 1101 0111 0000 0000   TBHI    Timer B high byte
    $BFE801 : 1011 1111 1110 1000 0000 0001   $BFD800 : 1011 1111 1101 1000 0000 0000   E. LSB  Event counter bits 0-7
    $BFE901 : 1011 1111 1110 1001 0000 0001   $BFD900 : 1011 1111 1101 1001 0000 0000   E. MID  Event counter bits 8-15
    $BFEA01 : 1011 1111 1110 1010 0000 0001   $BFDA00 : 1011 1111 1101 1010 0000 0000   E. MSB  Event counter bits 16-23
    $BFEB01 : 1011 1111 1110 1011 0000 0001   $BFDB00 : 1011 1111 1101 1011 0000 0000   -       Unused
    $BFEC01 : 1011 1111 1110 1100 0000 0001   $BFDC00 : 1011 1111 1101 1100 0000 0000   SP      Serial port register
    $BFED01 : 1011 1111 1110 1101 0000 0001   $BFDD00 : 1011 1111 1101 1101 0000 0000   IRC     Interrupt control register
    $BFEE01 : 1011 1111 1110 1110 0000 0001   $BFDE00 : 1011 1111 1101 1110 0000 0000   CRA     Control register A
    $BFEF01 : 1011 1111 1110 1111 0000 0001   $BFDF00 : 1011 1111 1101 1111 0000 0000   CRB     Control register B

