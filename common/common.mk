# Must use the gcc 2.95 amiga toolchain for this.  The gcc 6 based one doesn't build a working .device
CC=m68k-amigaos-gcc
AS=m68k-amigaos-as
VASM=vasm

CFLAGS=-m68000 -s -O2 -Wall -fomit-frame-pointer -noixemul -fbaserel 
ASFLAGS=-m68000
LDFLAGS=-Wl,-Map=$(DIR)/$(FILENAME).map

OBJS:=$(addprefix $(DIR)/,$(OBJECTS))

CFLAGS+=$(addprefix -I,$(INCDIRS)) $(EXTRA_CFLAGS)
ASFLAGS+=$(EXTRA_ASFLAGS)
LDFLAGS+=$(EXTRA_LDFLAGS)
VASMFLAGS=-Faout

# Search paths
vpath %.c $(SRCDIRS)
vpath %.s $(SRCDIRS)
vpath %.asm $(SRCDIRS)

$(DIR)/$(FILENAME): $(DIR) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

$(DIR):
	mkdir $(DIR)

$(DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(DIR)/%.o: %.s
	$(AS) $(ASFLAGS) -o $@ $<

$(DIR)/%.o: %.asm
	$(VASM) $(VASMFLAGS) -o $@ $<

clean:
	rm -rf $(DIR)

.PHONY: clean


