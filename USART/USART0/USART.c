/*
*========================================================================================================
*
* File                : USART.c
* Hardware Environment:	
* Build Environment   : AVR Studio 4.16 + Winavr 20090313
* Version             : V1.0
* By                  : Wu Ze
*
*                                  (c) Copyright 2005-2009, WaveShare
*                                       http://www.waveshare.net
*                                          All Rights Reserved
*
*========================================================================================================
*/

#define _DVK501_M128_EX_ 1
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ws_usart_port.h"
#include <util/delay.h>

ISR(USART0_RX_vect)
{      
	uint8_t status,res;
    if(!(WS_UCSR0A & 0x80)) //return 0xFFFF;        //no data to be received 
    status = WS_UCSR0A;
    res = WS_UDR0;
    if (status & 0x1c) //return 0xFFFF;        // If error, return 0xFFFF
	putUsart0(res); 
    //return res;
}  

int main(void)
{unsigned char tmp;
	DDRA=0XFF;
	PORTA=0X55;
	DDRB=0xFF;
	PORTB=0x00;
	sei();			//turn on global interrupt
	usartInit();
	while(1)
	{
/*	putUsart0(0x55);
	
		putUsart0(tmp);
		_delay_ms(10);
		PORTB=(uint8_t)getUsart0();
		tmp++;
		_delay_ms(500);*/
	}
}
