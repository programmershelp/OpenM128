#include<avr/io.h>
#include<avr/interrupt.h>
#define uint unsigned int
#define uchar unsigned char 


void init(void)
{
	DDRA=0XFF;
}


int main()
{
	unsigned long i; 
	init();
//	TCNT0=(256-200);
	TCCR0=(1<<CS02)|(1<<CS01)|(1<<CS00);//1024 YuFen frequency
//	TCCR0 = (1<<CS00);       //1 frequency
//	TCCR0=(1<<CS02)|(1<<CS00);//128 YuFen frequency
	while(1)
	{
		while(!(TIFR&(1<<TOV0)));
		TIFR|=0X01;

		TCNT0=(256-200);
		i++;
		if(i==50)
		{
			i=0;
			PORTA=~PORTA;
		}
	}
}
