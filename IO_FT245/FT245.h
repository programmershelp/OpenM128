#ifndef _FT245_H_
#define _FT245_H_

#include <avr/io.h>


extern const char num[ ];

typedef unsigned char	uint8;
typedef unsigned int	uint16;
typedef signed  char	sint8;
typedef signed	int		sint16;


#define sbi(io,bit)		(  io |=  (1<<bit) )	
#define cbi(io,bit)		(  io &= ~(1<<bit) )

#define OUT_WR()  sbi(DDRA,6)
#define OUT_RD()  sbi(DDRA,7)

#define DATA_DDR   DDRC
#define DATA_PORT  PORTC

#define WR_ON() cbi(PORTA,6)
#define WR_OFF() sbi(PORTA,6)

#define RD_ON() cbi(PORTA,7)
#define RD_OFF() sbi(PORTA,7)

uint8 a;

void delay50us(sint16 t);
void delay50ms(sint16 t);
void read_from_ft245();
void write_to_ft245();


#endif /* _FT245_H_ */
