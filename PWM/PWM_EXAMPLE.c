/*
*========================================================================================================
*
* File                : PWM_EXAMPLE.C
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

	DDRE = 0XFF;
    TCCR3B = 0X0A;
	TCCR3A = 0XFF;
	TCNT3 = 0X0000;
	OCR3A = 0X00;
	OCR3B = 0X00;
	OCR3C = 0X00;
	while(1)
	{
		_delay_ms(10);
	//	OCR3A++;
		OCR3B++;
		OCR3C++;
	}
	
}
