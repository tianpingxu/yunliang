#ifndef __DELAY_H
#define __DELAY_H	 

#include "n32l40x.h"


void Delay_InitConfig(void);
void delay_us(u32 nus);
void delay_ms(u16 nms);
void delay_s(u16 s);

#endif 

