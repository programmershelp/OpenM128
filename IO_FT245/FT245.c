/*
*========================================================================================================
*
* File                : FT245.c
* Hardware Environment:	
* Build Environment   : AVR Studio 4.18 + Winavr 
* Version             : V1.0
* By                  : kallan
*
*                                       http://www.waveShare.net
*                                          All Rights Reserved
*
*========================================================================================================
*/

#include "FT245.h"


void delay50us(sint16 t)
{
    uint8 j;		
    for(;t>0;t--)			
        for(j=0;j<70;j++)	
            ;
}


void delay50ms(sint16 t)
{
	uint16 i; 
	for(;t>0;t--)
		for(i=0;i<52642;i++)
			; 
}


void write_to_ft245()
{
    DATA_DDR=0xFF;

	a++;
	if(a>=10)
	{
		a=0;
	}
	
	
    DATA_PORT=num[a];
    delay50us(10);
    WR_OFF();
    delay50us(10);
    WR_ON();
    delay50us(10);
}


void read_from_ft245()
{
    RD_OFF();
    delay50us(10);
    RD_ON();
    delay50us(10);
}
