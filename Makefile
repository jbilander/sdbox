all:	sd

clean:	sd-clean

sd:
	make -C sd

sd-clean:
	make -C sd clean

.PHONY: all clean sd sd-clean
