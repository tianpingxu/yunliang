#include "sys_type.h"
#include "n32l40x.h"
#include "bsp_io.h"
#include "bsp_usart.h"
#include "bsp_timer.h"
#include "bsp_adc.h"
#include "bsp_wdg.h"
#include "bsp_flash.h"
#include "app_io.h"
#include "app_system.h"
#include "app_user.h"
#include "app_flash.h"
#include "app_sample.h"
#include "ADS1230.h"

#include "Table_Refrigerant.h"

void SystemClock_Config(void);

void vSysStateCheckInNormal(void);
void vSysStateCheckInLowPower(void);
void vLowPowerSuspend(void);
void vLowPowerResume(void);
void vComLostCheck(void);
void vWakeUpLCD(void);

EN_SYS_STATUS u8_gSysState;
UCHAR u8_gWakeUpEvent;
SYS_PARAM st_gSysParam;
USHORT u16_gSystemFlag;
UCHAR u8_SysPowerDownCmd;
ULONG u32_gPowerDownTimeAdd;
ULONG u32_gLowPowerTimeAdd;
UCHAR u8_PowerDownEnable;
UCHAR u8_gLcdPowerOnEvent;
USHORT u16_gFaultStatus;




void vBSP_RtcWakeupEnable(UCHAR ucSeconds)
{
    NVIC_InitType NVIC_InitStructure;
	EXTI_InitType EXTI_InitStructure;

    if(ucSeconds == 0)
        return;

	NVIC_DisableIRQ(RTC_IRQn);
	RTC_ConfigInt(RTC_INT_WUT, DISABLE);
	RTC_EnableWakeUp(DISABLE);

    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_PWR, ENABLE);

	RTC_ConfigWakeUpClock(RTC_WKUPCLK_CK_SPRE_16BITS);    
	/* wake up timer value */
    RTC_SetWakeUpCounter(ucSeconds - 1);

	EXTI_ClrITPendBit(EXTI_LINE20);
	EXTI_InitStructure.EXTI_Line = EXTI_LINE20;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitPeripheral(&EXTI_InitStructure);

	RTC_ClrFlag(RTC_FLAG_WTF);
	/* Enable the RTC Wakeup Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd  = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	RTC_ConfigInt(RTC_INT_WUT, ENABLE);
	RTC_EnableWakeUp(ENABLE);
}

void vBSP_RtcWakeupDisable(void)
{
	NVIC_DisableIRQ(RTC_IRQn);
	RTC_ConfigInt(RTC_INT_WUT, DISABLE);
	RTC_EnableWakeUp(DISABLE);
}


void McuStop(void)
{
#ifdef DEBUG
	DBG_ConfigPeriph(DBG_STOP, ENABLE);
#endif
	SysTick_Deinit();
	PWR_EnterSTOP2Mode(PWR_STOPENTRY_WFI,PWR_CTRL3_RAM1RET | PWR_CTRL3_RAM2RET); 
	SysTick_Config(SystemCoreClock/1000);
}



void vBSP_PowerWakeLowPowerModeEnable(void)
{
    GPIO_InitType GPIO_InitStructure;    
    EXTI_InitType EXTI_InitStructure;
    NVIC_InitType NVIC_InitStructure;

	GPIO_InitStruct(&GPIO_InitStructure);
	/* IO INTERRUPT*/

	 /*PA8 SW*/
//	GPIO_InitStructure.Pin		  	= GPIO_PIN_8;
//	GPIO_InitStructure.GPIO_Pull	= GPIO_Pull_Up;
//	GPIO_InitStructure.GPIO_Mode  	= GPIO_Mode_Input;
//	GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
//    GPIO_ConfigEXTILine(GPIOA_PORT_SOURCE, GPIO_PIN_SOURCE8);
//    EXTI_InitStructure.EXTI_Line    = EXTI_LINE8;
//    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
//    EXTI_InitStructure.EXTI_Trigger	= EXTI_Trigger_Falling; 
//    EXTI_InitStructure.EXTI_LineCmd	= ENABLE;
//    EXTI_InitPeripheral(&EXTI_InitStructure);
//    EXTI_ClrITPendBit(EXTI_LINE8);
//    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);    


	/*PA11  plug */
