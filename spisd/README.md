# SD card module

An SD card module can be used with the SPI adapter in order to mount the SD card as a volume in AmigaOS.
This driver uses the code written by Mike Sterling: https://github.com/mikestir/k1208-drivers/tree/master/sd. Thanks Mike!

The file `sd.c` does all the heavy lifiting and remains largely unchanged compared to Mike's original. 

The `build.bat` Windows batch file contains the command line used to compile the driver with VBCC, producing the binary `spisd.device` which should go in the DEVS: directory.

Install the [fat95 file system handler](http://aminet.net/package/disk/misc/fat95) in L: and copy the mountfile `SD0` (available in the `amiga` folder of this repo) to some suitable place e.g. `Devs` folder in wb where it can be used to mount the SD card on demand by simply double-clicking the icon (read more about how this works in other places, e.g. the fat95 documentation). For automatic mount on boot put the `SD0` file in the `Devs/DOSDrivers` folder.

### Build the spisd.device driver with gcc-2.95.3 https://github.com/adtools/amigaos-cross-toolchain in path:

More to come here...

