#ifndef _AT45DBXX_
#define _AT45DBXX_

void spiInitAt45db(void);

void spi_transmit_byte(uint8_t Data);
void write_buffer(uint16_t BufferOffset,uint8_t data);
uint8_t read_buffer(uint16_t BufferOffset);


#endif /*_WS_USART_H_*/