//	GPIO_InitStructure.Pin 	  = GPIO_PIN_11;
//	GPIO_InitStructure.GPIO_Pull	  = GPIO_Pull_Up;
//	GPIO_InitStructure.GPIO_Mode	  = GPIO_Mode_Input;
//	GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
//	GPIO_ConfigEXTILine(GPIOA_PORT_SOURCE, GPIO_PIN_SOURCE11);
//	EXTI_InitStructure.EXTI_Line	  = EXTI_LINE11;
//	EXTI_InitStructure.EXTI_Mode	  = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_InitPeripheral(&EXTI_InitStructure);
//	EXTI_ClrITPendBit(EXTI_LINE11);
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure); 

	
	vBSP_RtcWakeupEnable(LOWPOWER_LOOP_PERIOD / 1000);
}

void vBSP_PowerWakeLowPowerModeDisable(void)
{
	vBSP_RtcWakeupDisable();
}

extern SHORT u16_soc0_th;
extern SHORT u16_soc1_th;
extern SHORT u16_soc2_th;
extern SHORT u16_soc3_th;
extern SHORT u16_soc4_th;
void vSoftInit(void)
{
	USHORT wk_bat_vol = 0;
	
	s16_gBatSocReport = 1000;
	s16_gBatSocCal = 1000;
	
	wk_bat_vol = usGetMCUADCVolt(ADC_CH_1_PA0);
	wk_bat_vol = (ULONG)wk_bat_vol * 3 / 2;
	s16_gBatVoltage = wk_bat_vol;
	
	if(s16_gBatVoltage >= u16_soc4_th)
	{
		s16_gBatSocCal  =1000;
	}
	else if(s16_gBatVoltage >= u16_soc3_th)
	{
		s16_gBatSocCal  =750;
	}
	else if(s16_gBatVoltage >= u16_soc2_th)
	{
		s16_gBatSocCal  =500;
	}
	else if(s16_gBatVoltage >= u16_soc1_th)
	{
		s16_gBatSocCal  = 250;
	}
	else if(s16_gBatVoltage >= u16_soc0_th)
	{
		s16_gBatSocCal  = 0;
	}
	s16_gBatSocReport = s16_gBatSocCal;
	
//	UCHAR wk_idx = 0;
//	UCHAR wk_data[50];
//	USHORT wk_init_flag;
//	
//	FLASH_If_ReadByte(ADDR_PARAM_ININT_FLAG, (UCHAR*)&wk_init_flag, 2 );
//	if(wk_init_flag != 0x5AA5)
//	{
//		wk_init_flag = 0x5AA5;
//		st_gSysParam.refrigerant_type = (UCHAR)R_410A;
//		st_gSysParam.pressure_unit = UNIT_PRESSURE_PSI;
//		st_gSysParam.temp_unit = UNIT_TEMP_F;
//		st_gSysParam.shutdown_time = TIME_SHUTDOWN_1H;
//		st_gSysParam.lowpower_time = TIME_LOWPOWER_10MIN;
//		memset(&st_gPressureCaliParam,0,sizeof(ST_PRESSURE_CALI));
//		ucFlashWriteData(ADDR_REFIR_TYPE_SELECT,(UCHAR*)&st_gSysParam,sizeof(st_gSysParam));
//		ucFlashWriteData(ADDR_HIGHT_PRESSURE_PARAM,(UCHAR*)&st_gPressureCaliParam,sizeof(ST_PRESSURE_CALI));
//		ucFlashWriteData(ADDR_PARAM_ININT_FLAG,(UCHAR*)&wk_init_flag,sizeof(wk_init_flag));
//	}
//	else
//	{
//		FLASH_If_ReadByte(ADDR_START, wk_data, 30 );
//		st_gSysParam.refrigerant_type = wk_data[wk_idx++];
//		st_gSysParam.pressure_unit = wk_data[wk_idx++];
//		st_gSysParam.temp_unit = wk_data[wk_idx++];
//		st_gSysParam.shutdown_time = wk_data[wk_idx++];
//		st_gSysParam.lowpower_time = wk_data[wk_idx++];
//		memcpy(&st_gPressureCaliParam,&wk_data[wk_idx],sizeof(ST_PRESSURE_CALI));
//	}

//	u8_SysPowerDownCmd = FALSE;
//	ucTIM_50msFlg = TRUE;
//	u32_gPowerDownTimeAdd = 0;
//	u32_gLowPowerTimeAdd = 0;
//	u8_PowerDownEnable = TRUE;
}

void vSetSysState(UCHAR st)
{
	u8_gSysState = (EN_SYS_STATUS)st;
}

