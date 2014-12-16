## Oven controller Makefile

MCU=atmega328p
LIBS=

PROGRAMMER_MCU=m328p
AVRDUDE_PROGRAMMER=usbasp

PROJECT_NAME=minimosd-ng

OPTLEVEL=s

INC=-I./src -I./lib

# compiler
CFLAGS=-I. $(INC) -g -mmcu=$(MCU) -O$(OPTLEVEL) \
	-fpack-struct -fshort-enums             \
	-funsigned-bitfields -funsigned-char    \
	-Wall -Wstrict-prototypes               \
	-Wa,-ahlms=$(firstword                  \
	$(filter %.lst, $(<:.c=.lst)))

PRJSRC= src/main.c src/uart.c src/max7456.c \
        src/mavlink.c src/widgets.c

# automatically build widgets
WIDGETS_DIR = ./src/widgets
WIDGETS_FILES = $(notdir $(shell find $(WIDGETS_DIR) -name '*.c'))

PRJSRC += ${addprefix src/widgets/,$(WIDGETS_FILES)}



#  C
CFILES=$(filter %.c, $(PRJSRC))


SOURCEDIRS=./src
SOURCEDIRS+= $(WIDGETS_DIR)

TARGET=$(PROJECT_NAME).elf

CFLAGS += ${addprefix -I,$(SOURCEDIRS)}
CFLAGS += -mmcu=$(MCU)

# linker
LDFLAGS=-Wl,-Map,$(TARGET).map -mmcu=$(MCU) \
	-Wl,-u,vfprintf -lprintf_flt -lm $(LIBS)

OBJDEPS=$(CFILES:.c=.o)

CC=avr-gcc
OBJCOPY=avr-objcopy

HEXFORMAT=ihex

.SUFFIXES : .c .cc .cpp .C .o .out .s .S \
	.hex .ee.hex .h .hh .hpp

.PHONY : flash


all: $(PROJECT_NAME).hex $(TARGET) 
	


$(TARGET): $(OBJDEPS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJDEPS)

$(PROJECT_NAME).hex: $(PROJECT_NAME).elf
	$(OBJCOPY) -j .text                    \
		-j .data                       \
		-O $(HEXFORMAT) $< $@ ; avr-size -C --mcu $(MCU) $(PROJECT_NAME).elf

clean:
	rm -rf *.o $(PRG).elf *.eps *.png *.pdf *.bak *.hex *.bin *.srec
	rm -rf *.lst *.map $(EXTRA_CLEAN_FILES)
	rm -rf ./src/*.o ./src/*.bak ./src/*.lst ./src/*.map

# object from C
.c.o: 
	$(CC) $(CFLAGS) -c $< -o $@

flash:
	@avrdude -p $(PROGRAMMER_MCU) -c $(AVRDUDE_PROGRAMMER) -P usb -U flash:w:$(PROJECT_NAME).hex


