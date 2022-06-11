/******************************************************************************

* File Name          : LTM022A69B.c
* Author             : Librae
* Last Modified Date : 08/12/2010
* Description        : This file provides the 
						LTM022A69B LCD related functions' declaration.

******************************************************************************/
#include <avr/io.h>
#include <util/delay.h>

#include "SPI0.h"
#include "LTM022A69B.h"
#include "LCD_lib.h"

void lcd_ctrl_port_init(void);      //nRS,nCS ... initialization

//=============================================================================
//							LCD Basic Functions
//=============================================================================
/******************************************************************************
* Function Name  : LCD_WRITE_REG
* Description    : write LCD register
* Input          : index
* Output         : None
* Return         : None
******************************************************************************/

void SPI0_Init(void)
{

	DDRB=0xff;
	PORTB=0X00;

	SPCR=0x50;          //使能SPI ， 设置SPI为主模式，空闲时CLI为1，第一个上升沿采集数据，时钟    FOSC/16
	SPSR=0x00;			//setup SPI

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




void LCD_WRITE_REG(unsigned int index)
{
	
	LCD_RS_L();
	LCD_CS_L();
    SPI0_communication((uchar)(index>>8));    //00000000 000000000
    SPI0_communication((uchar)(index));
	LCD_CS_H();
	LCD_RS_H();
}

/******************************************************************************
* Function Name  : LCD_WRITE_COMMAND
* Description    : send command to LCD
* Input          : index, data
* Output         : None
* Return         : None
******************************************************************************/
void LCD_WRITE_COMMAND(unsigned int index,unsigned int data)
{
	//select command register
	LCD_RS_L();
	LCD_CS_L();
    SPI0_communication((unsigned char)(index>>8));    //00000000 000000000
    SPI0_communication((unsigned char)(index));
	LCD_CS_H();
	//send data
	LCD_RS_H();

	LCD_CS_L();
    SPI0_communication((unsigned char)(data>>8));    //00000000 000000000
    SPI0_communication((unsigned char)(data));
	LCD_CS_H();
}

/*******************************************************************************
* Function Name  : LCD_WRITE_DATA
* Description    : write data to LCD
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WRITE_DATA(unsigned int data)
{
    SPI0_communication((unsigned char)(data>>8));    //00000000 000000000
    SPI0_communication((unsigned char)(data));
}
/********************************************************************************
* Function Name  : lcd_ctrl_port_init
* Description    : nRS,nCS ... initialization  for LPC1114 PIN
* Input          : None
* Output         : None
* Return         : None
********************************************************************************/
void lcd_ctrl_port_init()
{
	DDRD=0XFF;
//	PORTD=0X40;
    
}
/******************************************************************************
* Function Name  : lcd_init
* Description    : LCD initialization
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void lcd_init(void)
{
    lcd_ctrl_port_init();
    SPI0_Init();
//  reser LCD
	PORTB &= ~(1<<PB6);
	_delay_us(5);
	PORTB |= (1<<PB6);
	_delay_us(5);


	LCD_WRITE_COMMAND( 0x000, 0x0001 ); /* oschilliation start */
	_delay_us( 10 );
	/* Power settings */  	
	LCD_WRITE_COMMAND( 0x100, 0x0000 ); /*power supply setup*/	
	LCD_WRITE_COMMAND( 0x101, 0x0000 ); 
	LCD_WRITE_COMMAND( 0x102, 0x3110 ); 
	LCD_WRITE_COMMAND( 0x103, 0xe200 ); 
	LCD_WRITE_COMMAND( 0x110, 0x009d ); 
	LCD_WRITE_COMMAND( 0x111, 0x0022 ); 
	LCD_WRITE_COMMAND( 0x100, 0x0120 ); 
	_delay_us( 20 );

	LCD_WRITE_COMMAND( 0x100, 0x3120 );
	_delay_us( 80 );
	/* Display control */   
	LCD_WRITE_COMMAND( 0x001, 0x0100 );
	LCD_WRITE_COMMAND( 0x002, 0x0000 );
	LCD_WRITE_COMMAND( 0x003, 0x1230 );
	LCD_WRITE_COMMAND( 0x006, 0x0000 );
	LCD_WRITE_COMMAND( 0x007, 0x0101 );
	LCD_WRITE_COMMAND( 0x008, 0x0808 );
	LCD_WRITE_COMMAND( 0x009, 0x0000 );
	LCD_WRITE_COMMAND( 0x00b, 0x0000 );
	LCD_WRITE_COMMAND( 0x00c, 0x0000 );
	LCD_WRITE_COMMAND( 0x00d, 0x0018 );
	/* LTPS control settings */   
	LCD_WRITE_COMMAND( 0x012, 0x0000 );
	LCD_WRITE_COMMAND( 0x013, 0x0000 );
	LCD_WRITE_COMMAND( 0x018, 0x0000 );
	LCD_WRITE_COMMAND( 0x019, 0x0000 );

	LCD_WRITE_COMMAND( 0x203, 0x0000 );
	LCD_WRITE_COMMAND( 0x204, 0x0000 );

	LCD_WRITE_COMMAND( 0x210, 0x0000 );
	LCD_WRITE_COMMAND( 0x211, 0x00ef );
	LCD_WRITE_COMMAND( 0x212, 0x0000 );
	LCD_WRITE_COMMAND( 0x213, 0x013f );
	LCD_WRITE_COMMAND( 0x214, 0x0000 );
	LCD_WRITE_COMMAND( 0x215, 0x0000 );
	LCD_WRITE_COMMAND( 0x216, 0x0000 );
	LCD_WRITE_COMMAND( 0x217, 0x0000 );

	// Gray scale settings
	LCD_WRITE_COMMAND( 0x300, 0x5343);
	LCD_WRITE_COMMAND( 0x301, 0x1021);
	LCD_WRITE_COMMAND( 0x302, 0x0003);
	LCD_WRITE_COMMAND( 0x303, 0x0011);
	LCD_WRITE_COMMAND( 0x304, 0x050a);
	LCD_WRITE_COMMAND( 0x305, 0x4342);
	LCD_WRITE_COMMAND( 0x306, 0x1100);
	LCD_WRITE_COMMAND( 0x307, 0x0003);
	LCD_WRITE_COMMAND( 0x308, 0x1201);
	LCD_WRITE_COMMAND( 0x309, 0x050a);

	/* RAM access settings */ 
	LCD_WRITE_COMMAND( 0x400, 0x4027 );
	LCD_WRITE_COMMAND( 0x401, 0x0000 );
	LCD_WRITE_COMMAND( 0x402, 0x0000 );	/* First screen drive position (1) */   	
	LCD_WRITE_COMMAND( 0x403, 0x013f );	/* First screen drive position (2) */   	
	LCD_WRITE_COMMAND( 0x404, 0x0000 );

	LCD_WRITE_COMMAND( 0x200, 0x0000 );
	LCD_WRITE_COMMAND( 0x201, 0x0000 );
	
	LCD_WRITE_COMMAND( 0x100, 0x7120 );
	LCD_WRITE_COMMAND( 0x007, 0x0103 );
	_delay_us( 10 );
	LCD_WRITE_COMMAND( 0x007, 0x0113 );


	_delay_us(20);
	
	lcd_clear_screen(0xf800);
}

/******************************************************************************
* Function Name  : lcd_clear_screen
* Description    : clear screen
* Input          : color_background: refresh background color
* Output         : None
* Return         : None
******************************************************************************/
void lcd_clear_screen(unsigned int color_background)
{
	unsigned int i,j;

	LCD_WRITE_COMMAND(0x210,0x00);
	LCD_WRITE_COMMAND(0x212,0x0000);
	LCD_WRITE_COMMAND(0x211,0xEF);
	LCD_WRITE_COMMAND(0x213,0x013F);

	
	LCD_WRITE_COMMAND(0x200,0x0000);
	LCD_WRITE_COMMAND(0x201,0x0000);

	LCD_RS_L();
	LCD_WRITE_REG(0x202);	//RAM Write index
	LCD_CS_L();

	for(i=0;i<320;i++)
	{
		for(j=0;j<240;j++)
		{
		  LCD_WRITE_DATA(color_background);
		}
	}
	LCD_RS_H();
	LCD_CS_H();
}
/******************************************************************************
* Function Name  : lcd_clear_area
* Description    : clear area,fill
* Input          : color_front:draw a rectangle, fill with color_front
                   x:
                   y:
                   width: rectangle width
                   height:rectangle height
* Output         : None
* Return         : None
******************************************************************************/
void lcd_clear_area(unsigned int color_front, 
                    unsigned char x, 
                    unsigned int y, 
                    unsigned int width, 
                    unsigned height)
{
	unsigned int i, j;

	LCD_WRITE_COMMAND( 0x08, x ); 	//x start point
	LCD_WRITE_COMMAND( 0x0a, y ); 	//y start point
	LCD_WRITE_COMMAND( 0x09, x + width - 1 );	//x end point
	LCD_WRITE_COMMAND( 0x0b, y + height - 1 );	//y end point
		
	LCD_WRITE_COMMAND( 0x06, x );
	LCD_WRITE_COMMAND( 0x07, y );

	LCD_WRITE_REG( 0x0E );

	LCD_CS_L();
	LCD_RS_H();

	for( i = 0; i < height; i++ )
	{
		for( j = 0; j < width; j++ )
		{
			LCD_WRITE_DATA( color_front );
		}
	}
	LCD_CS_H();
}

