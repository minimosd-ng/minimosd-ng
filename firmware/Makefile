## Oven controller Makefile

MCU=atmega328
LIBS=

PROGRAMMER_MCU=m328
AVRDUDE_PROGRAMMER=usbasp

PROJECT_NAME=ovencontroller

OPTLEVEL=s


# compiler
CFLAGS=-I. $(INC) -g -mmcu=$(MCU) -O$(OPTLEVEL) \
	-fpack-struct -fshort-enums             \
	-funsigned-bitfields -funsigned-char    \
	-Wall -Wstrict-prototypes               \
	-Wa,-ahlms=$(firstword                  \
	$(filter %.lst, $(<:.c=.lst)))

PRJSRC= src/main.c


#  C
CFILES=$(filter %.c, $(PRJSRC))


SOURCEDIRS=./src
INC=-I./src

TARGET=$(PROJECT_NAME).elf

CFLAGS += ${addprefix -I,$(SOURCEDIRS)}
CFLAGS += -mmcu=$(MCU)

# linker
LDFLAGS=-Wl,-Map,$(TARGET).map -mmcu=$(MCU) \
	-lm $(LIBS)

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