void vPowerOnEventCheck(void)
{
	ucTIM_50msFlg = TRUE;

	if(P_PLUG_IS_INSERT() == TRUE)
	{
		vBSP_DelayMs(20);
		if(P_PLUG_IS_INSERT() == TRUE)
		{
			ucPlugInsertStatus = STATUS_PLUG_INSERT;
			u8_gLcdPowerOnEvent = EVENT_PLUG_POWER_ON;
			vSetSysState(SYS_STATE_CHARG_IN_STOP);
			return;
		}
	}

	if(P_PWR_KEY_IS_PRESS() == TRUE)
	{
		vBSP_DelayMs(20);
		if(P_PWR_KEY_IS_PRESS() == TRUE)
		{
			ucPlugInsertStatus = STATUS_PLUG_REMOVE;
			u8_gLcdPowerOnEvent = EVENT_KEY_POWER_ON;
			vSetSysState(SYS_STATE_KEY_POWER_ON);
			return;
		}
	}
}


void vStateKeyPowerOn(void)
{
	vBatteryCalSoc(&s16_gBatSocReport,&u8_gBatSocLevel);
	vSampleTemp();
	//正常按键开机
	if(vSysPowerOn() == TRUE)
	{
		st_gKey.KeyState = KEY_STATE_NULL;
		vSetSysState(SYS_STATE_INIT);
		ucTIM_50msFlg = TRUE;
	}
	
}

void vStateChargeInStop(void)
{
	static ULONG wk_charge_time = 0;

	P_LCD_POWER_CTRL(ON);	

	vBatteryCalSoc(&s16_gBatSocReport,&u8_gBatSocLevel);
	vSampleTemp();

	//充电器插入进入关机充电界面，长按按键开机。
	if(vSysPowerOn() == TRUE)
	{
		wk_charge_time = 0;
		st_gKey.KeyState = KEY_STATE_NULL;
		u8_gLcdPowerOnEvent = EVENT_KEY_POWER_ON;
		P_LCD_POWER_CTRL(OFF);	
		vBSP_DelayMs(200);
		P_LCD_POWER_CTRL(ON);	
		vSetSysState(SYS_STATE_INIT);
		ucTIM_50msFlg = TRUE;
	}

	wk_charge_time+=50;
	if(wk_charge_time > 300000)
	{
		vUserInterfaceEnterLowpower(0xAA);
		wk_charge_time = 0;
		st_gUsartLCD1.usRecvIdx = 0;
		st_gUsartLCD1.RevComplete = FALSE;
		st_gUsartLCD2.usRecvIdx = 0;
		st_gUsartLCD2.RevComplete = FALSE;
		vBSP_USARTDeInit(USART_LCD1);
		vBSP_USARTDeInit(USART_LCD2);
		vSetSysState(SYS_STATE_CHARG_IN_LOWPOWER);
	}
	
	//拔出充电器关机
	if(ucPlugInsertStatus == STATUS_PLUG_REMOVE)
	{
		wk_charge_time = 0;
		vSetSysState(SYS_STATE_STOP);
		ucTIM_50msFlg = TRUE;
	}
	
}


void vStateChargeInLowpower(void)
{
	UCHAR wk_charge_complete;
	static UCHAR wk_pre_charge_complete = TRUE;
	
	//P_LCD_POWER_CTRL(OFF);

	//vAPI_IoStatus();
	
	vBatteryCalSoc(&s16_gBatSocReport,&u8_gBatSocLevel);

	wk_charge_complete = (IN_CHARGING_COMPLETE())?TRUE:FALSE;
	if(((wk_charge_complete == TRUE)&&(wk_pre_charge_complete == FALSE))|| 
		(ucSwStatus == TRUE)/* || (ucEncodeStatus == TRUE)*/)
	{
		u8_gLcdPowerOnEvent = EVENT_PLUG_WAKE_UP;
		vBSP_USARTInit(USART_LCD1);
		vBSP_USARTInit(USART_LCD2);
		vWakeUpLCD();
		vSetSysState(SYS_STATE_CHARG_IN_STOP);
		ucTIM_50msFlg = TRUE;
	}
	wk_pre_charge_complete = wk_charge_complete;


	//拔出充电器关机
	if(ucPlugInsertStatus == STATUS_PLUG_REMOVE)
	{
		vSetSysState(SYS_STATE_STOP);
		ucTIM_50msFlg = TRUE;
	}
}


