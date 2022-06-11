/********************************************************************************************************
*********************************************************************************************************
*
* File                : ws_macro.h
* Hardware Environment:	
* Build Environment   : AVR Studio 4.16 + Winavr 20090313
* Version             : V1.0
* By                  : Wu Ze
*
*                                  (c) Copyright 2005-2009, WaveShare
*                                       http://www.waveShare.net
*                                          All Rights Reserved
*
*********************************************************************************************************
********************************************************************************************************/

#ifndef _WS_MACRO_H_
#define _WS_MACRO_H_

#include <stdint.h>

#define sbi(io,bit)		(  io |=  (1<<bit) )	
#define cbi(io,bit)		(  io &= ~(1<<bit) )	
#define gbi(pin ,bit)	( pin &   (1<<bit) )


#endif /*_WS_MACRO_H_*/