/*  Copyright (C) 2018 Shrinath Nimare
    This file is part of ps1avr
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
//TODO
//1.fix random reset problem
#define F_CPU 16000000UL
#include </usr/avr/include/avr/io.h>
#include </usr/avr/include/util/delay.h>
#include </usr/avr/include/avr/eeprom.h>
#include </usr/avr/include/avr/interrupt.h>
#include "psx_controller.h"

void uart_init();           //for initializing the UART
void uart_send(BYTE data);  //sending a single byte through UART
//void WDT_off(void);
void main()
{

    DDRC = 1;           //to blink the LED at PC0
    PORTC = 1;
    _delay_ms(500);
    PORTC = 0;

    uart_init();        //initializing UART

    psx_bus_init();     //initializing PSX I/O pins
    int keysPessed;
    while(1)
    {
	psx_read_state(&keysPessed);    //reads the data
	_delay_ms(2);                   //can be commented for faster operation
	uart_send(keysPessed&0xff);              //sends the lower byte aka 4th byte
	uart_send((keysPessed>>8)&0xff);         //send the upper byte aka 5th byte
    }
}

void uart_init()
{
    UCSRB |= (1 << TXEN);       //enables TX
    UBRRL = 103;                //BAUD rate at 9600
    UCSRC |= (1 << URSEL) |     //selecting UCSRC (NOT UBRRH)
	    (1 << UCSZ0) |      //setting as 8 bits
	    (1 << UCSZ1);
}

void uart_send(BYTE data)
{
    while((UCSRA & (1 << UDRE)) == 0){};    //while UDR is not empty, wait
    UDR = data;                             //send data
}


/*
void WDT_off(void)
{
    WDTCR |= (1<<WDCE) | (1<<WDE);
    WDTCR = 0x00;
}
*/
