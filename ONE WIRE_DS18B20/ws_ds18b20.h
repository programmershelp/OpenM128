/********************************************************************************************************
*********************************************************************************************************
*
* File                : ws_ds18b20.h
* Hardware Environment:
* Build Environment   : AVR Studio 4.16 + Winavr 20090313 (ICCAVR-->GCCAVR)
* Version             : V1.0
* By                  : Wu Ze
*
*                                  (c) Copyright 2005-2009, WaveShare
*                                       http://www.waveShare.net
*                                          All Rights Reserved
*
*********************************************************************************************************
********************************************************************************************************/

#ifndef _ws_ds18b20_H_
#define _ws_ds18b20_H_


#define sbi(io,bit)		(  io |=  (1<<bit) )	
#define cbi(io,bit)		(  io &= ~(1<<bit) )	
#define gbi(pin ,bit)	( pin &   (1<<bit) )

#define SET_DQ() sbi(PORTG,2)
#define CLR_DQ() cbi(PORTG,2)
#define OUT_DQ() sbi(DDRG,2)
#define IN_DQ() cbi(DDRG,2)
#define GET_DQ() gbi(PING,2)


//-----复位-----
void resetOnewire(void) 
{
	OUT_DQ();
    CLR_DQ();
    _delay_us(750);
    SET_DQ();
	IN_DQ(); 
	while(GET_DQ());
	while(!(GET_DQ()));
	OUT_DQ();
}

//-----读数据-----
uint8_t rOnewire(void)
{
    uint8_t data=0,i=0;
    for(i=0;i<8;i++)
    {
		data=data>>1;
		OUT_DQ();
        CLR_DQ();
        _delay_us(5);
		SET_DQ();
		_delay_us(15);
		IN_DQ();
        if(GET_DQ()) data|=0x80;
        else while(!(GET_DQ()));
		_delay_us(60);
		SET_DQ();
	}
	return(data);
}
//-----写数据-----
void wOnewire(uint8_t data)
{
    uint8_t i=0;
	OUT_DQ();
    for(i=0;i<8;i++)
    {
        if(data&0x01)
		{
            CLR_DQ();
            _delay_us(5);
            SET_DQ();
            _delay_us(85); //65
		}
   	 	else
   	 	{
       		CLR_DQ(); 
        	_delay_us(90);  //65
        	SET_DQ();
        	_delay_us(5);
    	}
    	data=data>>1;
    }
}

//-----DS18B20转换温度-----
void convertDs18b20(void) 
{ 
    resetOnewire(); 
    wOnewire(0xcc); 
    wOnewire(0x44); 
} 
//------------DS18BB0读温度----------
unsigned int readTemp(void) 
{ 
	uint8_t temp1,temp2;
	convertDs18b20();
    resetOnewire(); 
    wOnewire(0xcc); 
    wOnewire(0xbe); 	
    temp1=rOnewire(); 
    temp2=rOnewire(); 
	return (((temp2<<8)|temp1)*6.25); //0.0625=xx, 0.625=xx.x, 6.25=xx.xx
}

#endif
