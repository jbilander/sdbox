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
    +5V        14     +5V for Pullup-resistor use only
               15     Unused
    Output     16     Reset (Buffered reset line from the Amiga)
    GND     17-25     GND
    
    On the Amiga 1000 some lines are used differently and the connector is db25 female type
    
    GND     14-22     GND
    +5V        23     +5V for Pullup-resistor use only
               24     Unused
    Output     25     Reset (Buffered reset line from the Amiga)
    
The Centronics interface on the Amiga should bring joy to any hacker's heart. It is completely PC compatible. Unfortunately, the Amiga 1000 Centronics port does not conform to the PC standard. First, a female connector was used instead of the usual male db25 connector, and second pin 23 is +5V instead of ground as it is usually on most printer cables. If such a cable is used with the A1000, a short occurs and the Amiga can be damaged.

Internally all the Centronics port lines (except for +5V and Reset) are connected directly to the port lines of the individual CIA's. The exact assignment are as follows:

