#include<avr/io.h>
#include<avr/delay.h>
#include"USART.H"
#include"NRF24L01.h"




// SPI(nRF24L01) commands
#define READ_REG_NRF24L01    	0x00 				// Define read command to register
#define WRITE_REG_NRF24L01   	0x20 				// Define write command to register
#define RD_RX_PLOAD 			0x61 				// Define RX payload register address
#define WR_TX_PLOAD 			0xA0 				// Define TX payload register address
#define FLUSH_TX    			0xE1 				// Define flush TX register command
#define FLUSH_RX    			0xE2 				// Define flush RX register command
#define REUSE_TX_PL 			0xE3 				// Define reuse TX payload register command
#define NOP         			0xFF 				// Define No Operation, might be used to read status register
//***************************************************//
// SPI(nRF24L01) registers(addresses)
#define CONFIG      			0x00				// 'Config' register address
#define EN_AA       			0x01                		// 'Enable Auto Acknowledgment' register address
#define EN_RXADDR   			0x02                		// 'Enabled RX addresses' register address
#define SETUP_AW    			0x03                		// 'Setup address width' register address
#define SETUP_RETR  			0x04                		// 'Setup Auto. Retrans' register address
#define RF_CH       			0x05                		// 'RF channel' register address
#define RF_SETUP    			0x06 				// 'RF setup' register address
#define STATUS      			0x07 				// 'Status' register address
#define OBSERVE_TX  			0x08 				// 'Observe TX' register address
#define CD          			0x09 				//'Carrier Detect' register address
#define RX_ADDR_P0  			0x0A				// 'RX address pipe0' register address
#define RX_ADDR_P1  			0x0B 				// 'RX address pipe1' register address
#define RX_ADDR_P2  			0x0C 				// 'RX address pipe2' register address
#define RX_ADDR_P3  			0x0D 				// 'RX address pipe3' register address
#define RX_ADDR_P4  			0x0E 				// 'RX address pipe4' register address
#define RX_ADDR_P5  			0x0F				// 'RX address pipe5' register address
#define TX_ADDR     			0x10 				// 'TX address' register address
#define RX_PW_P0    			0x11 				// 'RX payload width, pipe0' register address
#define RX_PW_P1    			0x12 				// 'RX payload width, pipe1' register address
#define RX_PW_P2    			0x13 				// 'RX payload width, pipe2' register address
#define RX_PW_P3    			0x14 				// 'RX payload width, pipe3' register address
#define RX_PW_P4    			0x15 				// 'RX payload width, pipe4' register address
#define RX_PW_P5    			0x16 				// 'RX payload width, pipe5' register address
#define FIFO_STATUS 			0x17 			    	// 'FIFO Status Register' register address

#define TX_ADR_WIDTH   	5  // 5字节宽度的发送/接收地址
#define TX_PLOAD_WIDTH 	32  // 数据通道有效数据宽度



// CE Pin & CSN Pin & IRQ Pin
/*
IRQ(PB4)   CSN(PD6)   CE(PD7)
*/

#define IRQ		  (PINB & (1<<4))

#define CE_0      PORTD &= ~(1<<7);
#define CE_1      PORTD |= (1<<7);
	              
#define CSN_0     PORTD &= ~(1<<6);
#define CSN_1     PORTD |= (1<<6);




#define	RX_DR			0x40
#define	TX_DS			0x20
#define	MAX_RT			0x10


uint8_t TX_ADDRESS[TX_ADR_WIDTH] = {0xb2,0xb2,0xb3,0xb4,0x01};  // 定义一个静态发送地址	 (Define a static send address)


uint8_t RX_BUF[TX_PLOAD_WIDTH];

uint8_t TX_BUF[TX_PLOAD_WIDTH];


uint8_t SPI_RW(uint8_t Data)
{
	SPDR = Data;
    while(!(SPSR&0x80));
	return SPDR;
}

/****向寄存器reg写一个字节，同时返回状态字节**************/
/****Reg to register a byte written while returning state bytes****/
uint8_t SPI_RW_Reg(uint8_t reg,uint8_t value)
{
	uint8_t status;

	CSN_0;
	status=SPI_RW(reg);   //select register  and write value to it
	SPI_RW(value);   
	CSN_1;
	return(status); 
}

/****向寄存器reg读一个字节，同时返回状态字节**************/
/****To register reg read a byte, and return to state bytes*****/
uint8_t SPI_Read_Reg(uint8_t reg)
{
	uint8_t status;
	CSN_0;
	SPI_RW(reg);
    status = SPI_RW(0);

	CSN_1;
	return(status);
}

