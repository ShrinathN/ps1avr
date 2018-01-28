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
//pin definitions
#define PSX_DIRECTION DDRB
#define PSX_PORT PORTB
#define PSX_PIN PINB
#define PSX_DATA PB4
#define PSX_CMD PB3
#define PSX_ATT PB2
#define PSX_CLK PB1
#define PSX_ACK PB0
#define PSX_CLK_DELAY_US 2
#define PSX_BYTE_DELAY_US 17
typedef unsigned char BYTE;

//initializes the AVR pins
void psx_bus_init()		//function to initialize the PSX bus pins
{
    PSX_DIRECTION =		//enabling CLK, ATT, and CMD pins as out
	    (1 << PSX_CLK) |
	    (1 << PSX_ATT) |
	    (1 << PSX_CMD);
    PSX_PORT =
	    (1 << PSX_CLK) |	//setting CLK, ATT and CMD as high,
	    (1 << PSX_ATT) |
	    (1 << PSX_CMD) |
	    (1 << PSX_DATA) |	//turning on DATA and ACK pins' internal pull down
	    (1 << PSX_ACK);
}

void psx_att_low()		//to set ATT low
{
    PSX_PORT &= ~(1 << PSX_ATT);
}

void psx_att_high()		//to set ATT high
{
    PSX_PORT |= (1 << PSX_ATT);
}

void psx_clk_low()		//to set CLK low
{
    PSX_PORT &= ~(1 << PSX_CLK);
}

void psx_clk_high()		//to set CLK high
{
    PSX_PORT |= (1 << PSX_CLK);
}

void psx_read_state(int * data)	//to read the button data from a controller
{
    BYTE command, counter;

    psx_att_low();				//ATT low to enable controller

    //byte 1
    PSX_PORT |= (1 << PSX_CMD);			//setting CMD pin default high
    command = 0x01;				//first command to init communication
    _delay_us(2);			        //2us delay
    for(counter = 0; counter < 8; counter++)	//loop to read a byte from the controller
    {
	PSX_PORT &= ~(1 << PSX_CMD);		//clearing past CMD pin state
	PSX_PORT |=				//pushing bit to CMD pin
		((command >> counter)
		 &0x1)
		<<PSX_CMD;
	psx_clk_low();				//CLK low period
	_delay_us(PSX_CLK_DELAY_US);		//delay
	psx_clk_high();				//CLK high period
	_delay_us(PSX_CLK_DELAY_US);		//delay
    }
    PSX_PORT |= (1 << PSX_CMD);			//setting CMD pin default high

    _delay_us(PSX_BYTE_DELAY_US);		//delay between bytes

    //byte 2
    command = 0x42;				//command 0x42 to request data
    PSX_PORT |= (1 << PSX_CMD);			//setting CMD pin default high
    for(counter = 0; counter < 8; counter++)	//loop to read a byte from the controller
    {
	PSX_PORT &= ~(1 << PSX_CMD);		//clearing past CMD pin state
	PSX_PORT |=				//pushing bit to CMD pin
		((command >> counter)
		 &0x1)
		<<PSX_CMD;
	psx_clk_low();				//CLK low period
	_delay_us(PSX_CLK_DELAY_US);		//delay
	psx_clk_high();				//CLK high period
	_delay_us(PSX_CLK_DELAY_US);		//delay
    }
    PSX_PORT |= (1 << PSX_CMD);			//setting CMD pin default high

    _delay_us(PSX_BYTE_DELAY_US);		//delay between bytes

    //byte 3
    command = 0;				//no command
    PSX_PORT |= (1 << PSX_CMD);			//setting CMD pin default high
    for(counter = 0; counter < 8; counter++)	//loop to read a byte from the controller
    {
	PSX_PORT &= ~(1 << PSX_CMD);		//clearing past CMD pin state
	PSX_PORT |=				//pushing bit to CMD pin
		((command >> counter)
		 &0x1)
		<<PSX_CMD;
	psx_clk_low();				//CLK low period
	_delay_us(PSX_CLK_DELAY_US); //delay
	psx_clk_high();				//CLK high period
	_delay_us(PSX_CLK_DELAY_US); //delay
    }
    PSX_PORT |= (1 << PSX_CMD);			//setting CMD pin default high

    _delay_us(PSX_BYTE_DELAY_US);		//delay between bytes

    //byte 4
    *data = 0;
    command = 0;
    PSX_PORT |= (1 << PSX_CMD);			//setting CMD pin default high
    for(counter = 0; counter < 8; counter++)	//loop to read a byte from the controller
    {
	PSX_PORT &= ~(1 << PSX_CMD);		//clearing past CMD pin state
	PSX_PORT |=				//pushing bit to CMD pin
		((command >> counter)
		 &0x1)
		<<PSX_CMD;
	psx_clk_low();				//CLK low period
	_delay_us(PSX_CLK_DELAY_US);		//delay
	*data |= ((~PSX_PIN &			//setting the lower byte as 4th byte serial data
		   (1 << PSX_DATA))
		  >>PSX_DATA)
		<< (counter);
	psx_clk_high();				//CLK high period
	_delay_us(PSX_CLK_DELAY_US); //delay
    }
    PSX_PORT |= (1 << PSX_CMD);			//setting CMD pin default high

    _delay_us(PSX_BYTE_DELAY_US);		//delay between bytes

    //byte 5
    command = 0;
    PSX_PORT |= (1 << PSX_CMD);			//setting CMD pin default high
    for(counter = 0; counter < 8; counter++)	//loop to read a byte from the controller
    {
	PSX_PORT &= ~(1 << PSX_CMD);		//clearing past CMD pin state
	PSX_PORT |=				//pushing bit to CMD pin
		((command >> counter)
		 &0x1)
		<<PSX_CMD;
	psx_clk_low();				//CLK low period
	_delay_us(PSX_CLK_DELAY_US);		//delay
	*data |= ((~PSX_PIN &			//setting the lower byte as 4th byte serial data
		   (1 << PSX_DATA))
		  >>PSX_DATA)
		<< (counter + 8);
	psx_clk_high();				//CLK high period
	_delay_us(PSX_CLK_DELAY_US);		//delay
    }
    PSX_PORT |= (1 << PSX_CMD);			//setting CMD pin default high
    psx_att_high();				//ATT high again to disable comm
}
