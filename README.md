# ps1avr
Original PS1 controller interface for AVR microcontrollers

Default connections from the PS1 controller are as follows (excuse the dumb connector diagram)

___________________________________
\ 0  0  0  |  0  0  0  |  0  0  0 /
 ---------------------------------
  ^
 Pin 1
 
 Pin 1 (DATA) ---> PB4
 Pin 2 (CMD) ----> PB3
 Pin 3 (N/C)
 Pin 4 (GND) ----> GND
 Pin 5 (VCC) ----> VCC (3V to 5V)
 Pin 6 (ATT) ----> PB2
 Pin 7 (CLK) ----> PB1
 Pin 8 (N/C)
 Pin 9 (ACK) ----> PB0
 
 You are free to change the pins (so as long as they remain on the same port on the AVR). Change the pin definitions in the "psx_controller.h" file.
 I use a 16MHz crystal with my AVR, but if you use any other crystal, be sure to define the speed in the "main.c" file, it is defined as F_CPU
