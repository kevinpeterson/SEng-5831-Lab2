DEVICE = atmega1284p
MCU = atmega1284p
AVRDUDE_DEVICE = m1284p
PORT ?= usb
DEVICE ?= atmega168
MCU ?= atmega168
AVRDUDE_DEVICE ?= m168

CFLAGS=-g -Wall -mcall-prologues -DF_CPU=20000UL -mmcu=$(MCU) $(DEVICE_SPECIFIC_CFLAGS) -O0
CC=avr-gcc
AS=avr-as
OBJ2HEX=avr-objcopy
LDFLAGS=-Wl,-gc-sections -lpololu_$(DEVICE) -Wl,-relax

AVRDUDE=avrdude

TARGET=lab2
OBJECT_FILES=$(TARGET).o motor.o scheduler.o command_parse.o command_line.o serial.o motor_encoder.o logger.o pd_controller.o

all: $(TARGET).hex

clean:
	rm -f *.o *.hex *.obj *.hex

%.hex: $(TARGET).obj
	$(OBJ2HEX) -R .eeprom -O ihex $< $@

%.obj: $(OBJECT_FILES)
	$(CC) $(CFLAGS) $(OBJECT_FILES) $(LDFLAGS) -o $@

%.s: %.c
	$(CC) -mmcu=$(MCU) -fverbose-asm -S $<

lab2.lst: lab1.obj
	avr-objdump -d -M mips -S lab2.obj > lab2.lst

program: $(TARGET).hex
	$(AVRDUDE) -p $(AVRDUDE_DEVICE) -c avrisp2 -P $(PORT) -U flash:w:$(TARGET).hex

.PHONY: release
release:
	tar -fczv SEng5831-Lab2-`date +%m_%d_%y`.tar.gz Makefile *.c *.h *.md 