void vStateInit(void)
{
	UCHAR wk_idx = 0;
	UCHAR wk_data[50];
	USHORT wk_init_flag;

//	vBatteryCalSoc(&s16_gBatSocReport,&u8_gBatSocLevel);
//	if(vSysPowerOn() == FALSE)
//	{
//		return;
//	}
//	else
//	{
//		if(u8_gLcdPowerOnEvent == EVENT_PLUG_POWER_ON)
//		{
//			vSetSysState(SYS_STATE_CHARG_IN_STOP);
//			ucTIM_50msFlg = TRUE;
//			return;
//		}
//	}

	FLASH_If_ReadByte(ADDR_PARAM_ININT_FLAG, (UCHAR*)&wk_init_flag, 2 );
	if(wk_init_flag != 0x5AA5)
	{
		wk_init_flag = 0x5AA5;
		st_gSysParam.refrigerant_type = (UCHAR)R_410A;
		st_gSysParam.pressure_unit = UNIT_PRESSURE_PSI;
		st_gSysParam.temp_unit = UNIT_TEMP_F;
		st_gSysParam.shutdown_time = TIME_SHUTDOWN_1H;
		st_gSysParam.lowpower_time = TIME_LOWPOWER_10MIN;
		memset(&st_gPressureCaliParam,0,sizeof(ST_PRESSURE_CALI));
		ucFlashWriteData(ADDR_REFIR_TYPE_SELECT,(UCHAR*)&st_gSysParam,sizeof(st_gSysParam));
		ucFlashWriteData(ADDR_HIGHT_PRESSURE_PARAM,(UCHAR*)&st_gPressureCaliParam,sizeof(ST_PRESSURE_CALI));
		ucFlashWriteData(ADDR_PARAM_ININT_FLAG,(UCHAR*)&wk_init_flag,sizeof(wk_init_flag));
	}
	else
	{
		FLASH_If_ReadByte(ADDR_START, wk_data, 30 );
		st_gSysParam.refrigerant_type = wk_data[wk_idx++];
		st_gSysParam.pressure_unit = wk_data[wk_idx++];
		st_gSysParam.temp_unit = wk_data[wk_idx++];
		st_gSysParam.shutdown_time = wk_data[wk_idx++];
		st_gSysParam.lowpower_time = wk_data[wk_idx++];
		memcpy(&st_gPressureCaliParam,&wk_data[wk_idx],sizeof(ST_PRESSURE_CALI));
	}

	u8_SysPowerDownCmd = FALSE;
	ucTIM_50msFlg = TRUE;
	u32_gPowerDownTimeAdd = 0;
	u32_gLowPowerTimeAdd = 0;
	u8_PowerDownEnable = TRUE;

	vSetSysState(SYS_STATE_NORMAL);

}

//int CNT = 0;
//LONG g_adc_high=888,g_adc_low=666;
//float pressure_high=0.0,pressure_low=0.0;

void vStateNormal(void)
{	
	vBatteryCalSoc(&s16_gBatSocReport,&u8_gBatSocLevel);
	vSampleTemp();

	vComLostCheck();
	vSysStateCheckInNormal();
}



void vStateLowpower(void)
{
	//P_LCD_POWER_CTRL(OFF);
	//vAPI_IoStatus();
	
	while(1)
	{
		vBSP_WDG_Refresh();
		u8_gWakeUpEvent = 0;
		for(int idx = 0;idx <ADC_BUFF_MAX;idx++)
		{
			vSamplePressureTemp();
			vSamplePressureTemp();
		}
		//printf("low:%ld\r\nhigh:%ld\r\n",g_adc_low,g_adc_high );
		vSysStateCheckInLowPower();
		if(SYS_STATE_LOWPOWER != u8_gSysState)
		{
			break;
		}
	}
}


void vStateStop(void)
{
	while(1)
	{
		P_LCD_POWER_CTRL(OFF);
		P_SYSTEM_POWER_CTRL(OFF);
	}
}


