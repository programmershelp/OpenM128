#include <avr/io.h>

#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include"PS2.h"

extern unsigned char g_KeyAsiic;


int main(void)
{
//	unsigned char key_value;
	PORT_Init();
	INT0_Init();
	usartInit();

	printf("*-----------------------------------------------------------------*\r\n");
	printf("*---------------------PS2 keyboard experiment---------------------*\r\n");
	printf("*-----------------------------------------------------------------*\r\n");



	while(1)
	{
		g_KeyAsiic = PS2_KeyScan(); //Access keys ASIIC PS2

		if(g_KeyAsiic != 0xff) //If there is a button press
		{
			printf("Keyboard Input : %c\r\n",g_KeyAsiic );
		}
	}

}
