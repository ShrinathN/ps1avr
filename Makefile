CC = avr-gcc


all: main.c psx_controller.h
	avr-gcc -g -Os -mmcu=atmega8a -c main.c
	avr-gcc -g -mmcu=atmega8a -o main.elf main.o
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex

clean:
	rm *.hex *.elf *.o
