/*
*========================================================================================================
*
* File                : LED.c
* Hardware Environment:	
* Build Environment   : AVR Studio 4.16 + Winavr 20090313
* Version             : V1.0
* By                  : Wu Ze
*
*                                  (c) Copyright 2005-2009, WaveShare
*                                       http://www.waveShare.net
*                                          All Rights Reserved
*
*========================================================================================================
*/


#include <avr/io.h>
#include <util/delay.h>

	
int main(void)
{
	DDRA=0xFF;	
	PORTA=0x00;
	while(1)
	{
	//	delay_ms(1000);	

       PORTA=0x00;
	   _delay_ms(500);
	   PORTA=0xff;
	   _delay_ms(500);
	}
}


