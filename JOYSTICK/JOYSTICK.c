/*
*========================================================================================================
*
* File                : JOYSTICK.c
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


int main(void)
{


	DDRA=0XFF;
	PORTA=0X55;
	SFIOR=0x00;
	DDRG = 0X00;
	PORTG = 0Xff;
	while(1)
    {
		PING &=0X1F;
		switch((PING&0x1F))
		{	
			case 0x1E:  while((PING&0x1F)==0x1E);PORTA =0x01;break;
			case 0x1D:  while((PING&0x1F)==0x1D);PORTA =0x02;break;	       
			case 0x1B:  while((PING&0x1F)==0x1B);PORTA =0x04;break;	 
			case 0x17:  while((PING&0x1F)==0x17);PORTA =0x08;break;	 
			case 0x0F:  while((PING&0x1F)==0x0F);PORTA =0x10;break;	
		}

	}
}
