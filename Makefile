all:	spisd

clean:	spisd-clean

spisd:
	make -C spisd

spisd-clean:
	make -C spisd clean

.PHONY: all clean spisd spisd-clean
