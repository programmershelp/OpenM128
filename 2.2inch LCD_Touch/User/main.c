#include <avr/io.h>
#include "LCD22.h"
#include <util/delay.h>
#include "Touchpanel.h"
#include"USART.H"


/* Private variables ---------------------------------------------------------*/
extern Matrix matrix ;
 Coordinate  display ;
double Data;
Coordinate  *touch_dot ;

int main()
{	
	LCD_Initializtion();
  //DrawGrids();
  //TouchPanel_Calibrate(White,Red);
  //TouchPanel_Calibrate2();
	GUI_Text(0,0,"Touchpad:",White,Red);
	while (1)
	{		
	  getDisplayPoint(&display, Read_Ads7846(), &matrix ); 	
	  
	  
	touch_dot= Read_Ads7846();
	
	display.x=((touch_dot->x-80)/5.500); // �Ѷ�����X_ADCֵת����TFT X����ֵ(The read value is converted X_ADC TFT X coordinate value)
	//display.y=((touch_dot->y-115)/4.205); // �Ѷ�����Y_ADCֵת����TFT Y����ֵ//    touch_dot.x = (((touch_dot.x_ad_val * 240)>>12)-110)*2;
	display.y=(touch_dot->y)*(0.1448)-13.1644;	  // �Ѷ�����Y_ADCֵת����TFT Y����ֵ//
	  
	  	
 	TP_DrawPoint(display.y,display.x);
	   
	//TP_DrawPoint(222,222);
    //Touch_Debug();		
	}
	
}
