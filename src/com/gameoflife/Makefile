PROGNAME = LifeGame

OFILES = lib/video.o lib/dma.o images.o main.o

HFILES = lib/gba.h lib/input.h lib/color.h lib/video.h lib/dma.h images.h

VBAOPT	 = -f13

TOOLDIR  = /usr/local/cs2110-tools
CFLAGS   = -Wall -Werror -std=c99 -pedantic -O2
CFLAGS  += -mthumb-interwork -mlong-calls -nostartfiles
LDFLAGS = -L $(TOOLDIR)/lib \
		  -L $(TOOLDIR)/lib/gcc/arm-thumb-eabi/4.4.1/thumb \
		  -L $(TOOLDIR)/arm-thumb-eabi/lib \
		  --script $(TOOLDIR)/arm-thumb-eabi/lib/arm-gba.ld
CC      = $(TOOLDIR)/bin/arm-thumb-eabi-gcc
AS      = $(TOOLDIR)/bin/arm-thumb-eabi-as
LD      = $(TOOLDIR)/bin/arm-thumb-eabi-ld
OBJCOPY = $(TOOLDIR)/bin/arm-thumb-eabi-objcopy
GDB     = $(TOOLDIR)/bin/arm-thumb-eabi-gdb
GBADEPS = gbadeps
CFILES  = $(OFILES:.o=.c)


all: $(GBADEPS) $(PROGNAME).gba

$(PROGNAME).gba: $(PROGNAME).elf
	@echo "Linking files together to create $(PROGNAME).gba"
	@$(OBJCOPY) -O binary $(PROGNAME).elf $(PROGNAME).gba

$(PROGNAME).elf: crt0.o $(OFILES)
	@$(LD) $(LDFLAGS) -o $(PROGNAME).elf $^ -lgcc -lc -lgcc

crt0.o: $(TOOLDIR)/arm-thumb-eabi/lib/crt0.s
	@$(AS) -mthumb-interwork $^ -o crt0.o

%.o: %.c
	@echo "[COMPILE] Compiling $<"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Removing all generated files"
	@rm -f *.o *.elf *.gba $(GBADEPS)

vba: $(PROGNAME).gba
	@echo "[RUN] Running Emulator"
	@$(TOOLDIR)/bin/vbam $(VBAOPT) $(PROGNAME).gba > /dev/null

rebuild: clean $(PROGNAME).gba

rebuildvba: clean vba

$(GBADEPS): $(CFILES) $(HFILES)
	@$(CC) $(CFLAGS) -MM $(CFILES) > $(GBADEPS)

-include $(GBADEPS)