/******************************************************************************
* Function Name  : lcd_set_cursor
* Description    : Set cursor
* Input          : x, y
* Output         : None
* Return         : None
******************************************************************************/
void lcd_set_cursor(unsigned char x, unsigned int y)
{
  	if( (x > 320) || (y > 240) )
	{
		return;
	}
	LCD_WRITE_COMMAND( 0x06, x );
	LCD_WRITE_COMMAND( 0x07, y );
}

/******************************************************************************
* Function Name  : lcd_display_char
* Description    : ch_asc: ascii code of data. position_x, position_y.
					color_front, color_background.
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void lcd_display_char(  unsigned char ch_asc, 
                        unsigned int color_front,
                        unsigned int color_background, 
                        unsigned char postion_x, 
                        unsigned char postion_y)
{
	unsigned char i, j, b;
	const unsigned char *p = 0;
	
	LCD_WRITE_COMMAND(0x08,postion_x*8); 	//x start point
	LCD_WRITE_COMMAND(0x0a,postion_y*16); 	//y start point
	LCD_WRITE_COMMAND(0x09,postion_x*8+7);	//x end point
	LCD_WRITE_COMMAND(0x0b,postion_y*16+15);	//y end point

	LCD_WRITE_COMMAND(0x06,postion_x*8);	//RAM X address(0 ~ FF, actually 0 ~ EF)	
	LCD_WRITE_COMMAND(0x07,postion_y*16);	//RAM Y address(0 ~ 1FF, actually 0 ~ 13F)
	
	LCD_WRITE_REG(0x0E);	//RAM Write index(prepare to write data)

	LCD_CS_L();
	LCD_RS_H();

	p = ascii;
	p += ch_asc*16;
	for(j=0;j<16;j++)
	{
		b=*(p+j);
		for(i=0;i<8;i++)
		{
			if( b & 0x80 )
			{
				LCD_WRITE_DATA(color_front);
			}
			else
			{
				LCD_WRITE_DATA(color_background);
			}
			b=b<<1;			
		}	
	}

	LCD_CS_H();
}

/******************************************************************************
* Function Name  : lcd_display_string
* Description    : *str: address of string data. 
					x: the xth row(0~30).
					y: the yth column(0~20).
					color_front, color_background.
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void lcd_display_string(unsigned char *str, 
						unsigned int color_front, 
						unsigned int color_background, 
						unsigned char x, 
						unsigned char y )
{
	while (*str) 
	{ 
		lcd_display_char( *str, color_front, color_background, x, y);
		if(++x>=30)
		{
			x=0;
			if(++y>=20)
			{
				y=0;
			}
		}
		str ++;
    }
}

/******************************************************************************
* Function Name  : 汉字显示
* Description    : 16x16点阵的汉字显示函数
* Input          : gb:汉字对应字模中的位置，x:显示位置第几列，y:显示位置第几行
					color_front, color_background.  
* Output         : None
* Return         : None
******************************************************************************/
/*显示方法（年月日）
for( i = 2; i < 8; i++ )
{
	lcd_display_GB2312( i-2, (i-1)*2, 0 );
}
*/
void lcd_display_GB2312( unsigned char gb, 
						unsigned int color_front, 
						unsigned int color_background, 
						unsigned char postion_x, 
						unsigned char postion_y )
{
	unsigned char i,j,b;
	unsigned char *p;
	
	LCD_WRITE_COMMAND(0x08,postion_x*16); 	//x start point
	LCD_WRITE_COMMAND(0x0a,postion_y*16); 	//y start point
	LCD_WRITE_COMMAND(0x09,postion_x*16+15);	//x end point
	LCD_WRITE_COMMAND(0x0b,postion_y*16+15);	//y end point
	LCD_WRITE_COMMAND(0x06,postion_x*16);	
	LCD_WRITE_COMMAND(0x07,postion_y*16);

	LCD_WRITE_REG(0x0E);	//RAM Write index

	LCD_CS_L();
	LCD_RS_H();

	p = (unsigned char *)GB2312;
	p += gb*32;
	for(j=0;j<32;j++)
	{
		b=*(p+j);
		for(i=0;i<8;i++)
		{
			if(b&0x80)
			{
				LCD_WRITE_DATA(color_front);
			}
			else
			{
				LCD_WRITE_DATA(color_background);
			}
			b=b<<1;
			
		}	
	}
	LCD_CS_H();
}

