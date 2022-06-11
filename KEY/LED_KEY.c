/*
*========================================================================================================
*
* File                : KEY.c
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


void key_press(void)
{
	PIND &=0xC0;
	switch((PIND&0xC0))
	{	
		case 0x80:  while((PIND&0xC0)==0x40);PORTA =0x01;break;
		case 0x40:  while((PIND&0xC0)==0x80);PORTA =0x02;break;	       
	}

	PINE &=0xC0;
	switch((PINE&0xC0))
	{	
		case 0x80:  while((PINE&0xC0)==0x40);PORTA =0x04;break;
		case 0x40:  while((PINE&0xC0)==0x80);PORTA =0x08;break;	       
	}

}
int main(void)
{

	DDRD =0X00;
	DDRE =0X00;
	PORTD = 0xff;	
	PORTE = 0xff;
	DDRA = 0xff;
	PORTA = 0x55;
	while(1)
    {
     key_press();
	}

}
