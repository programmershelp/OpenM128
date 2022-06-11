#define _DVK501_M128_EX_ 1
#include <avr/io.h>
#include <util/delay.h>
#include <ws_iic_port.h>
#include <ws_pcf8563_port.h>

int main(void)
{
	DDRA=0xFF;
	PORTA=0x00;

	uint8_t time[3];
	uint8_t Flag,Flag1;
	usartInit();
	printf("*--------------------------------------------------- *\r\n");
	printf("*-------------------AT24CXX experiment-------------- *\r\n");
	printf("*--------------------------------------------------- *\r\n");


	twi_init();	
	PCF8563_init();	
	PCF8563_setTime(0,0,0); 
	
	while(1)
	{	
		while(1)
		{
			PCF8563_getTime(time);

			if(Flag1 != Flag)
			{
				PORTA = time[0];
				printf("Real time clock  %d:%d:%d \r \n",time[2],time[1],time[0]);
				Flag1=Flag;
			}  	
			Flag = time[0];

		}
	}
}
