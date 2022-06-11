/*
*========================================================================================================
*
* File                : ws_lcd_st7920_port.h
* Hardware Environment:	
* Build Environment   : AVR Studio 4.16 + Winavr 20090313
* Version             : V1.0
* By                  : Wu Ze
*
*                                  (c) Copyright 2005-2009, WaveShare
*                                       http://www.waveShare.net
*                                          All Rights Reserved
*
*========================================================================================================
*/

#ifndef _WS_LCD_ST7920_PORT_H
#define _WS_LCD_ST7920_PORT_H
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include <ws_lcd_st7920.h>
#include <ws_macro.h>

uint8_t lcd_buffer[64];

/*Hardware Environment：DVK501 && M128+ EX*/
#if defined(_DVK501_M128_EX_)

#define SET_CS_LCD() sbi(PORTB,4)
#define CLR_CS_LCD() cbi(PORTB,4)
#define OUT_CS_LCD() sbi(DDRB,4)

void st7920LcdInit(void)
{
DDRB|=0x87;
PORTB|=0xF8;
// SPI initialization
// SPI Type: Master
// SPI Clock Rate: 2*1843.200 kHz
// SPI Clock Phase: Cycle Half
// SPI Clock Polarity: Low
// SPI Data Order: MSB First
SPCR=0x50;
SPSR=0x01;
sendCodeST7920(0x0C);//整体显示
sendCodeST7920(0x01);//清屏
_delay_ms(2);
memset(lcd_buffer,' ',sizeof(lcd_buffer));
}

/*Hardware Environment：DVK501 && M16+ EX*/
#elif defined(_DVK501_M16_EX_)
void st7920LcdInit(void)
{
DDRB|=0xE0;
// SPI initialization
// SPI Type: Master
// SPI Clock Rate: 2*1843.200 kHz
// SPI Clock Phase: Cycle Half
// SPI Clock Polarity: Low
// SPI Data Order: MSB First
SPCR=0x50;
SPSR=0x01;
sendCodeST7920(0x0C);//整体显示
sendCodeST7920(0x01);//清屏
_delay_ms(2);
memset(lcd_buffer,' ',sizeof(lcd_buffer));
}
/*Hardware Environment：DVK501 && M48+ EX*/
#elif defined(_DVK501_M48_EX_)
void st7920LcdInit(void)
{
DDRB|=0x3C;
// SPI initialization
// SPI Type: Master
// SPI Clock Rate: 2*1843.200 kHz
// SPI Clock Phase: Cycle Half
// SPI Clock Polarity: Low
// SPI Data Order: MSB First
SPCR=0x50;
SPSR=0x01;
sendCodeST7920(0x0C);//整体显示
sendCodeST7920(0x01);//清屏
_delay_ms(2);
memset(lcd_buffer,' ',sizeof(lcd_buffer));
}
/*Hardware Environment：DVK501 && M169+ EX*/
#elif defined(_DVK501_M169_EX_)
void st7920LcdInit(void)
{
DDRB|=0x0F;
// SPI initialization
// SPI Type: Master
// SPI Clock Rate: 2*1843.200 kHz
// SPI Clock Phase: Cycle Half
// SPI Clock Polarity: Low
// SPI Data Order: MSB First
SPCR=0x50;
SPSR=0x01;
sendCodeST7920(0x0C);//整体显示
sendCodeST7920(0x01);//清屏
_delay_ms(2);
memset(lcd_buffer,' ',sizeof(lcd_buffer));
}

/*Hardware Environment：DVK501 && M162+ EX*/
#elif defined(_DVK501_M162_EX_)
void st7920LcdInit(void)
{
DDRB|=0xF0;
// SPI initialization
// SPI Type: Master
// SPI Clock Rate: 2*1843.200 kHz
// SPI Clock Phase: Cycle Half
// SPI Clock Polarity: Low
// SPI Data Order: MSB First
SPCR=0x50;
SPSR=0x01;
sendCodeST7920(0x0C);//整体显示
sendCodeST7920(0x01);//清屏
_delay_ms(2);
memset(lcd_buffer,' ',sizeof(lcd_buffer));
}
#else
  #warning "LCD1264_ST7920 interface undefined."
#endif

#endif /*_WS_LCD_ST7920_PORT_H*/
