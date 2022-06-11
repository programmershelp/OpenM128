#include<avr/io.h>
#include<avr/delay.h>
#include<avr/interrupt.h>

#include"USART.H"

void ADC_Init(void);
unsigned int ad_getdata(void);
void key_scanf(unsigned int value);

int main(void)
{
	unsigned int AD_value;
	unsigned char i;
	DDRA=0XFF;
	PORTA=0X55;
	usartInit();
	ADC_Init();

	printf("*--------------------------------------------* \r\n");
	printf("www.waveshare.net \r\n");
	printf("ADC Test procedure \r\n");
	printf("*--------------------------------------------* \r\n");
	while(1)
	{


		for(i=0;i<200;i++)
		AD_value=ad_getdata();
		key_scanf(AD_value);
		_delay_ms(1000);

	}
}


void key_scanf(unsigned int value)
{
		if((value>=0)&&(value<=70))
		printf("You press the button is key0 \r\n");	

		if((value>=70)&&(value<=140))
		printf("You press the button is key1 \r\n");	

		if((value>=140)&&(value<=210))
		printf("You press the button is key2 \r\n");
			
		if((value>=210)&&(value<=260))
		printf("You press the button is key3 \r\n");
			
		if((value>=260)&&(value<=320))
		printf("You press the button is key4 \r\n");
			
		if((value>=320)&&(value<=380))
		printf("You press the button is key5 \r\n");
			

		if((value>=380)&&(value<=420))
		printf("You press the button is key6 \r\n");	

		if((value>=420)&&(value<=470))
		printf("You press the button is key7 \r\n");	

		if((value>=470)&&(value<=530))
		printf("You press the button is key8 \r\n");
			
		if((value>=530)&&(value<=600))
		printf("You press the button is key9 \r\n");
			
		if((value>=600)&&(value<=650))
		printf("You press the button is key10 \r\n");
			
		if((value>=650)&&(value<=700))
		printf("You press the button is key11 \r\n");

		if((value>=700)&&(value<=760))
		printf("You press the button is key12 \r\n");	

		if((value>=760)&&(value<=830))
		printf("You press the button is key13 \r\n");	

		if((value>=830)&&(value<=890))
		printf("You press the button is key14 \r\n");
			
		if((value>=890)&&(value<=940))
		printf("You press the button is key15 \r\n");
}


void ADC_Init(void)
{
    DDRA&=~(1<<PA0);      //PA0 Set to input, became great resistance, high resistance state
    SFIOR =0;              //Continuous conversion mode 
    ADMUX=0X40;            //AVCC£¬AREF Pins plus filter capacitance
	
}

unsigned int ad_getdata(void)
{ 
    unsigned int addata; 
    ADCSR =0X80;                    //Start ADC conversion 
    ADCSR |=0x40;           	    //Began to switch 
	while(!(ADCSR&0x10));			//Waiting for conversion
    addata=ADCL; 
    addata=addata+ADCH*256;        //The ADCL, ADCH converted into 10 into the system
	ADCSR |=0x10;                  //ADIF to write "1", ADIF qing 0
	ADCSR =0X00;                   //Close ADC conversion 
   return addata; 
}