void vSysStateCheckInNormal(void)
{
	static UCHAR wk_pre_key_status = FALSE;
	UCHAR wk_key_status = FALSE;
	static UCHAR wk_pre_encode_status = FALSE;
	UCHAR wk_encode_status = FALSE;
	UCHAR wk_low_power_time_min[4] = {0,2,5,10};
	static UCHAR wk_0bar_cnt1 = 0;
	static UCHAR wk_0bar_cnt2 = 0;


	u32_gLowPowerTimeAdd = (st_gSysParam.lowpower_time != TIME_LOWPOWER_OFF)?u32_gLowPowerTimeAdd + 50:0;
	u32_gPowerDownTimeAdd = ((st_gSysParam.shutdown_time != TIME_SHUTDOWN_OFF) && (u8_PowerDownEnable == TRUE))?u32_gPowerDownTimeAdd + 50:0;
	
	wk_key_status = ucSwStatus;//P_PWR_KEY_IS_PRESS();
	wk_encode_status = ucEncodeStatus;
	if(((wk_pre_key_status == FALSE) && (wk_key_status == TRUE)) || 
		((wk_pre_encode_status == FALSE) && (wk_encode_status == TRUE)) ||
		(ucPlugInsertStatus == STATUS_PLUG_INSERT)) //||
		//(st_gHL_Pressure[0].p_0001xbar_report != 0) || 
		//(st_gHL_Pressure[1].p_0001xbar_report != 0))
	{
		u32_gLowPowerTimeAdd = 0;
		u32_gPowerDownTimeAdd = 0;
	}
	
	if(st_gHL_Pressure[0].p_0001xbar_report != 0)
	{
		if(++wk_0bar_cnt1 >= 8)
		{
			wk_0bar_cnt1 = 0;
			u32_gLowPowerTimeAdd = 0;
			u32_gPowerDownTimeAdd = 0;
		}
	}
	else
	{
		wk_0bar_cnt1 = 0;
	}
	
	if(st_gHL_Pressure[1].p_0001xbar_report != 0)
	{
		if(++wk_0bar_cnt2 >= 8)
		{
			wk_0bar_cnt2 = 0;
			u32_gLowPowerTimeAdd = 0;
			u32_gPowerDownTimeAdd = 0;
		}
	}
	else
	{
		wk_0bar_cnt2 = 0;
	}
	
	
	
	wk_pre_key_status = wk_key_status;
	wk_pre_encode_status = wk_encode_status;


	if((u8_SysPowerDownCmd == TRUE) ||
		((u16_gFaultStatus & FAULT_LOW_VOL) && (ucPlugInsertStatus == STATUS_PLUG_REMOVE)) ||
		(u32_gPowerDownTimeAdd > (ULONG)st_gSysParam.shutdown_time * 3600000))
	{
		st_gKey.KeyState = KEY_STATE_NULL;
		u32_gPowerDownTimeAdd = 0;
		u32_gLowPowerTimeAdd = 0;
		u8_SysPowerDownCmd = FALSE;
		vSetSysState(SYS_STATE_STOP);
		return;
	}

	
	if((st_gKey.KeyState == KEY_STATE_LONG)&&(u8_PowerDownEnable == TRUE))
	{
		st_gKey.KeyState = KEY_STATE_NULL;
		if(ucPlugInsertStatus == STATUS_PLUG_INSERT)	//长按关机时，如果充电器在，就切到关机充电界面
		{
			u8_gLcdPowerOnEvent = EVENT_PLUG_WAKE_UP;
			P_LCD_POWER_CTRL(OFF);	
			vBSP_DelayMs(200);
			P_LCD_POWER_CTRL(ON);	
			vSetSysState(SYS_STATE_CHARG_IN_STOP);
		}
		else
		{
			u32_gPowerDownTimeAdd = 0;
			u32_gLowPowerTimeAdd = 0;
			u8_SysPowerDownCmd = FALSE;
			vSetSysState(SYS_STATE_STOP);
			return;
		}
	}

	if(u32_gLowPowerTimeAdd > wk_low_power_time_min[st_gSysParam.lowpower_time] * 60000)
	{
		vUserInterfaceEnterLowpower(0xBB);
		u32_gLowPowerTimeAdd = 0;
		
		st_gUsartLCD1.usRecvIdx = 0;
		st_gUsartLCD1.RevComplete = FALSE;
		st_gUsartLCD2.usRecvIdx = 0;
		st_gUsartLCD2.RevComplete = FALSE;
		vBSP_USARTDeInit(USART_LCD1);
		vBSP_USARTDeInit(USART_LCD2);
		
		vSetSysState(SYS_STATE_LOWPOWER);
		return;
	}
}

