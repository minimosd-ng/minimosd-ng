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



# automatically build widgets
PROJECT_DIR = ./src
PROJECT_FILES = $(notdir $(shell find $(PROJECT_DIR) -maxdepth 1 -name '*.c'))
SRCS = ${addprefix src/,$(PROJECT_FILES)}

WIDGETS_DIR = ./src/widgets
WIDGETS_FILES = $(notdir $(shell find $(WIDGETS_DIR) -name '*.c'))

HEADERS = $(shell find $(PROJECT_DIR) -name '*.h')


SRCS += ${addprefix src/widgets/,$(WIDGETS_FILES)}



#  C
CFILES=$(filter %.c, $(SRCS))


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

depend: .depend

.depend: $(SRCS) $(HEADERS)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^ > ./.depend;

include .depend

# object from C
%.o: %.c $(HEADERS)
	@echo Compiling $<
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJDEPS) $(HEADERS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJDEPS)

$(PROJECT_NAME).hex: $(PROJECT_NAME).elf
	$(OBJCOPY) -j .text                    \
		-j .data                       \
		-O $(HEXFORMAT) $< $@ ; avr-size -C --mcu $(MCU) $(PROJECT_NAME).elf

$(PROJECT_NAME).eep: $(PROJECT_NAME).elf
	$(OBJCOPY) -j .eeprom --no-change-warnings --change-section-lma .eeprom=0  \
		-O $(HEXFORMAT) $< $@

clean:
	rm -rf *.o $(PRG).elf *.eps *.png *.pdf *.bak *.hex *.bin *.srec
	rm -rf *.lst *.map $(EXTRA_CLEAN_FILES)
	rm -rf ./src/*.o ./src/*.bak ./src/*.lst ./src/*.map
	rm -rf ./src/widgets/*.o ./src/widgets/*.bak ./src/widgets/*.lst ./src/widgets/*.map

flash: $(PROJECT_NAME).hex
	@avrdude -p $(PROGRAMMER_MCU) -c $(AVRDUDE_PROGRAMMER) -P usb -U flash:w:$(PROJECT_NAME).hex

eeprom: $(PROJECT_NAME).eep
	@avrdude -p $(PROGRAMMER_MCU) -c $(AVRDUDE_PROGRAMMER) -P usb -U eeprom:w:$(PROJECT_NAME).eep

