/*********************************************************************************************************
*
* @File                : TouchPanel.c
* @Hardware Environment: 
* @Build Environment   : RealView MDK-ARM  Version: 4.54
* @Version             : V1.0
* @date                : 201231221
* @Author              :jason
*
*                                  (c) Copyright 2005-2011, WaveShare
*                                       http://www.waveshare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/

#include "Touchpanel.h"
#include "LCD22.h"
#include"USART.H"

/* Private variables ---------------------------------------------------------*/
Matrix matrix ;

Coordinate  display ;


Coordinate ScreenSample[3];

Coordinate DisplaySample[3] = {
                                { 120,  40 },
								{ 40, 200},
                                { 200,270}
	                          };

/* Private define ------------------------------------------------------------*/
#define THRESHOLD 2


/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Writes to the selected LCD register.
* Input          : - Touch_Reg: address of the selected register.
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
__inline void Touch_WriteReg(uint16_t Touch_Reg)
{
	LCD_CS_H();
	LCD_RS_L();
	TOUCH_nCS_L();

	SPI0_communication((unsigned char)(Touch_Reg>>8));    //00000000 000000000
	SPI0_communication((unsigned char)(Touch_Reg));

	TOUCH_nCS_H();
	LCD_RS_H();
}

/*******************************************************************************
* Function Name  : Touch_WriteData
* Description    : 
* Input          : - data:
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
__inline void Touch_WriteData(uint16_t data)
{
    SPI0_communication((unsigned char)(data>>8));    //00000000 000000000
    SPI0_communication((unsigned char)(data));
}
/*******************************************************************************
* Function Name  : Touch_Write_Command
* Description    : 
* Input          : - Touch_Reg: address of the selected register.
*                  - Touch_RegValue: value to write to the selected register.
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
__inline void Touch_Write_Command(uint16_t Touch_Reg,uint16_t data)
{
	LCD_CS_H();
	LCD_RS_L();
	TOUCH_nCS_L();

	SPI0_communication((unsigned char)(Touch_Reg>>8));    //00000000 000000000
	SPI0_communication((unsigned char)(Touch_Reg));
	LCD_CS_H();
	//send data
	LCD_RS_H();

	TOUCH_nCS_L();
	SPI0_communication((unsigned char)(data>>8));    //00000000 000000000
	SPI0_communication((unsigned char)(data));
	TOUCH_nCS_H();
	
}

/******************************************************************************
* Function Name  : GetTouchADC
* Description    : Read XY AD Value
* Input          : data:
* Output         : None
* Return         : Touchpanel AD value
******************************************************************************/
uint32_t GetTouchADC (unsigned char data)
{	  
	uint32_t NUMH , NUML;
	uint32_t Num;
	
	LCD_CS_H()
	TOUCH_nCS_L();
	_delay_us(2);
	SPI0_communication(data);
	_delay_us(2);              
	NUMH=SPI0_communication(0x00);
	NUML=SPI0_communication(0x00);
	Num=((NUMH)<<8)+NUML; 	
	Num>>=4;                
	TOUCH_nCS_H();

	return(Num);   
}

/*******************************************************************************
* Function Name  : TP_GetAdXY
* Description    : Read ADS7843
* Input          : None
* Output         : None
* Return         : 
* Attention		 : None
*******************************************************************************/
void TP_GetAdXY(int *x,int *y)  
{  
	uint32_t adx,ady; 
  adx=GetTouchADC(CHX); 
//  delay_us(1); 
  ady=GetTouchADC(CHY); 
  *x=adx; 
  *y=ady; 	


} 

/*******************************************************************************
* Function Name  : TP_DrawPoint
* Description    : 
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void TP_DrawPoint(uint16_t Xpos,uint16_t Ypos)
{
  LCD_SetPoint(Xpos,Ypos,Black);     /* Center point */
  LCD_SetPoint(Xpos+1,Ypos,Black);
  LCD_SetPoint(Xpos,Ypos+1,Black);
  LCD_SetPoint(Xpos+1,Ypos+1,Black);	
	
}	