void vSysStateCheckInLowPower(void)
{
	static UCHAR wk_0bar_cnt1 = 0;
	static UCHAR wk_0bar_cnt2 = 0;

	vBSP_WDG_Refresh();
	vLowPowerSuspend();
	vBSP_PowerWakeLowPowerModeEnable();

	if(u32_gPowerDownTimeAdd > (ULONG)st_gSysParam.shutdown_time * 3600000)
	{
		u32_gLowPowerTimeAdd = 0;
		u32_gPowerDownTimeAdd = 0;
		u8_gWakeUpEvent = 0;
		vLowPowerResume();
		vSetSysState(SYS_STATE_STOP);
		return;
	}

	if((u8_gWakeUpEvent & (WAKE_INTERRUPT_SW | WAKE_INTERRUPT_PLUG)) //|| 
	/*(st_gHL_Pressure[0].p_0001xbar_report != 0) || (st_gHL_Pressure[1].p_0001xbar_report != 0)*/)
	{
		u32_gLowPowerTimeAdd = 0;
		u32_gPowerDownTimeAdd = 0;
		u8_gLcdPowerOnEvent = EVENT_KEY_WAKE_UP;
		//u8_gLcdPowerOnEvent = (u8_gWakeUpEvent & WAKE_INTERRUPT_PLUG)?EVENT_PLUG_POWER_ON:EVENT_KEY_WAKE_UP;
		u8_gWakeUpEvent = 0;
		vLowPowerResume();
		vWakeUpLCD();
		vSetSysState(SYS_STATE_NORMAL);
		return;
	}
	
	if(st_gHL_Pressure[0].p_0001xbar_report != 0)
	{
		if(++wk_0bar_cnt1 >= 2)
		{
			wk_0bar_cnt1 = 0;
			u32_gLowPowerTimeAdd = 0;
			u32_gPowerDownTimeAdd = 0;
			u8_gLcdPowerOnEvent = EVENT_KEY_WAKE_UP;
			//u8_gLcdPowerOnEvent = (u8_gWakeUpEvent & WAKE_INTERRUPT_PLUG)?EVENT_PLUG_POWER_ON:EVENT_KEY_WAKE_UP;
			u8_gWakeUpEvent = 0;
			vLowPowerResume();
			vWakeUpLCD();
			vSetSysState(SYS_STATE_NORMAL);
		}
	}
	else
	{
		wk_0bar_cnt1 = 0;
	}
	
	if(st_gHL_Pressure[1].p_0001xbar_report != 0)
	{
		if(++wk_0bar_cnt2 >= 2)
		{
			wk_0bar_cnt2 = 0;
			u32_gLowPowerTimeAdd = 0;
			u32_gPowerDownTimeAdd = 0;
			u8_gLcdPowerOnEvent = EVENT_KEY_WAKE_UP;
			//u8_gLcdPowerOnEvent = (u8_gWakeUpEvent & WAKE_INTERRUPT_PLUG)?EVENT_PLUG_POWER_ON:EVENT_KEY_WAKE_UP;
			u8_gWakeUpEvent = 0;
			vLowPowerResume();
			vWakeUpLCD();
			vSetSysState(SYS_STATE_NORMAL);
		}
	}
	else
	{
		wk_0bar_cnt2 = 0;
	}
	


	u8_gWakeUpEvent = 0;
	McuStop();
	vBSP_WDG_Refresh();
	
	
	if(u8_gWakeUpEvent & WAKE_INTERRUPT_WKT)
	{
		u8_gWakeUpEvent = 0;
		u32_gPowerDownTimeAdd = ((st_gSysParam.shutdown_time != TIME_SHUTDOWN_OFF) && (u8_PowerDownEnable == TRUE))?u32_gPowerDownTimeAdd + LOWPOWER_LOOP_PERIOD:0;	

		SystemClock_Config();
		SysTick_Config(SystemCoreClock/1000);
		
		vBSP_Port_FullSpeed();
		P_LED1_CTRL(ON);
		P_LED2_CTRL(ON);
		
		vADS1230_Init();

	}
	

//	if(((u16_gFaultStatus & FAULT_LOW_VOL) && (ucPlugInsertStatus == STATUS_PLUG_REMOVE)) ||
//		(u32_gPowerDownTimeAdd > (ULONG)st_gSysParam.shutdown_time * 3600000))
	if(u32_gPowerDownTimeAdd > (ULONG)st_gSysParam.shutdown_time * 3600000)
	{
		u32_gLowPowerTimeAdd = 0;
		u32_gPowerDownTimeAdd = 0;
		u8_gWakeUpEvent = 0;
		vLowPowerResume();
		vSetSysState(SYS_STATE_STOP);
		return;
	}

	if((u8_gWakeUpEvent & (WAKE_INTERRUPT_SW | WAKE_INTERRUPT_PLUG)) || 
	(st_gHL_Pressure[0].p_0001xbar_report != 0) || (st_gHL_Pressure[1].p_0001xbar_report != 0))
	{
		u32_gLowPowerTimeAdd = 0;
		u32_gPowerDownTimeAdd = 0;
		u8_gLcdPowerOnEvent = EVENT_KEY_WAKE_UP;
		//u8_gLcdPowerOnEvent = (u8_gWakeUpEvent & WAKE_INTERRUPT_PLUG)?EVENT_PLUG_POWER_ON:EVENT_KEY_WAKE_UP;
		u8_gWakeUpEvent = 0;
		vLowPowerResume();
		vWakeUpLCD();
		vSetSysState(SYS_STATE_NORMAL);
		return;
	}


}

