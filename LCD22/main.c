#include <avr/io.h>
//#include<avr/interrupt.h>
#include <util/delay.h>

#include "SPI0.h"
#include "LTM022A69B.h"
#include "LCD_lib.h"


int main(void)

{

	DDRA=0XFF;
	PORTA=0XFF;
	DDRD=0X40;

	lcd_init();
	_delay_ms(10);
	lcd_display_test();

	while(1);
}

 
