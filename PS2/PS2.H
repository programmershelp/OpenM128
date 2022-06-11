/*********************************************************************************************************
*
* File                : PS2.h
* Hardware Environment: 
* Build Environment   : RealView MDK-ARM Version:4.14
* Version             : V1.0
* By                  : Xiao xian hui
*
*                                  (c) Copyright 2005-2010, WaveShare
*                                       http://www.waveShare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/

#ifndef _PS2_H_
#define _PS2_H_




void INT0_Init(void);
void PORT_Init(void);
unsigned char PS2_KeyScan(void);


#endif /*_PS2_H*/
