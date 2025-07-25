#include "delay.h"
#include <n32l40x_rcc.h>
static u8  fac_us = 16;

void Delay_InitConfig(void)
{
	// SysTick->CTRL &= ~(1<<2);	
	// fac_us = 16 / 8;		
}
								   
void delay_us(u32 nus)
{		
	u32 temp;	
	SysTick->LOAD = (u32)nus * 2;
	SysTick->VAL = 0x00;           
	SysTick->CTRL = 0x01;
	do {
		temp = SysTick->CTRL;
	}
	while((temp & 0x01) && !(temp & (1 << 16)));
 	SysTick->CTRL = 0x00;
	SysTick->VAL =0X00;     
}

void delay_ms(u16 nms)
{	 
	u32 temp;
	SysTick->LOAD = (u32)nms * 2 * 1000;
	SysTick->VAL = 0x00;           
	SysTick->CTRL = 0x01;
	do {
		temp = SysTick->CTRL;
	}
	while((temp & 0x01) && !(temp & (1 << 16)));
 	SysTick->CTRL = 0x00;
	SysTick->VAL =0X00;             
}


void delay_s(u16 s)
{	 
	u32 i;
	for(i=0;i<s;i++) delay_ms(1000);          
}