/******************************************************************************
* Function Name  : lcd_display_image
* Description    : Draw image
* Input          : x, y: image start at x, y. width, length, *img.
* Output         : None
* Return         : None
******************************************************************************/
void lcd_display_image( const unsigned char *img, 
					unsigned char x, 
					unsigned int y, 
					unsigned int width, 
					unsigned height )
{
	unsigned int i, j;
	unsigned int data16;


	LCD_WRITE_COMMAND( 0x08, x ); 	//x start point
	LCD_WRITE_COMMAND( 0x0a, y ); 	//y start point
	LCD_WRITE_COMMAND( 0x09, x + width - 1 );	//x end point
	LCD_WRITE_COMMAND( 0x0b, y + height - 1 );	//y end point
		
	LCD_WRITE_COMMAND( 0x06, x );
	LCD_WRITE_COMMAND( 0x07, y );

	LCD_WRITE_REG( 0x0E );

	LCD_CS_L();
	LCD_RS_H();

	for( i = 0; i < height; i++ )
	{
		for( j = 0; j < width; j++ )
		{
			//Be carful of MCU type, big endian or little endian
			//little endian
			data16 = ( *(img + 1) << 8 ) | (*img);
			LCD_WRITE_DATA( data16 );
			img += 2;
		}
	}
	LCD_CS_H();
}

//=============================================================================
//							Application Functions
//=============================================================================

/******************************************************************************
* Function Name  : lcd_display_test
* Description    : test
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
const unsigned int color[] =
{
	// 0 ~ 262143, 0x00000 ~ 0x3FFFF
	0xf800,	//red
	0x07e0,	//green
	0x001f,	//blue
	0xffe0,	//yellow
	0x0000,	//black
	0xffff,	//white
	0x07ff,	//light blue
	0xf81f	//purple
};

void lcd_display_test(void)
{
	unsigned int temp, num;
	unsigned int i;
	unsigned char n;

	lcd_clear_screen(RED);
    _delay_us(600);
	
	LCD_WRITE_COMMAND(0x210,0x00);
	LCD_WRITE_COMMAND(0x212,0x0000);
	LCD_WRITE_COMMAND(0x211,0xEF);
	LCD_WRITE_COMMAND(0x213,0x013F);
	
	LCD_WRITE_COMMAND(0x200,0x0000);
	LCD_WRITE_COMMAND(0x201,0x0000);

	LCD_RS_L();
	LCD_WRITE_REG(0x202);	//RAM Write index
	LCD_CS_L();
	//dispaly color bar
	for(n=0;n<8;n++)
	{
		LCD_CS_L();
		LCD_RS_H();
		temp=color[n];
		for(num=40*240;num>0;num--)
		{
			LCD_WRITE_DATA(temp);
		}
	}

//	_delay_us(600);

	//display pure color
	for(n=0;n<8;n++)
	{


	    temp=color[n];
		for(i=0;i<240;i++)
		{
			for(num=0;num<320;num++)
			{
		  		LCD_WRITE_DATA(temp);
			}
		}
	_delay_us(600);
	}
	lcd_clear_screen(GREEN);
	LCD_CS_H();	
	LCD_RS_H();
}

