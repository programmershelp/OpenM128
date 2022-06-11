/*
*========================================================================================================
*
* File                : ws_usart_port.h
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
#ifndef _WS_USART_PORT_H_
#define _WS_USART_PORT_H_

#include <avr/io.h>

/*Hardware Environment£ºDVK501 && M128+ EX*/
#if defined(_DVK501_M128_EX_)
void usartInit(void)
{
// USART1 initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART1 Receiver: On
// USART1 Transmitter: On
// USART1 Mode: Asynchronous
// USART1 Baud Rate: 9600

UCSR1A=0x00;
UCSR1B=0x98;
UCSR1C=0x06;
UBRR1H=0x00;
UBRR1L=0x2F;
}

#define WS_UCSR1A UCSR1A
#define WS_UDR1 UDR1

/*Hardware Environment£ºDVK501 && M16+ EX*/
#elif defined(_DVK501_M16_EX_)
void usartInit(void)
{
// USART initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART Receiver: On
// USART Transmitter: On
// USART Mode: Asynchronous
// USART Baud Rate: 9600
UCSRA=0x00;
UCSRB=0x98;
UCSRC=0x86;
UBRRH=0x00;
UBRRL=0x2F;

}

#define WS_UCSR1A UCSRA
#define WS_UDR1 UDR
/*Hardware Environment£ºDVK501 && M48+ EX*/
#elif defined(_DVK501_M48_EX_)
void usartInit(void)
{
// USART1 initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART1 Receiver: On
// USART1 Transmitter: On
// USART1 Mode: Asynchronous
// USART1 Baud Rate: 9600
UCSR1A=0x00;
UCSR1B=0x98;
UCSR1C=0x06;
UBRR1H=0x00;
UBRR1L=0x2F;

}
#define WS_UCSR1A UCSR1A
#define WS_UDR1 UDR1

/*Hardware Environment£ºDVK501 && M169+ EX*/
#elif defined(_DVK501_M169_EX_)
void usartInit(void)
{
// USART initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART Receiver: On
// USART Transmitter: On
// USART1 Mode: Asynchronous
// USART Baud Rate: 9600
UCSR1A=0x00;
UCSR1B=0x98;
UCSR1C=0x06;
UBRR1H=0x00;
UBRR1L=0x2F;

}
#define WS_UCSR1A UCSR1A
#define WS_UDR1 UDR1
/*Hardware Environment£ºDVK501 && M162+ EX*/
#elif defined(_DVK501_M162_EX_)
void usartInit(void)
{
// USART1 initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART1 Receiver: On
// USART1 Transmitter: On
// USART1 Mode: Asynchronous
// USART1 Baud Rate: 9600
UCSR1A=0x00;
UCSR1B=0x98;                                                        
UCSR1C=0x86;
UBRR1H=0x00;
UBRR1L=0x2F;

}
#define WS_UCSR1A UCSR1A
#define WS_UDR1 UDR1
#else
  #warning "USART interface undefined."
#endif

#include "ws_usart.h"

#endif /*_WS_USART_PORT_H_*/