void vLowPowerSuspend(void)
{
	P_LED1_CTRL(OFF);
	P_LED2_CTRL(OFF);

	vADS1230_PowerDown();
	vBSP_USARTDeInit(USART_LCD1);
	vBSP_USARTDeInit(USART_LCD2);
	vBSP_USARTDeInit(USART_BLE);
	vBSP_ADC_DeInit();
	vBSP_WDG_Suspend();
}

void vLowPowerResume(void)
{
	SystemClock_Config();
    SysTick_Config(SystemCoreClock/1000);

	//vBSP_Port_FullSpeed();
	vBSP_USARTInit(USART_LCD1);
	vBSP_USARTInit(USART_LCD2);
	vBSP_USARTInit(USART_BLE);
	vBSP_Timer1Init();
	vBSP_ADC_Init();
	vADS1230_Init();
	P_LCD_POWER_CTRL(ON);
}


void vChargeManage(void)
{
	static USHORT wk_ot_trig_time = 0;
	static USHORT wk_ot_rls_time = 0;
	static USHORT wk_low_vol_trig_time = 0;
	static USHORT wk_low_vol_rls_time = 0;

	


	if(u16_gFaultStatus & FAULT_LOW_VOL)
	{
		if(s16_gBatVoltage >= 3400)
		{
			wk_low_vol_rls_time+=50;
			if(wk_low_vol_rls_time >= 2000)
			{
				wk_low_vol_rls_time = 0;
				u16_gFaultStatus &= ~FAULT_LOW_VOL;
			}
		}
	}
	else
	{
		if(s16_gBatVoltage < 3400)
		{
			wk_low_vol_trig_time+=50;
			if(wk_low_vol_trig_time >= 2000)
			{
				wk_low_vol_trig_time = 0;
				u16_gFaultStatus |= FAULT_LOW_VOL;
			}
		}
	}
	

	if(u16_gFaultStatus & FAULT_BAT_OT)
	{
		if((s16_gBatTemp <= 500) && (s16_gBatTemp >= -150))
		{
			wk_ot_rls_time+=50;
			if(wk_ot_rls_time >= 2000)
			{
				wk_ot_rls_time = 0;
				u16_gFaultStatus &= ~FAULT_BAT_OT;
			}
		}
	}
	else
	{
		if((s16_gBatTemp > 550) || (s16_gBatTemp < -200))
		{
			wk_ot_trig_time+=50;
			if(wk_ot_trig_time >= 2000)
			{
				wk_ot_trig_time = 0;
				u16_gFaultStatus |= FAULT_BAT_OT;
			}
		}
	}

	//充电器状态拔出，OFF，，插入ON
	if(u16_gFaultStatus & FAULT_BAT_OT)
	{
		P_CHARGE_ENABLE_CTRL(OFF);
	}
	else
	{
		P_CHARGE_ENABLE_CTRL(ON);
	}

}

UCHAR u8_gChgComplete = FALSE;
UCHAR u8_gCharging = FALSE;
USHORT complete4135 = 0;
USHORT complete4175 = 0;
USHORT complete4170 = 0;

USHORT charging4025 = 0;
USHORT charging4135 = 0;
USHORT charging4080 = 0;
USHORT charging4175 = 0;
USHORT charging4170 = 0;

