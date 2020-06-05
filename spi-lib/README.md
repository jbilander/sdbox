# Amiga spi-lib

The functionality of the SPI adapter is exposed through the spi-lib source code library.
Just include the three files in the project you are building.

The spi-lib exposes a small number of methods:

- spi_initialize() - initializes the library. Must be called before any other method.
- spi_shutdown() - should be called when shutting down to reset the parallel port to its unused state.
- spi_speed(long speed) - the SPI adapter can run in slow (250 kHz) or fast (8 MHz) mode. A SPI peripheral may need to run in the slow mode during initialization. The speed is set to slow by default when spi-lib is initialized.
- spi_select() / spi_deselect() - activates/deactivates the SPI chip select pin.
- spi_read(char *buf, long size) - reads size bytes (1 <= size <= 8192) from the SPI peripheral and writes them to the buffer pointed to by buf.
- spi_write(char *buf, long size) - writes size bytes (1 <= size <= 8192) to the SPI peripheral that are taken from the buffer pointed to by buf.
