# DEVICE   = COM6
# DUDE_CFG = C:/Dev/Arduino/hardware/tools/avr/etc/avrdude.conf
CC       = avr-gcc
OC       = avr-objcopy
DUDE     = avrdude
CFLAGS   = -Os -Wall -Wextra -mmcu=atmega328p
OBJ      = main.o std.o serial.o interpreter.o


install: all
	$(OC) -O ihex -R .eeprom µfck.elf µfck.hex
	$(DUDE) -C$(DUDE_CFG) -v -patmega328p -carduino -P$(DEVICE) -b115200 -D -Uflash:w:µfck.hex:i

all: $(OBJ)
	$(CC) $(CFLAGS) -o µfck.elf $(OBJ) -flto -fuse-linker-plugin -Wl,--gc-sections 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -std=c99 -fno-exceptions -ffunction-sections -fdata-sections -MMD -flto -DF_CPU=16000000L