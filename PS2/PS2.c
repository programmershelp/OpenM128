#include <avr/io.h>

#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>


#include "PS2.h"
#define NULL 0xFF
#define TRUE 1
#define FALSE 0


#define	BAUDRATE 9600
#define	PS2_DB_INPUT (PINE & (1 << PE6))


typedef struct _PS2_KEY_STATUS //To define new key state structure body type
{
	unsigned char bKeyNewFlag; //he new key sign 
	unsigned char bBreakFlag; //Break code mark
	unsigned char bShiftFlag; //The shift key press sign
	unsigned char bKeyAsiic; //The ASIIC code value of buttons
}PS2_KEY_STATUS;

PS2_KEY_STATUS PS2KeyStatus;



const unsigned char PS2CodeUnShift[][2] PROGMEM = //The shift key didn't press the decoding table
{		
	{0x0e, '`'},
	{0x15, 'q'},
	{0x16, '1'},
	{0x1a, 'z'},
	{0x1b, 's'},
	{0x1c, 'a'},
	{0x1d, 'w'},
	{0x1e, '2'},
	{0x21, 'c'},
	{0x22, 'x'},
	{0x23, 'd'},
	{0x24, 'e'},
	{0x25, '4'},
	{0x26, '3'},
	{0x29, ' '},
	{0x2a, 'v'},
	{0x2b, 'f'},
	{0x2c, 't'},
	{0x2d, 'r'},
	{0x2e, '5'},
	{0x31, 'n'},
	{0x32, 'b'},
	{0x33, 'h'},
	{0x34, 'g'},
	{0x35, 'y'},
	{0x36, '6'},
	{0x39, ','},
	{0x3a, 'm'},
	{0x3b, 'j'},
	{0x3c, 'u'},
	{0x3d, '7'},
	{0x3e, '8'},
	{0x41, ','},
	{0x42, 'k'},
	{0x43, 'i'},
	{0x44, 'o'},
	{0x45, '0'},
	{0x46, '9'},
	{0x49, '.'},
	{0x4a, '/'},
	{0x4b, 'l'},
	{0x4c, ';'},
	{0x4d, 'p'},
	{0x4e, '-'},
	{0x52, '\''},
	{0x54, '['},
	{0x55, '='},
	{0x5b, ']'},
	{0x5d, '\\'},
	{0x61, '<'},
	{0x69, '1'},
	{0x6b, '4'},
	{0x6c, '7'},
	{0x70, '0'},
	{0x71, '.'},
	{0x72, '2'},
	{0x73, '5'},
	{0x74, '6'},
	{0x75, '8'},
	{0x79, '+'},
	{0x7a, '3'},
	{0x7b, '-'},
	{0x7c, '*'},
	{0x7d, '9'},
	{0x00, 0x00}
};

const unsigned char PS2CodeShift[][2] PROGMEM = //The shift key press decoding table
{
	0x0e,'~',
	0x15,'Q',
	0x16,'!',
	0x1a,'Z',
	0x1b,'S',
	0x1c,'A',
	0x1d,'W',
	0x1e,'@',
	0x21,'C',
	0x22,'X',
	0x23,'D',
	0x24,'E',
	0x25,'$',
	0x26,'#',
	0x29,' ',
	0x2a,'V',
	0x2b,'F',
	0x2c,'T',
	0x2d,'R',
	0x2e,'%',
	0x31,'N',
	0x32,'B',
	0x33,'H',
	0x34,'G',
	0x35,'Y',
	0x36,'^',
	0x39,'L',
	0x3a,'M',
	0x3b,'J',
	0x3c,'U',
	0x3d,'&',
	0x3e,'*',
	0x41,'<',
	0x42,'K',
	0x43,'I',
	0x44,'O',
	0x45,')',
	0x46,'(',
	0x49,'>',
	0x4a,'?',
	0x4b,'L',
	0x4c,':',
	0x4d,'P',
	0x4e,'_',
	0x52,'"',
	0x54,'{',
	0x55,'+',
	0x5b,'}',
	0x5d,'|',
	0x61,'>',
	0x69,'1',
	0x6b,'4',
	0x6c,'7',
	0x70,'0',
	0x71,'.',
	0x72,'2',
	0x73,'5',
	0x74,'6',
	0x75,'8',
	0x79,'+',
	0x7a,'3',
	0x7b,'-',
	0x7c,'*',
	0x7d,'9',
	0,0
};






unsigned char g_KeyAsiic;

/************************************************************************
*name: void DelayMS(unsigned int uiMS)  									 
*function: delay  function      									
*parameters: no     		                   									
*Return  no		                           								
************************************************************************/

void DelayMS(unsigned int wMS)
{	
	while(wMS--)
	{
		_delay_loop_2(2000);	// 延时1ms(粗略)
	}
}

