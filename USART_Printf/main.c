#include<avr/io.h>
#include<avr/delay.h>
#include<avr/interrupt.h>
#include<stdio.h>
#include"USART.H"


int main(void)
{
	usartInit();
	printf("*--------------------------------------------* \r\n");
	printf("www.waveshare.net \r\n");
	printf("OpenM128 Development board\r\n");
	printf("*--------------------------------------------* \r\n");
	while(1);
}
