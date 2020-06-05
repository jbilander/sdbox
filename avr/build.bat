"C:\\Program Files (x86)\\Arduino\\hardware\\tools\\avr/bin/avr-gcc" -Os -mmcu=atmega328p main.c -o main.elf
"C:\\Program Files (x86)\\Arduino\\hardware\\tools\\avr/bin/avr-objcopy" -O ihex main.elf main.hex
