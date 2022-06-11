#include"avr/io.h"
#include"SPI0.h"

//#define uint unsigned int
//#define uchar unsigned char


void SPI0_Init(void);
uchar SPI0_communication(uchar send_char);


//****************************************** 
//SPI初始化 
//****************************************** 

void SPI0_Init(void)
{

	SPCR=0x59;          //使能SPI ， 设置SPI为主模式，空闲时CLI为1，第一个上升沿采集数据，时钟    FOSC/16
	SPSR=0x00;			//setup SPI
	DDRB=0x07;
//	PORB=0X00;
//	SPI_RxHead = 0; 
//	SPI_TxHead = 0; 
	
}

//****************************************** 
//发送数据 
//****************************************** 

uchar SPI0_communication(uchar send_char)
{
	SPDR = send_char;               //启动数据传输
	while (!(SPSR & (1<<SPIF)));    //等待传输结束
	return SPDR;
}



























