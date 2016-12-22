# µfck
ufck (µfck) is a minimal bios including a brainf*ck interpreter for the Arduino Uno (ATmega 328p) with the total binary size of less than 1000 bytes.

# Requirements
> avr-gcc, avrdude
> no stdlib needed!

# How to use
Compile & upload with make, replace the 'DEVICE' and 'DUDE_CFG' with your preferences:
	make install DEVICE=COM6 DUDE_CFG=C:/Dev/Arduino/hardware/tools/avr/etc/avrdude.conf
