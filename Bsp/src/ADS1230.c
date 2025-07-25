#include "sys_type.h"
#include "n32l40x.h"
#include "bsp_io.h"
#include "bsp_timer.h"
#include "bsp_wdg.h"
#include "ADS1230.h"

//static void Dout1_Dir(UCHAR dir)
//{
//    GPIO_InitType GPIO_InitStructure;    
//	GPIO_InitStruct(&GPIO_InitStructure);
//	if(dir)
//	{
//		GPIO_InitStructure.Pin		  = GPIO_PIN_15;
//		GPIO_InitStructure.GPIO_Pull	= GPIO_No_Pull;
//		GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
//		GPIO_InitPeripheral(GPIOD, &GPIO_InitStructure);
//	}
//	else
//	{
//		GPIO_InitStructure.Pin		  = GPIO_PIN_15;
//		GPIO_InitStructure.GPIO_Pull	= GPIO_No_Pull;
//		GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Input;
//		GPIO_InitPeripheral(GPIOD, &GPIO_InitStructure);
//	}
//}

//static void Dout2_Dir(UCHAR dir)
//{
//    GPIO_InitType GPIO_InitStructure;    
//	GPIO_InitStruct(&GPIO_InitStructure);
//	if(dir)
//	{
//		GPIO_InitStructure.Pin		  = GPIO_PIN_12;
//		GPIO_InitStructure.GPIO_Pull	= GPIO_No_Pull;
//		GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
//		GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
//	}
//	else
//	{
//		GPIO_InitStructure.Pin		  = GPIO_PIN_12;
//		GPIO_InitStructure.GPIO_Pull	= GPIO_No_Pull;
//		GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Input;
//		GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
//	}
//}


//static void vdelay(USHORT dly)
//{
//	while(dly--);
//}

void vADS1230_Init(void)
{
    GPIO_InitType GPIO_InitStructure;    

    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE);
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOD, ENABLE);

	GPIO_InitStruct(&GPIO_InitStructure);

	//ADS1230_1
	GPIO_InitStructure.Pin		  = GPIO_PIN_1;
	GPIO_InitStructure.GPIO_Pull	= GPIO_No_Pull;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
	GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.Pin		  = GPIO_PIN_15;
	GPIO_InitStructure.GPIO_Pull	= GPIO_No_Pull;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
	GPIO_InitPeripheral(GPIOD, &GPIO_InitStructure);
	GPIO_InitStructure.Pin		  = GPIO_PIN_14;
	GPIO_InitStructure.GPIO_Pull	= GPIO_No_Pull;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Input;
	GPIO_InitPeripheral(GPIOD, &GPIO_InitStructure);
	
	//ADS1230_2
	GPIO_InitStructure.Pin		  = GPIO_PIN_14;
	GPIO_InitStructure.GPIO_Pull	= GPIO_No_Pull;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
	GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.Pin		  = GPIO_PIN_13;
	GPIO_InitStructure.GPIO_Pull	= GPIO_No_Pull;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
	GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.Pin		  = GPIO_PIN_12;
	GPIO_InitStructure.GPIO_Pull	= GPIO_No_Pull;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Input;
	GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);


	ADS1230_SCLK1(LOW);
	ADS1230_SCLK2(LOW);
	POWER_DOWN_1(LOW);//AD OFF
	POWER_DOWN_2(LOW);
	vBSP_DelayMs(10);
	POWER_DOWN_1(HIGH);//AD ON
	POWER_DOWN_2(HIGH);

}

void vADS1230_PowerDown(void)
{
	//POWER_DOWN_1(LOW);//AD OFF
	//POWER_DOWN_2(LOW);
	
	ADS1230_SCLK1(HIGH);
	ADS1230_SCLK2(HIGH);

	
}

static void vdelay(USHORT dly)
{
	dly=dly*5;
	while(dly--);
}

void vADS1230_Offset1(void)
{
    while(!ADS1230_DOUT1_LOW());  //等待DATA拉低
    for(uint8_t i = 0;i < 26;i ++)            //发�?6个CLK
    {
        ADS1230_SCLK1(HIGH);
		vdelay(2);
        ADS1230_SCLK1(LOW);
		vdelay(2);
    }   
}

void vADS1230_Offset2(void)
{
    while(!ADS1230_DOUT2_LOW());  //等待DATA拉低
    for(uint8_t i = 0;i < 26;i ++)            //发�?6个CLK
    {
        ADS1230_SCLK2(HIGH);
		vdelay(2);
        ADS1230_SCLK2(LOW);
		vdelay(2);
    }   
}
LONG vADS1230_Read1(void)
{
	uint32_t _raw = 0;

	vBSP_WDG_Refresh();

	vTIM_1msStart(2000);
	while(!vTIM_1msIsTimeout())
	{
		if(ADS1230_DOUT1_LOW())
		{
			vBSP_WDG_Refresh();
			//vdelay(1);
			for(uint8_t i=0; i < 20; i++)//read drdy pin within 20st
			{
				ADS1230_SCLK1(1);
				vdelay(1);
				_raw <<= 1;
				_raw |= ADS1230_DOUT1_STATUS();
				vdelay(1);
				ADS1230_SCLK1(0);
				vdelay(2);
			}
			for(uint8_t i =0 ;i < 4;i++)//to avoid drdy pin with keep the 20st sclk status.
			{
				ADS1230_SCLK1(1);
				vdelay(2);
				ADS1230_SCLK1(0);
				vdelay(2);
			}
			if(_raw & 0x80000)
			{
				_raw = ~_raw;
				_raw = -1*(_raw & 0x7FFFF);
			}
			break;
		}

		//*ad_val = _raw;
	}
	

	
	return _raw;

}

LONG vADS1230_Read2(void)
{
	uint32_t _raw = 0;

	vBSP_WDG_Refresh();
	vTIM_1msStart(2000);
	while(!vTIM_1msIsTimeout())
	{
		if(ADS1230_DOUT2_LOW())
		{
			vBSP_WDG_Refresh();
			//vdelay(1);
			for(uint8_t i=0; i < 20; i++)//read drdy pin within 20st
			{
				ADS1230_SCLK2(1);
				vdelay(1);
				_raw <<= 1;
				_raw |= ADS1230_DOUT2_STATUS();
				vdelay(1);
				ADS1230_SCLK2(0);
				vdelay(2);
			}
			for(uint8_t i =0 ;i < 4;i++)//to avoid drdy pin with keep the 20st sclk status.
			{
				ADS1230_SCLK2(1);
				vdelay(2);
				ADS1230_SCLK2(0);
				vdelay(2);
			}
			if(_raw & 0x80000)
			{
				_raw = ~_raw;
				_raw = -1*(_raw & 0x7FFFF);
			}
			break;
		}

	}
	//*ad_val = _raw;
	return _raw;

}

