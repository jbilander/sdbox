# SD card module

An SD card module can be used with the SPI adapter in order to mount the SD card as a volume in AmigaOS.
This driver uses the code written by Mike Sterling: https://github.com/mikestir/k1208-drivers/tree/master/sd. Thanks Mike!

The file `sd.c` does all the heavy lifiting and remains largely unchanged compared to Mike's original. 

Typing `make` will produce the binary spisd.device which should go into the DEVS: directory.

Install the [fat95 file system handler](http://aminet.net/package/disk/misc/fat95) in L: and copy the mountfile `SD0` (available in the `amiga` folder of this repo) to some suitable place e.g. `Devs` where it can be used to mount the SD card on demand by simply double-clicking the icon (read more about how this works in other places, e.g. the fat95 documentation). For automatic mount on boot put the `SD0` file in the `Devs/DOSDrivers` folder.

### Build the spisd.device driver with gcc-2.95.3 https://github.com/adtools/amigaos-cross-toolchain in path:

    jbilander@apollo:~/projects/sdbox$ make
    make -C sd
    make[1]: Entering directory '/home/jbilander/projects/sdbox/sd'
    mkdir build-device
    m68k-amigaos-gcc -m68000 -s -O2 -Wall -fomit-frame-pointer -noixemul -fbaserel -I. -I../common -ramiga-dev -c -o build-device/device.o device.c
    m68k-amigaos-gcc -m68000 -s -O2 -Wall -fomit-frame-pointer -noixemul -fbaserel -I. -I../common -ramiga-dev -c -o build-device/spi-par.o ../common/spi-par.c
    m68k-amigaos-as -m68000  -o build-device/spi-par-low.o ../common/spi-par-low.s
    m68k-amigaos-gcc -m68000 -s -O2 -Wall -fomit-frame-pointer -noixemul -fbaserel -I. -I../common -ramiga-dev -c -o build-device/sd.o sd.c
    m68k-amigaos-gcc -m68000 -s -O2 -Wall -fomit-frame-pointer -noixemul -fbaserel -I. -I../common -ramiga-dev -c -o build-device/timer.o ../common/timer.c
    m68k-amigaos-gcc -m68000 -s -O2 -Wall -fomit-frame-pointer -noixemul -fbaserel -I. -I../common -ramiga-dev -Wl,-Map=build-device/spisd.device.map  -o build-device/spisd.device build-device/device.o build-device/spi-par.o build-device/spi-par-low.o build-device/sd.o build-device/timer.o
    make[1]: Leaving directory '/home/jbilander/projects/sdbox/sd'
    jbilander@apollo:~/projects/sdbox$ cd sd/build-device/
    jbilander@apollo:~/projects/sdbox/sd/build-device$ ls -al
    total 61
    drwxrwxr-x 2 jbilander jbilander    9 Jun  7 23:43 .
    drwxrwxr-x 3 jbilander jbilander    9 Jun  7 23:43 ..
    -rw-rw-r-- 1 jbilander jbilander 1810 Jun  7 23:43 device.o
    -rw-rw-r-- 1 jbilander jbilander 2995 Jun  7 23:43 sd.o
    -rw-rw-r-- 1 jbilander jbilander 1059 Jun  7 23:43 spi-par-low.o
    -rw-rw-r-- 1 jbilander jbilander 1375 Jun  7 23:43 spi-par.o
    -rwxrwxr-x 1 jbilander jbilander 5044 Jun  7 23:43 spisd.device
    -rw-rw-r-- 1 jbilander jbilander 9521 Jun  7 23:43 spisd.device.map
    -rw-rw-r-- 1 jbilander jbilander  294 Jun  7 23:43 timer.o
    jbilander@apollo:~/projects/sdbox/sd/build-device$
