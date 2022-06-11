/*
*========================================================================================================
*
* File                : Push Buttons.c
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

#define DD_OUT DDRC |=0xff
#define DD_IN DDRC &=~0xff
#define POR_OUT PORTC |=0xff
#define POR_IN PORTC &=~0xff
#define PIN_IN PINC

void key_scanf()
{
	PIN_IN &=0xff;
	switch((PIN_IN&0xff))
	{
		case 0xfe: while((PIN_IN&0xff)==0xfe);PORTA =0x01;break;
		case 0xfd: while((PIN_IN&0xff)==0xfd);PORTA =0x02;break;
		case 0xfb: while((PIN_IN&0xff)==0xfb);PORTA =0x04;break;
		case 0xf7: while((PIN_IN&0xff)==0xf7);PORTA =0x08;break;
		case 0xef: while((PIN_IN&0xff)==0xef);PORTA =0x10;break;
		case 0xdf: while((PIN_IN&0xff)==0xdf);PORTA =0x20;break;
		case 0xbf: while((PIN_IN&0xff)==0xbf);PORTA =0x40;break;
		case 0x7f: while((PIN_IN&0xff)==0x7f);PORTA =0x80;break;
	}
}
int main(void)
{


	DDRA = 0xff;
	DD_IN;
	POR_OUT;
	while(1)
	{
	      key_scanf();
    }

}


