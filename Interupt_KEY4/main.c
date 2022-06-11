#include<avr/io.h>
#include<avr/delay.h>
#include<avr/interrupt.h>

unsigned char value=0x55;



void Init_INT7(void);
void Init_Port(void);



void Init_Port(void)
{
	DDRA = 0XFF;
	PORTA = 0X00;
	DDRE |= (1<<PE7);
	PORTE |= (1 << PE7); //PC0 input take to pull

}
int main(void)
{
	Init_Port();

	EICRB |= (1 << ISC71); //INT0 down along the trigger

	EIMSK |= (1 << INT7); //That can INT0 interrupt
	sei();
	while(1);
}






/************************************************************************
*name:ISR(INT7_vect )													 
*function:Interrupt service routines		       									
*parameters: no					          									
*return: no		                           								
*************************************************************************/


ISR(INT7_vect)
{	
//	_delay_ms(100);
	PORTA++;	

}
