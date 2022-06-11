#include<avr/io.h>
#include<avr/delay.h>
#include"USART.H"
#include"NRF24L01.h"

#define Open_TX
//#define Open_RX


extern uint8_t RX_BUF[];
extern uint8_t TX_BUF[];

int main(void)
{
	usartInit();
	printf("*--------------------------------------------------- *\r\n");
	printf("*-------------------NRF24L01 experiment-------------- *\r\n");
	printf("*--------------------------------------------------- *\r\n");

	nRF24L01_Initial();
	while(1)
	{
		#ifdef Open_TX
			TX_BUF[0]=0x55;
			NRF24L01_Send();
			printf("\r\nSend data£º0x%x\r\n",TX_BUF[0]);
			_delay_ms(1000);
		#else
			NRF24L01_Receive();
		#endif

	}

}




			