/********读出bytes字节的数据*************************/
/********Read the bytes bytes of data****************/

uint8_t SPI_Read_Buf(uint8_t reg,uint8_t *pBuf,uint8_t bytes)
{
	uint8_t status,byte_ctr;

	CSN_0;
	status=SPI_RW(reg);       
	for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
		pBuf[byte_ctr]=SPI_RW(0);

	CSN_1;
	return(status);
}

/****************写入bytes字节的数据*******************/
/****************Write bytes bytes of data*************/
uint8_t SPI_Write_Buf(uint8_t reg,uint8_t *pBuf,uint8_t bytes)
{
	uint8_t status,byte_ctr;
	CSN_0;
	status=SPI_RW(reg); 
	_delay_us(10);
	for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
		SPI_RW(*pBuf++);
	CSN_1;
	return(status);
}

/*接收函数，返回1表示有数据收到，否则没有数据接收到**/
/*Receive function, and returns 1 said data received, or no data to receive*/
uint8_t nRF24L01_RxPacket(uint8_t* rx_buf)
{
    uint8_t status,revale=0;
	CE_0;
	_delay_us(10);
	status=SPI_RW(STATUS);	// 读取状态寄存其来判断数据接收状况	(Check the state judge read data receiving condition)

	printf("STATUS接受状态：0x%2x\r\n",status);

	if(status & RX_DR)				// 判断是否接收到数据(Judge whether to receive data)
	{
		SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer

		revale =1;			
	}
	SPI_RW_Reg(WRITE_REG_NRF24L01 + STATUS,status);   //接收到数据后RX_DR,TX_DS,MAX_PT都置高为1，通过写1来清楚中断标志
	CE_1;
	return revale;	
}

 /****************发送函数***************************/
