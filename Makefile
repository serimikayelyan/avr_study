#avr-gcc -c -std=gnu99 -DF_CPU=16000000 -mmcu=atmega328p blink.c -o blink.o
#avr-gcc -Os -mmcu=atmega328p blink.o -o blink.elf
##avr-objcopy -O ihex -R .eeprom blink.elf blink.ihex
##avr-gcc -c -g -Os -mmcu=atmega328p -c blink.c
##avr-gcc -g -mmcu=atmega328p -o blink.elf blink.o
#avr-objcopy -j .text -j .data -O ihex blink.elf blink.hex
#avr-size --format=avr --mcu=atmega328p blink.elf
#
##avrdude -c usbtiny -p m328p -U flash:w:code.hex
##
##avrdude -C /path/to/avrdude.conf
##        -p m328p
##        -c PROGRAMMER_NAME
##        -b 19600
##        -P PORT_NAME
##        -U flash:w:TheExecutable.ihex:i
#avrdude -c arduino -p m328p -P /dev/ttyUSB0 -U flash:w:blink.hex

MCU ?= atmega328p
F_CPU ?= 16000000
PROGRAMMER ?= arduino
PORT ?= /dev/ttyUSB0

# Compiler and Tools
CC = avr-gcc
OBJCOPY = avr-objcopy
CFLAGS = -Os -std=gnu99 -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Wall -Wextra $(EXTRA_FLAGS)
LDFLAGS = -mmcu=$(MCU)

# Files (Projects must define SRC and TARGET)
OBJ = $(SRC:.c=.o)
PREPROC = $(SRC:.c=.ii)
ASM = $(SRC:.c=.s)
$(info OBJ=$(OBJ))
ELF = $(TARGET).elf
HEX = $(TARGET).hex

# Default Target: Compile Everything
all: $(HEX)

# Compile C file to Object file
$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $< -o $@

preproc: $(PREPROC)

$(PREPROC): $(SRC)
	$(CC) $(CFLAGS) -E $< -o $@

asm: $(ASM)

$(ASM): $(SRC)
	$(CC) $(CFLAGS) -S $< -o $@

# Link Object file to ELF
$(ELF): $(OBJ)
	$(CC) $(LDFLAGS) $< -o $@

# Convert ELF to HEX
$(HEX): $(ELF)
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

clean:
	rm $(OBJ) $(ELF) $(HEX) $(PREPROC) $(ASM) > /dev/null 2>&1

flash: $(HEX)
	avrdude -c $(PROGRAMMER) -p $(MCU) -P $(PORT) -U flash:w:$(HEX)
