/*
*========================================================================================================
*
* File                : ws_usart.h
* Hardware Environment:	
* Build Environment   : AVR Studio 4.16 + Winavr 20090313
* Version             : V1.0
* By                  : Wu Ze
*
*                                  (c) Copyright 2005-2009, WaveShare
*                                       http://www.waveshare.net
*                                          All Rights Reserved
*
*========================================================================================================
*/
#ifndef _WS_USART_H_
#define _WS_USART_H_

#include <avr/io.h>

//从RS232发送一个字节
void putUsart0(uint8_t c)
{ 
	while(!(WS_UCSR0A & 0x20));
	WS_UDR0 = c;
}
//从RS232接收一个字节
uint16_t getUsart0(void)
{
    uint8_t status,res;
	if(!(WS_UCSR0A & 0x80)) return 0xFFFF;        //no data to be received 
    status = WS_UCSR0A;
    res = WS_UDR0;
	PORTA=res;
    if (status & 0x1c) return 0xFFFF;        // If error, return 0xFFFF
	

    return res;
}
//等待从RS232接收一个有效的字节
uint8_t waitUsart0(void)
{
    uint16_t c;
    while((c=getUsart0())==0xFFFF);

    return (uint8_t)c;
}

#endif /*_WS_USART_H_*/
