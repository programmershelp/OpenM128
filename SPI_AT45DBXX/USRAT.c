
#include<avr/io.h>
#include<avr/delay.h>
#include<avr/interrupt.h>
#include<stdio.h>
#include"USART.H"

#define WS_UCSR0A UCSR0A
#define WS_UDR0 UDR0



static int uart_putchar(char c, FILE *stream);  //Add custom putchar function  

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL,_FDEV_SETUP_WRITE); 


//From RS232 sends a byte
static int uart_putchar(char c, FILE *stream)    
{ 
  if (c == '\n')        
  uart_putchar('\r', stream);      
  loop_until_bit_is_set(UCSR0A, UDRE0);      
  UDR0 = c;      
  return 0;    
} 

void putUsart0(unsigned char c)
{ 
	while(!(WS_UCSR0A & 0x20));
	WS_UDR0 = c;
}
//From receiving a RS232 bytes
unsigned int getUsart0(void)
{
    unsigned char status,res;
	if(!(WS_UCSR0A & 0x80)) return 0xFFFF;        //no data to be received 
    status = WS_UCSR0A;
    res = WS_UDR0;
    if (status & 0x1c) return 0xFFFF;        // If error, return 0xFFFF 
    return res;
}
//Waiting for receiving a from RS232 effective bytes
unsigned char waitUsart0(void)
{
    unsigned int c;
    while((c=getUsart0())==0xFFFF);
    return (unsigned char)c;
}



void usartInit(void)
{
	// USART0 initialization
	// Communication Parameters: 8 Data, 1 Stop, No Parity
	// USART0 Receiver: On
	// USART0 Transmitter: On
	// USART0 Mode: Asynchronous
	// USART0 Baud Rate: 9600
	//
	UCSR0A=0x00;
	UCSR0B=0x98;
	UCSR0C=0x06;
	UBRR0H=0x00;
	UBRR0L=0x2F;
	stdout = &mystdout;       //Must add 
}





