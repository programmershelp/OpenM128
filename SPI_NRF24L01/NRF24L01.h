#ifndef _NRF24L01_
#define _NRF24L01_

#include<avr/io.h>








//uint8_t SPI_Write_Reg(uint8_t reg,uint8_t value);
/****向寄存器reg写一个字节，同时返回状态字节**************/

uint8_t SPI_RW_Reg(uint8_t reg,uint8_t value);

uint8_t SPI_Read_Reg(uint8_t reg);
uint8_t SPI_Read_Buf(uint8_t reg,uint8_t *pBuf,uint8_t bytes);
uint8_t SPI_Write_Buf(uint8_t reg,uint8_t *pBuf,uint8_t bytes);
uint8_t nRF24L01_RxPacket(unsigned char *rx_buf);
void nRF24L01_TxPacket(unsigned char *tx_buf);
void RX_Mode(void);
void TX_Mode(uint8_t * tx_buf);
void nRF24L01_Initial(void);
void nRF24L01_Config(void);
void NRF24L01_Send(void);
void NRF24L01_Receive(void);


#endif /*_NRF24L01_*/
