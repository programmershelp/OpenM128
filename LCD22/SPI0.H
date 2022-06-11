#ifndef __SPI0_H__
#define __SPI0_H__

#define uint unsigned int
#define uchar unsigned char


void SPI0_Init(void);

uchar SPI0_communication(uchar send_char);


#endif
