#include<avr/io.h>
#include<avr/delay.h>
#include<avr/interrupt.h>
#include<stdio.h>
#include"USART.H"

#include"AT24Cxx.h"

#define AT24C_WR_ADDR		0xA0	//write device-address 
#define AT24C_RD_ADDR		0xA1	//read device-address


int main(void)
{
	uint8_t adr,WrDat,t;	
	uint8_t *pRdDat=&t;	

	usartInit();
	printf("*--------------------------------------------------- *\r\n");
	printf("*-------------------AT24CXX experiment-------------- *\r\n");
	printf("*--------------------------------------------------- *\r\n");

	adr=0x00;
	WrDat='0xaa';
	twi_init();	



	if( I2C_Write(AT24C_WR_ADDR,adr,WrDat)!=I2C_ERR )	//AT24CXX Write success£¿
	{
		_delay_ms(50);	//Wait EERPOM finish
		printf("Write data: %c\r\n",WrDat);

		if( I2C_Read(AT24C_WR_ADDR,adr,AT24C_RD_ADDR,pRdDat)!=I2C_ERR )	//AT24CXX read success?

			printf("Read data: %c\r\n",*pRdDat);
			printf("EEPROM 24C02 Read Test OK\r\n");

	}	


	while(1);
}
