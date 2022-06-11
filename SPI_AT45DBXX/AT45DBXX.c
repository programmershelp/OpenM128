#include<avr/io.h>
#include<avr/delay.h>
#include"USART.H"

#include"AT45DBXX.h"


#define CLR_SS_AT45DB() PORTB &= ~(1<<PB0);
#define SET_SS_AT45DB() PORTB |= (1<<PB0);


void spiInitAt45db(void)
{
	DDRB|=0x07;
	PORTB&=0xF8;
	SPCR=0x50;
	SPSR=0x01;
}

//==================================
void spi_transmit_byte(uint8_t Data)
{	
	SPDR=Data;								
    while(!(SPSR&0x80));
}

//============================================================
void write_buffer(uint16_t BufferOffset,uint8_t data)
{			
	CLR_SS_AT45DB();		
	spi_transmit_byte(0x84);//84			  						
	spi_transmit_byte(0xff);						
	spi_transmit_byte((uint8_t)(BufferOffset>>8));	
	spi_transmit_byte((uint8_t)BufferOffset);		
	spi_transmit_byte(data);		
	SET_SS_AT45DB();	
}
//================================================================
uint8_t read_buffer(uint16_t BufferOffset)
{		
    uint8_t temp;
	CLR_SS_AT45DB();
 	spi_transmit_byte(0xD4);//54
	spi_transmit_byte(0xff);
	spi_transmit_byte((uint8_t)(BufferOffset>>8));
	spi_transmit_byte((uint8_t)BufferOffset);
	spi_transmit_byte(0xff);
	spi_transmit_byte(0xff);
	temp=SPDR;	  
	SET_SS_AT45DB();
    return temp;								
}
