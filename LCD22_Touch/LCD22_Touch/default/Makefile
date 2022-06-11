###############################################################################
# Makefile for the project LCD22
###############################################################################

## General Flags
PROJECT = LCD22
MCU = atmega128
TARGET = LCD22.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99          -DF_CPU=8000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=LCD22.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"E:\program\AVR\OpenM128\EN\LCD22_Touch\LCD22_Touch\User\LCD22" -I"E:\program\AVR\OpenM128\EN\LCD22_Touch\LCD22_Touch\User\Touchpanel" -I"E:\program\AVR\OpenM128\EN\LCD22_Touch\LCD22_Touch\User\Uart" 

## Objects that must be built in order to link
OBJECTS = main.o AsciiLib.o LCD22.o Touchpanel.o USRAT.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) LCD22.hex LCD22.eep LCD22.lss size

## Compile
main.o: ../User/main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

AsciiLib.o: ../User/LCD22/AsciiLib.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

LCD22.o: ../User/LCD22/LCD22.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

Touchpanel.o: ../User/Touchpanel/Touchpanel.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

USRAT.o: ../User/Uart/USRAT.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) LCD22.elf dep/* LCD22.hex LCD22.eep LCD22.lss LCD22.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)

