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
               15     Unused
    Output     16     Reset (Buffered reset line from the Amiga)
    Ground  17-25     GND
    
    On the Amiga 1000 some lines are used differently and the connector is db25 female type
    
    Ground  14-22     GND
    Power      23     +5V for Pullup-resistor use only
               24     Unused
    Output     25     Reset (Buffered reset line from the Amiga)
    

The Centronics interface on the Amiga should bring joy to any hacker's heart. It is completely PC compatible. Unfortunately, the Amiga 1000 Centronics port does not conform to the PC standard. First, a female connector was used instead of the usual male db25 connector, and second pin 23 is +5V instead of ground as it is usually on most printer cables. If such a cable is used with the A1000, a short occurs and the Amiga can be damaged.

Internally all the Centronics port lines (except for +5V and Reset) are connected directly to the port lines of the individual CIA's. The exact assignment are as follows:


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
 

The Centronics interface is a parallel interface. The data byte lies on the eight data lines. When the computer has placed a valid byte on the data lines it clears the STROBE line to 0 for 1.4 microseconds, signalling the printer that a valid byte is read for it. The printer must then acknowledge this by pulling the Ack line low for at least one microsecond. The computer then place the next byte on the bus.

The printer uses the BUSY line to indicate that it is occupied and can not accept any more data at the moment. This occurs when the printer buffer is full, for example. The computer the waits until BUSY goes high again before it continues sending data. With the Paper Out line the printer tells the computer that it is out of paper. The select line is also controlled by the printer and indicates whether it is ONLINE (selected, SEL high) or OFFLINE (unselected, SEL low).

The Centronics port is well suited as a universal interface for connecting home-built expansions or an audio digitizer or an EPROM burner, since almost all of the lines can be programmed to be either inputs or outputs.
