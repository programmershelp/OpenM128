/*
*========================================================================================================
*
* File                : main.c
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

const char num[ ]={'1','2','3','4','5','6','7','8','9'};

int main(void)
{
	uint8 m;

	DATA_DDR=0xFF;
	OUT_WR();

	for(m=0;m<100;m++)	
    { 
		write_to_ft245();
	}
		delay50us(50);



	DATA_DDR=0;
    OUT_RD();
			
    while(1)
    {            
    	read_from_ft245();
    }
	
	  
}