/*******************************************************************************
* Function Name  : DrawCross
* Description    : 
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void DrawCross(uint16_t Xpos,uint16_t Ypos,int color)
{
  LCD_DrawLine(Xpos-15,Ypos,Xpos-2,Ypos,color);
  LCD_DrawLine(Xpos+2,Ypos,Xpos+15,Ypos,color);
  LCD_DrawLine(Xpos,Ypos-15,Xpos,Ypos-2,color);
  LCD_DrawLine(Xpos,Ypos+2,Xpos,Ypos+15,color);
  
  LCD_DrawLine(Xpos-15,Ypos+15,Xpos-7,Ypos+15,color);
  LCD_DrawLine(Xpos-15,Ypos+7,Xpos-15,Ypos+15,color);

  LCD_DrawLine(Xpos-15,Ypos-15,Xpos-7,Ypos-15,color);
  LCD_DrawLine(Xpos-15,Ypos-7,Xpos-15,Ypos-15,color);

  LCD_DrawLine(Xpos+7,Ypos+15,Xpos+15,Ypos+15,color);
  LCD_DrawLine(Xpos+15,Ypos+7,Xpos+15,Ypos+15,color);

  LCD_DrawLine(Xpos+7,Ypos-15,Xpos+15,Ypos-15,color);
  LCD_DrawLine(Xpos+15,Ypos-15,Xpos+15,Ypos-7,color);	  	
}	
	
/*******************************************************************************
* Function Name  : Read_Ads7846
* Description    : Get TouchPanel X Y
* Input          : None
* Output         : None
* Return         : Coordinate *
* Attention		 : None
*******************************************************************************/
Coordinate *Read_Ads7846(void)
{
	uint32_t XAD; 
	XAD=32;
  static Coordinate  screen;
  int m0,m1,m2,TP_X[1],TP_Y[1],temp[3];
  uint8_t count=0;
  int buffer[2][9]={{0},{0}};
  
  do
  {		   
    TP_GetAdXY(TP_X,TP_Y);  
	buffer[0][count]=TP_X[0];  
	buffer[1][count]=TP_Y[0];
	count++;  
  }

  while(!TP_INT_IN&& count<9);  /* TP_INT_IN  */
  if(count==9)   /* Average X Y  */ 
  {
	/* Average X  */
    temp[0]=(buffer[0][0]+buffer[0][1]+buffer[0][2])/3;
	temp[1]=(buffer[0][3]+buffer[0][4]+buffer[0][5])/3;
	temp[2]=(buffer[0][6]+buffer[0][7]+buffer[0][8])/3;

	m0=temp[0]-temp[1];
	m1=temp[1]-temp[2];
	m2=temp[2]-temp[0];

	m0=m0>0?m0:(-m0);
  m1=m1>0?m1:(-m1);
	m2=m2>0?m2:(-m2);

	if( m0>THRESHOLD  &&  m1>THRESHOLD  &&  m2>THRESHOLD ) return 0;

	if(m0<m1)
	{
	  if(m2<m0) 
	    screen.x=(temp[0]+temp[2])/2;
	  else 
	    screen.x=(temp[0]+temp[1])/2;	
	}
	else if(m2<m1) 
	  screen.x=(temp[0]+temp[2])/2;
	else 
	  screen.x=(temp[1]+temp[2])/2;

	/* Average Y  */
  temp[0]=(buffer[1][0]+buffer[1][1]+buffer[1][2])/3;
	temp[1]=(buffer[1][3]+buffer[1][4]+buffer[1][5])/3;
	temp[2]=(buffer[1][6]+buffer[1][7]+buffer[1][8])/3;
	m0=temp[0]-temp[1];
	m1=temp[1]-temp[2];
	m2=temp[2]-temp[0];
	m0=m0>0?m0:(-m0);
	m1=m1>0?m1:(-m1);
	m2=m2>0?m2:(-m2);
	if(m0>THRESHOLD&&m1>THRESHOLD&&m2>THRESHOLD) return 0;

	if(m0<m1)
	{
	  if(m2<m0) 
	    screen.y=(temp[0]+temp[2])/2;
	  else 
	    screen.y=(temp[0]+temp[1])/2;	
    }
	else if(m2<m1) 
	   screen.y=(temp[0]+temp[2])/2;
	else
	   screen.y=(temp[1]+temp[2])/2;



	/*x坐标*/
	/*
	PutChar(XAD,16,48+screen.x/1000,Black,Grey); 			XAD=XAD+8;
	PutChar(XAD,16,48+screen.x%1000/100,Black,Grey);  XAD=XAD+8; 
	PutChar(XAD,16,48+screen.x%100/10,Black,Grey); 	  XAD=XAD+8;
	PutChar(XAD,16,48+screen.x%10,Black,Grey);     		XAD=XAD+8;
	*/
	///*y坐标*/
	/*
	XAD=32;
	PutChar(XAD,32,48+screen.y/1000,Black,Grey);   		 XAD=XAD+8;
	PutChar(XAD,32,48+screen.y%1000/100,Black,Grey);   XAD=XAD+8;
	PutChar(XAD,32,48+screen.y%100/10,Black,Grey);     XAD=XAD+8;
	PutChar(XAD,32,48+screen.y%10,Black,Grey);         XAD=XAD+8;
*/

	return &screen;
  }  
  return 0; 
}
	 

