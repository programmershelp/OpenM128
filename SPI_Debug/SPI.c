#include<avr/io.h>
#define uchar unsigned char
#define SCK 2
uchar date[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

void spi_init()
{
	DDRB = 0xf7;
	SPCR = 0X5E;
}

void spiSendChar(uint8_t send_char)
{
    PORTB &= ~(1<<SCK);
	SPDR = send_char;
	while (!(SPSR & (1<<SPIF)));
	PORTB |= (1<<SCK);
}

int main(void)
{
	
	spi_init();
    while(1)
	{
	   spiSendChar(0x0d);
	}
}

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
