# Must use the gcc 2.95 amiga toolchain for this. 
CC=m68k-amigaos-gcc
AS=m68k-amigaos-as

CFLAGS=-m68000 -s -O2 -Wall -fomit-frame-pointer -noixemul -fbaserel 
ASFLAGS=-m68000
LDFLAGS=-Wl,-Map=$(DIR)/$(FILENAME).map

OBJS:=$(addprefix $(DIR)/,$(OBJECTS))

CFLAGS+=$(addprefix -I,$(INCDIRS)) $(EXTRA_CFLAGS)
ASFLAGS+=$(EXTRA_ASFLAGS)
LDFLAGS+=$(EXTRA_LDFLAGS)

vpath %.c $(SRCDIRS)
vpath %.s $(SRCDIRS)

$(DIR)/$(FILENAME): $(DIR) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS) 

$(DIR):
	mkdir $(DIR)

$(DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(DIR)/%.o: %.s
	$(AS) $(ASFLAGS) -o $@ $<

clean:
	rm -rf $(DIR)

.PHONY: clean