/*******************************************************************************
* Function Name  : setCalibrationMatrix
* Description    : Calculate K A B C D E F
* Input          : None
* Output         : None
* Return         : 
* Attention		 : None
*******************************************************************************/
FunctionalState setCalibrationMatrix( Coordinate * displayPtr,
                          Coordinate * screenPtr,
                          Matrix * matrixPtr)
{

  FunctionalState retTHRESHOLD = ENABLE ;
  /* K＝(X0－X2) (Y1－Y2)－(X1－X2) (Y0－Y2) */
  matrixPtr->Divider = ((screenPtr[0].x - screenPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) - 
                       ((screenPtr[1].x - screenPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
  if( matrixPtr->Divider == 0 )
  {
    retTHRESHOLD = DISABLE;
  }
  else
  {
    /* A＝((XD0－XD2) (Y1－Y2)－(XD1－XD2) (Y0－Y2))／K	*/
    matrixPtr->An = ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) - 
                    ((displayPtr[1].x - displayPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
	/* B＝((X0－X2) (XD1－XD2)－(XD0－XD2) (X1－X2))／K	*/
    matrixPtr->Bn = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].x - displayPtr[2].x)) - 
                    ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].x - screenPtr[2].x)) ;
    /* C＝(Y0(X2XD1－X1XD2)+Y1(X0XD2－X2XD0)+Y2(X1XD0－X0XD1))／K */
    matrixPtr->Cn = (screenPtr[2].x * displayPtr[1].x - screenPtr[1].x * displayPtr[2].x) * screenPtr[0].y +
                    (screenPtr[0].x * displayPtr[2].x - screenPtr[2].x * displayPtr[0].x) * screenPtr[1].y +
                    (screenPtr[1].x * displayPtr[0].x - screenPtr[0].x * displayPtr[1].x) * screenPtr[2].y ;
    /* D＝((YD0－YD2) (Y1－Y2)－(YD1－YD2) (Y0－Y2))／K	*/
    matrixPtr->Dn = ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].y - screenPtr[2].y)) - 
                    ((displayPtr[1].y - displayPtr[2].y) * (screenPtr[0].y - screenPtr[2].y)) ;
    /* E＝((X0－X2) (YD1－YD2)－(YD0－YD2) (X1－X2))／K	*/
    matrixPtr->En = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].y - displayPtr[2].y)) - 
                    ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].x - screenPtr[2].x)) ;
    /* F＝(Y0(X2YD1－X1YD2)+Y1(X0YD2－X2YD0)+Y2(X1YD0－X0YD1))／K */
    matrixPtr->Fn = (screenPtr[2].x * displayPtr[1].y - screenPtr[1].x * displayPtr[2].y) * screenPtr[0].y +
                    (screenPtr[0].x * displayPtr[2].y - screenPtr[2].x * displayPtr[0].y) * screenPtr[1].y +
                    (screenPtr[1].x * displayPtr[0].y - screenPtr[0].x * displayPtr[1].y) * screenPtr[2].y ;


//	printf("matrixPtr->An=%ld\r\n",matrixPtr->An);
//	printf("matrixPtr->Bn=%ld\r\n",matrixPtr->Bn);
//	printf(" matrixPtr->Cn=%ld\r\n", matrixPtr->Cn);
//	printf("matrixPtr->Dn=%ld\r\n",matrixPtr->Dn);
//	printf("matrixPtr->En=%ld\r\n",matrixPtr->En);
//	printf("matrixPtr->Fn=%ld\r\n",matrixPtr->Fn);
  }
  return( retTHRESHOLD ) ;
}