void ucChargeStateCheck(void)
{
	static UCHAR wk_chgcomplete = FALSE;
	static UCHAR wk_charging = FALSE;
	static SHORT full_vol_when_idle=4135;
	if(SYS_STATE_CHARG_IN_LOWPOWER == u8_gSysState)
		full_vol_when_idle=4115;
	else
		full_vol_when_idle=4135;
	
	wk_charging = u8_gCharging;
	wk_chgcomplete = u8_gChgComplete;
	
	if(((ucPlugInsertStatus == STATUS_PLUG_INSERT) && (ucChgCompleteStatus == TRUE) && (s16_gBatVoltage >= full_vol_when_idle)))
	{
		if(SYS_STATE_NORMAL == u8_gSysState)
		{
			if(complete4135++>200)//10sec
			{
				complete4135=0;
				wk_chgcomplete = TRUE;
			
				wk_charging = FALSE;
			}
		}
		else
		{
			wk_chgcomplete = TRUE;
			
			wk_charging = FALSE;
		}

	}
	else if(ucPlugInsertStatus == STATUS_PLUG_REMOVE )
	{
		complete4135=0;
	}
	if((ucPlugInsertStatus == STATUS_PLUG_INSERT) && (ucChgCompleteStatus == FALSE))
	{
		if((SYS_STATE_CHARG_IN_STOP == u8_gSysState) || (SYS_STATE_CHARG_IN_LOWPOWER == u8_gSysState))
		{
			if(s16_gBatVoltage >= 4175)
			{
				wk_chgcomplete = TRUE;
				
				wk_charging = FALSE;
				complete4175++;
			}
		}
		else
		{
			if(s16_gBatVoltage >= 4170)
			{
				wk_chgcomplete = TRUE;
				
				wk_charging = FALSE;
				complete4170++;
			}
		}
	}	


	if((ucPlugInsertStatus == STATUS_PLUG_INSERT) && (ucChgCompleteStatus == TRUE))
	{
		if(u8_gChgComplete == TRUE)
		{
			if(SYS_STATE_CHARG_IN_STOP == u8_gSysState)
			{
				if(s16_gBatVoltage < 4000)
				{
					wk_charging = TRUE;
					
					wk_chgcomplete = FALSE;
					charging4025++;
				}
			}
			else
			{
				if(s16_gBatVoltage < 4020)
				{
					wk_charging = TRUE;
					
					wk_chgcomplete = FALSE;
					charging4025++;
				}
			}
		}
		else
		{
			if(s16_gBatVoltage < full_vol_when_idle)
			{
				wk_charging = TRUE;
				
				wk_chgcomplete = FALSE;
				charging4135++;
			}
		}
	}
	
	if((ucPlugInsertStatus == STATUS_PLUG_INSERT) && (ucChgCompleteStatus == FALSE))
	{
		if(u8_gChgComplete == TRUE)
		{
			if(s16_gBatVoltage < 4080)
			{
				wk_charging = TRUE;
				
				wk_chgcomplete = FALSE;
				charging4080++;
			}
		}
		else
		{
			if((SYS_STATE_CHARG_IN_STOP == u8_gSysState) || (SYS_STATE_CHARG_IN_LOWPOWER == u8_gSysState))
			{
				if(s16_gBatVoltage < 4175)
				{
					wk_charging = TRUE;
					
					wk_chgcomplete = FALSE;
					charging4175++;
				}
			}
			else
			{
				if(s16_gBatVoltage < 4170)
				{
					wk_charging = TRUE;
					
					wk_chgcomplete = FALSE;
					charging4170++;
				}
			}
		}
	}

	
//	if(((ucPlugInsertStatus == STATUS_PLUG_INSERT) && (ucChgCompleteStatus == TRUE) && (s16_gBatVoltage < 4100)))
//	{
//		wk_charging = TRUE;
//	}
//	if(((ucPlugInsertStatus == STATUS_PLUG_INSERT) && (ucChgCompleteStatus == FALSE) && (s16_gBatVoltage <= 4185)))
//	{
//		wk_charging = TRUE;
//	}

	u8_gChgComplete = wk_chgcomplete;
	u8_gCharging = wk_charging;
}


void vWakeUpLCD(void)
{
//	ucBSP_UartSendDataPoll(USART_LCD1,"HELLO",5);	
//	ucBSP_UartSendDataPoll(USART_LCD2,"HELLO",5);	
	
	P_LCD_POWER_CTRL(OFF);	
	P_LCD_POWER_CTRL(OFF);	
	vBSP_DelayMs(50);
	P_LCD_POWER_CTRL(ON);	
	P_LCD_POWER_CTRL(ON);	
}

void vComLostCheck(void)
{
	if(u8_gComLostEnable == FALSE)
	{
		u16_ComLostOt1 = 0;
		u16_ComLostOt2 = 0;
		return;
	}
	
	if((u16_ComLostOt1 >= COM_LOST_TIME) ||(u16_ComLostOt2 >= COM_LOST_TIME))
	{
		u16_ComLostOt1 = 0;
		u16_ComLostOt2 = 0;
		P_LCD_POWER_CTRL(OFF);	
		P_LCD_POWER_CTRL(OFF);	
		vBSP_DelayMs(50);
		P_LCD_POWER_CTRL(ON);	
		P_LCD_POWER_CTRL(ON);	
	}
}