/*************************************************************************
*name: void PORT_Init(void)				 									 
*function: Gratuitous mouth initialization				       									 
*parameters: no    		                   									 
*Return: no	                           								 
*************************************************************************/	
void PORT_Init(void)
{

	DDRE &= ~(1 << PE6); //PE6 output high level, PE6 input take to pull
	PORTE |= (1 << PE6);
	PORTE |= (1 << PE7); //PC0 input take to pull


	PS2KeyStatus.bKeyNewFlag = FALSE; //Initial state key structures variables
	PS2KeyStatus.bBreakFlag = FALSE;
	PS2KeyStatus.bShiftFlag = FALSE;
	PS2KeyStatus.bKeyAsiic = NULL;
}


void INT0_Init(void)
{
	EICRB |= (1 << ISC71); //INT0 down along the trigger
	EIMSK |= (1 << INT7); //That can INT0 interrupt
	sei();
}



/************************************************************************
*name: unsigned char PS2_KeyDeCode(unsigned char bKeyCode)					 
*function: PS2 keyboard decoding  program		       									    
*parameters: bKeyCode Keyboard yards 							
*Return: The key ASIIC yards		                           								
************************************************************************/
unsigned char PS2_KeyDeCode(unsigned char bKeyCode)
{
	unsigned char i;
	if (!PS2KeyStatus.bBreakFlag)//Make code
	{
		switch (bKeyCode)
		{
			case 0xF0: 
			{
				PS2KeyStatus.bBreakFlag = 1;
				break;
			}
			case 0x12:	  
			{
				PS2KeyStatus.bShiftFlag = 1; 
				break;
			}
			case 0x59:	   
			{
				PS2KeyStatus.bShiftFlag = 1;
				break;
			}	   
			default:        
			{	
				PS2KeyStatus.bBreakFlag = FALSE;
				if(!PS2KeyStatus.bShiftFlag) 
				{ 	 
					for(i = 0; i < 65; i++)
					{
						if(pgm_read_byte(PS2CodeUnShift[i]) == bKeyCode) 
						{
							PS2KeyStatus.bKeyAsiic = 
									pgm_read_byte(PS2CodeUnShift[i] + 1);
							PS2KeyStatus.bKeyNewFlag = TRUE;
							break;
						}
					}
				} 
				else 
				{	 				  
					for(i=0; i < 65; i++)
					{
						if(pgm_read_byte(PS2CodeShift[i]) == bKeyCode) 
						{
							PS2KeyStatus.bKeyAsiic = 
									pgm_read_byte(PS2CodeShift[i] + 1);
							PS2KeyStatus.bKeyNewFlag = TRUE;
							break;
						}
					}
				}
				break;
			}
		}
	} 
	else 
	{
		PS2KeyStatus.bBreakFlag = 0;
		switch (bKeyCode)		
		{
			case 0x12 :	   	
				PS2KeyStatus.bShiftFlag = 0;
				break;
			case 0x59 :		
				PS2KeyStatus.bShiftFlag = 0;
				break;
			default:
				break;
		}
	}
}	
/************************************************************************
*name:ISR(INT0_vect )													 
*function:Interrupt service routines		       									
*parameters: no					          									
*return: no		                           								
*************************************************************************/

ISR(INT7_vect )
{
	static char bOddParity = 0;
	static unsigned char bBitCounter = 0; //键盘帧位计数
	static unsigned char bKeyCode;
	bBitCounter++;
	if(bBitCounter == 1)//START位
	{
		if(PS2_DB_INPUT != 0)
		{
			bBitCounter = 0;
		}
		return;
	}
	else if(bBitCounter > 1 && bBitCounter < 10) //接收8位数据
	{
		bKeyCode >>= 1;
		if(PS2_DB_INPUT)
		{	
			bKeyCode |= 0x80;
			bOddParity++;
		}
		return;
	}
	else if(bBitCounter == 10)
	{
		if(bKeyCode ^ (bOddParity & 0x01)) //奇校验正确
		{
			PS2_KeyDeCode(bKeyCode); //解码
//			printf("key value: %x\r\n",bKeyCode );
		}
		else
		{
			bBitCounter = 0;
		}
		return;
	}
	else if(bBitCounter == 11) //11位数据接收完毕
	{
		bBitCounter = 0;
		bOddParity = 0;
		return;
	}

}
/************************************************************************
*name: unsigned char PS2_KeyScan(void)												 
*dunction: PS2 keypad scanning program	       									
*parameters: no					          									
*return: New press buttons ASIIC yards		                           					
			
*************************************************************************/	

unsigned char PS2_KeyScan(void)
{
	unsigned char temp;
	if(PS2KeyStatus.bKeyNewFlag == TRUE) 
	{
		PS2KeyStatus.bKeyNewFlag = FALSE; 
		if(PS2KeyStatus.bBreakFlag == FALSE)
		{
			temp = PS2KeyStatus.bKeyAsiic;
			PS2KeyStatus.bKeyAsiic = NULL;
			return temp;
		}
	}
	return NULL;
}