/*******************************************************************************
* Function Name  : getDisplayPoint
* Description    : Touch panel X Y to display X Y
* Input          : None
* Output         : None
* Return         : 
* Attention		 : None
*******************************************************************************/
FunctionalState getDisplayPoint(Coordinate * displayPtr,
                     Coordinate * screenPtr,
                     Matrix * matrixPtr )
{
  FunctionalState retTHRESHOLD =ENABLE ;

  if( matrixPtr->Divider != 0 )
  {
    /* XD = AX+BY+C */        
    displayPtr->x = ( (matrixPtr->An * screenPtr->x) + 
                      (matrixPtr->Bn * screenPtr->y) + 
                       matrixPtr->Cn 
                    ) / matrixPtr->Divider ;

	/* YD = DX+EY+F */        
    displayPtr->y = ( (matrixPtr->Dn * screenPtr->x) + 
                      (matrixPtr->En * screenPtr->y) + 
                       matrixPtr->Fn 
                    ) / matrixPtr->Divider ;


printf("displayPtr->x=%ld\r\n",( (matrixPtr->Dn * screenPtr->x) + 
                      (matrixPtr->En * screenPtr->y) + 
                       matrixPtr->Fn 
                    ));
printf("displayPtr>y=%ld\r\n",( (matrixPtr->Dn * screenPtr->x) + 
                      (matrixPtr->En * screenPtr->y) + 
                       matrixPtr->Fn 
                    ));

  }
  else
  {
    retTHRESHOLD = DISABLE;
  }
  return(retTHRESHOLD);
} 

/*******************************************************************************
* Function Name  : TouchPanel_Calibrate
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void TouchPanel_Calibrate(uint16_t charColor, uint16_t bkColor )
{
  uint8_t i;
  Coordinate * Ptr;
	LCD_Clear(bkColor);
  for(i=0;i<3;i++)
  {	 
   GUI_Text(10,100,"Touch crosshair to calibrate",charColor,bkColor);
   _delay_ms(300);
   DrawCross(DisplaySample[i].x,DisplaySample[i].y,charColor);
   do
   {
     Ptr=Read_Ads7846();
   }
   while( Ptr == (void*)0 );
   ScreenSample[i].x= Ptr->x; ScreenSample[i].y= Ptr->y;
	 
	DrawCross(DisplaySample[i].x,DisplaySample[i].y,bkColor);
  }
  setCalibrationMatrix( &DisplaySample[0],&ScreenSample[0],&matrix );
	GUI_Text(10,100,"Touch crosshair to calibrate",bkColor,bkColor);
} 

/*******************************************************************************
* Function Name  : TouchPanel_Calibrate
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void TouchPanel_Calibrate2(void)
{	
  ScreenSample[0].x = 2418; ScreenSample[0].y= 3024;
  ScreenSample[1].x = 3264; ScreenSample[1].y= 2496;
  ScreenSample[2].x = 3688; ScreenSample[2].y= 3664;   	
  setCalibrationMatrix( &DisplaySample[0],&ScreenSample[0],&matrix );

} 

/*******************************************************************************
* Function Name  : TouchPanel_Calibrate
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Touch_Debug(void)
{
	uint32_t XAD,YAD;
	 
	XAD=32;
	YAD=62;
//	GUI_Text(0,YAD,"Coordinate:",Red,Grey);
//	GUI_Text(0,YAD,"XAD=",Red,Grey);
//	GUI_Text(0,YAD,"yAD=",Red,Grey);
/* */
	//x坐标
	PutChar(XAD,YAD,48+display.x/1000,Black,Grey); 			XAD=XAD+8;
	PutChar(XAD,YAD,48+display.x%1000/100,Black,Grey);  XAD=XAD+8; 
	PutChar(XAD,YAD,48+display.x%100/10,Black,Grey); 	  XAD=XAD+8;
	PutChar(XAD,YAD,48+display.x%10,Black,Grey);     		XAD=XAD+8;
	//y坐标
	XAD=32;
	PutChar(XAD,YAD+16,48+display.y/1000,Black,Grey);   		 XAD=XAD+8;
	PutChar(XAD,YAD+16,48+display.y%1000/100,Black,Grey);   XAD=XAD+8;
	PutChar(XAD,YAD+16,48+display.y%100/10,Black,Grey);     XAD=XAD+8;
	PutChar(XAD,YAD+16,48+display.y%10,Black,Grey);         XAD=XAD+8;
	/**/

}
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

