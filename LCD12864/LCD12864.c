/********************************************************************************************************
*********************************************************************************************************
*
* File                : LCD12864.c
* Hardware Environment:
* Build Environment   : AVR Studio 4.16 + Winavr 20090313
* Version             : V1.0
* By                  : Wu Ze
*
*                                  (c) Copyright 2005-2009, WaveShare
*                                       http://www.waveShare.net
*                                          All Rights Reserved
*
*********************************************************************************************************
********************************************************************************************************/

#define _DVK501_M128_EX_ 1
#include <avr/io.h>
#include "ws_lcd_ST7920_port.h"

int main(void)
{
	st7920LcdInit(); /*LCD init*/

	showLine(3,2,lcd_buffer,"waveshare");
//	showLine(2,1,lcd_buffer,"深圳微雪电子");
//	showLine(2,2,lcd_buffer,"waveshare.net");

	refreshLCD(lcd_buffer); /*刷新LCD*/
	while(1)
	{
			showLine(2,1,lcd_buffer,"深圳微雪电子");
			showLine(2,2,lcd_buffer,"waveshare.net");		
	}

}
