PORT = /dev/ttyUSB0
BAUD = 115200
MCU = atmega328p
PROGRAMMER = arduino

all: build flash

main.elf: main.c
	avr-gcc -Os -mmcu=$(MCU) main.c -o main.elf

main.hex: main.elf
	avr-objcopy -O ihex main.elf main.hex

build: main.hex

flash: main.hex
	avrdude -p$(MCU) -c$(PROGRAMMER) -P$(PORT) -b$(BAUD) -D -Uflash:w:main.hex:i

clean:
	rm main.elf
	rm main.hex
