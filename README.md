# sdbox
### A parallel to sd-card project for the Amiga

Work-In-Progress...

The parallel port (centronics interface) pinout for A500/A2000, 

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
    
    On the Amiga 1000 some lines are used differently and the connector is of female db25 type
    
    GND     14-22     GND
    +5V        23     +5V for Pullup-resistor use only
               24     Unused
    Output     25     Reset (Buffered reset line from the Amiga)
    
    