void nRF24L01_TxPacket(unsigned char * tx_buf)
{
	CE_0;			//StandBy I模式	
	SPI_Write_Buf(WRITE_REG_NRF24L01 + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // 装载接收端地址	(Loading the receiver address)
	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 			 // 装载数据(Load data)	
	SPI_RW_Reg(WRITE_REG_NRF24L01 + CONFIG, 0x0e);   		 // IRQ收发完成中断响应，16位CRC，主发送(IRQ transceiver complete interrupt response, 16 CRC, the Lord sent)
	CE_1;		 //置高CE，激发数据发送(Buy high CE, stimulate the data sent)
	_delay_us(10);
}


void RX_Mode(void)
{
	CE_0;
  	SPI_Write_Buf(WRITE_REG_NRF24L01 + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // 接收设备接收通道0使用和发送设备相同的发送地址
  	SPI_RW_Reg(WRITE_REG_NRF24L01 + RX_PW_P0, TX_PLOAD_WIDTH);  // 接收通道0选择和发送通道相同有效数据宽度 
 
  	SPI_RW_Reg(WRITE_REG_NRF24L01 + EN_AA, 0x3f);               // 使能接收通道0自动应答
  	SPI_RW_Reg(WRITE_REG_NRF24L01 + EN_RXADDR, 0x3f);           // 使能接收通道0
  	SPI_RW_Reg(WRITE_REG_NRF24L01 + RF_CH, 40);                 // 选择射频通道0x40

  	SPI_RW_Reg(WRITE_REG_NRF24L01 + RF_SETUP, 0x07);            // 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益
  	SPI_RW_Reg(WRITE_REG_NRF24L01 + CONFIG, 0x0f);              // CRC使能，16位CRC校验，上电，接收模式
  	CE_1;
	_delay_us(10);
}

void TX_Mode(uint8_t * tx_buf)
{
	CE_0;
  	SPI_Write_Buf(WRITE_REG_NRF24L01 + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);     // 写入发送地址
  	SPI_Write_Buf(WRITE_REG_NRF24L01 + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // 为了应答接收设备，接收通道0地址和发送地址相同
  	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 			 // 装载数据
  	SPI_RW_Reg(WRITE_REG_NRF24L01 + EN_AA, 0x3f);       // 使能接收通道0自动应答
  	SPI_RW_Reg(WRITE_REG_NRF24L01 + EN_RXADDR, 0x3f);   // 使能接收通道0
  	SPI_RW_Reg(WRITE_REG_NRF24L01 + SETUP_RETR, 0x0a);  // 自动重发延时等待250us+86us，自动重发10次
  	SPI_RW_Reg(WRITE_REG_NRF24L01 + RF_CH, 40);         // 选择射频通道0x40
  	SPI_RW_Reg(WRITE_REG_NRF24L01 + RF_SETUP, 0x07);    // 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益
	SPI_RW_Reg(WRITE_REG_NRF24L01 + RX_PW_P0, TX_PLOAD_WIDTH);  // 接收通道0选择和发送通道相同有效数据宽度
  	SPI_RW_Reg(WRITE_REG_NRF24L01 + CONFIG, 0x0e);      // CRC使能，16位CRC校验，上电
	CE_1;
	_delay_us(10);
} 

void nRF24L01_Initial(void)
{
/* IRQ(PB4)   CSN(PD6)   CE(PD7) */
/* MISO(PB3)  MOSI(PB2)  SCK(PB1) SS(PB0) */

	DDRB |= (1<<1) | (1<<2) | (1<<0) ;        // ->output
	DDRB &= ~((1<<4)|(1<<3));
	PORTB |=  (1<<3) |(1<<4) |(1<<2) |(1<<1);
	PORTB &= ~(1<<0);

	DDRD |= (1<<6) | (1<<7);
	PORTD |=  (1<<6) |(1<<7);
//    while(1);
//	SPCR = 0X5E;
//    SPSR=0x00;
	SPCR=0x50;          //使能SPI ， 设置SPI为主模式，空闲时CLI为1，第一个上升沿采集数据，时钟    FOSC/16
	SPSR=0x00;			//setup SPI
	
	 
 	CSN_1;
	CE_0; 
}


/****************** 配置函数********************************/
void nRF24L01_Config(void)
{

	SPI_RW_Reg(WRITE_REG_NRF24L01 + CONFIG, 0x0e); // Set PWR_UP bit, enable CRC(2 bytes) &Prim:RX. RX_DR enabled..
	SPI_RW_Reg(WRITE_REG_NRF24L01 + EN_AA, 0x3f);
	SPI_RW_Reg(WRITE_REG_NRF24L01 + EN_RXADDR, 0x3f); // Enable Pipe0
//	SPI_RW_Reg(WRITE_REG_NRF24L01 + SETUP_AW, 0x02); // Setup address width=5 bytes
//	SPI_RW_Reg(WRITE_REG_NRF24L01 + SETUP_RETR, 0x1a); // 500us + 86us, 10 retrans...
	SPI_RW_Reg(WRITE_REG_NRF24L01 + RF_CH, 40);
	SPI_RW_Reg(WRITE_REG_NRF24L01 + RF_SETUP,0x07); // TX_PWR:0dBm, Datarate:2Mbps,
}
 
void NRF24L01_Send(void)
{
    uint8_t status=0x00;
	


	CE_0;
	TX_Mode(TX_BUF);
//	while(IRQ);
	_delay_us(10);
	status=SPI_Read_Reg(STATUS);	// 读取状态寄存其来判断数据接收状况
	printf("STATUS=0x%2x\r\n",status);
	if(status&TX_DS)	/*tx_ds == 0x20*/
	{
		printf("STATUS=0x%x\r\n",status);
		printf("XXXXXX!!\r\n");	
		SPI_RW_Reg(WRITE_REG_NRF24L01 + STATUS, 0x20);      // 清除TX，让IRQ拉低；	
	}
	else if(status&MAX_RT)  /* status = 0x10*/
		{
			printf("Send to achieve maximum number of sending!!\r\n");	
			SPI_RW_Reg(WRITE_REG_NRF24L01 + STATUS, 0x10);      // 清除TX，让IRQ拉低；			
		}
	CE_1;
}

void NRF24L01_Receive(void)
{   
    uint8_t status=0x01;  
	RX_Mode();
	while(IRQ);
	CE_0;
	_delay_us(10);
	status=SPI_Read_Reg(STATUS);					    	// 读取状态寄存其来判断数据接收状况
	printf("STATUS=：0x%x\r\n",status);
	if(status & 0x40)								        //接受中断标志位
	{
		SPI_Read_Buf(RD_RX_PLOAD,RX_BUF,TX_PLOAD_WIDTH);    // read receive payload from RX_FIFO buffer
		SPI_RW_Reg(WRITE_REG_NRF24L01 + STATUS, 0x40);      // 清除TX，让IRQ拉低
		printf("Receiving data:0x%x\r\n",RX_BUF[0]);	
	}  
	CE_1;

}


