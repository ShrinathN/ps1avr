#define F_CPU 16000000UL
#include </usr/avr/include/avr/io.h>
#include </usr/avr/include/util/delay.h>
#include </usr/avr/include/avr/eeprom.h>
#include </usr/avr/include/avr/interrupt.h>
#include "psx_controller.h"

void uart_init();
void uart_send(BYTE data);
void WDT_off(void);
void main()
{

    DDRC = 1;
    PORTC = 1;
    _delay_ms(500);
    PORTC = 0;
    uart_init();
    WDT_off();
    psx_bus_init();
    int d;
    while(1)
    {
        psx_read_state(&d);
//        _delay_ms(2);
        uart_send(d&0xff);
        uart_send((d>>8)&0xff);
    }
}

void uart_init()
{
    UCSRB |= (1 << TXEN);
    UBRRL = 103;
    UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
}

void uart_send(BYTE data)
{
    while((UCSRA & (1 << UDRE)) == 0){};
    UDR = data;
}

void WDT_off(void)
{
    /* reset WDT */
//    _WDR();
    /* Write logical one to WDCE and WDE */
    WDTCR |= (1<<WDCE) | (1<<WDE);
    /* Turn off WDT */
    WDTCR = 0x00;
}
