#include<avr/io.h>
#include<avr/delay.h>
#include"USART.H"
#include"AT45DBXX.h"

int main(void)
{
	uint8_t num;
	uint8_t tmp[255];
	uint8_t f_value=0;

	usartInit();
	printf("*--------------------------------------------------- *\r\n");
	printf("*-------------------AT45DBXX experiment-------------- *\r\n");
	printf("*--------------------------------------------------- *\r\n");


	DDRA=0xFF;
	spiInitAt45db();
	printf("FALSH AT45DBXX Write Test \r\n");

	for(num=0;num<255;num++) /*Written to the flash*/
	{
		write_buffer((uint16_t)num,num);
		_delay_ms(1);
	}

	printf("FALSH AT45DBXX Read Test \r\n");

	for(num=0;num<255;num++) /*Read the flash*/
	{
		tmp[num]=read_buffer(num);
		_delay_ms(1);
	}

	for(num=0;num<255;num++)
	{
		if(tmp[num]==num)
		{
			f_value++;
//			printf("FALSH AT45DBXX Read :%x \r\n",tmp[num]);			
		}
	}



	if(f_value==0xff)
	{
		f_value=0;
		printf("FALSH AT45DBXX Read Test OK \r\n");		
	}
	else 
	printf("FALSH AT45DBXX Read Test failure\r\n");

	while(1);

}

			
