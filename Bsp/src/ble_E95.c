#include <string.h>
#include <stdio.h>
#include "sys_type.h"
#include "w25q64.h"
#include "bsp_usart.h"
#include "bsp_timer.h"
#include "ble_E95.h"
#include "app_System.h"
//#include "app_user.h"
#include "app_flash.h"
#include "app_io.h"

#define _delay_ms(dly)	vBSP_DelayMs(dly)

UCHAR u8_gBleState;
UCHAR u8_gBleModuleOffLineWithMcu = FALSE;
UCHAR u8_aBleMacAddress[12] = {0};

UCHAR ucBLE_CheckPowerOn(void)
{
	UCHAR wk_st;
	wk_st = ((GPIO_ReadOutputDataBit(GPIOB, GPIO_PIN_9) == 0)?1:0);	
	return wk_st;
}

UCHAR ucBLE_CheckMode(void)
{
	UCHAR wk_st;
	wk_st = ((GPIO_ReadOutputDataBit(GPIOC, GPIO_PIN_13) == 0)?1:0);	
	return wk_st;
}
UCHAR ucBLE_CheckConnect(void)
{
	UCHAR wk_st;
	wk_st = ((GPIO_ReadInputDataBit(GPIOB, GPIO_PIN_4) == 0)?1:0);	
	
	return wk_st;
}
UCHAR ucBLE_CheckBusy(void)
{
	UCHAR wk_st;
	wk_st = ((GPIO_ReadInputDataBit(GPIOB, GPIO_PIN_3) == 0)?1:0);	
	
	return wk_st;
}
UCHAR ucBLE_CheckActive(void)
{
	UCHAR wk_st;
	wk_st = ((GPIO_ReadInputDataBit(GPIOC, GPIO_PIN_14) == 0)?1:0);	
	
	return wk_st;
}

static void vBLE_WaitUartResp(UCHAR *resp_dat,UCHAR dat_len,USHORT ot_time)
{
	vTIM_1msStart(ot_time);
	while(!vTIM_1msIsTimeout())
	{
		if(BLE_UART_REV_COMPLETE == TRUE)
		{
			BLE_UART_REV_COMPLETE = FALSE;
			if(resp_dat !=NULL)
			{
				memcpy(resp_dat,&BLE_UART_REV_BUFF[5],dat_len);
			}
			BLE_UART_REV_IDX = 0;
			BLE_UART_REV_OVER_TIME = 0;
			BLE_UART_REV_COMPLETE = FALSE;
			memset(BLE_UART_REV_BUFF,0,BLE_UART_REV_BUFF_LEN);
			vTIM_1msStart(0);
			return;
		}
	}

	u8_gBleModuleOffLineWithMcu = TRUE;
	if(resp_dat !=NULL)
	{
		memset(resp_dat,0,dat_len);
	}
	vTIM_1msStart(0);
	return;
}

void vBLE_E95_Reset(void)
{
	BLE_RESET_CTRL(LOW);
	vBSP_DelayMs(10);
	BLE_RESET_CTRL(HIGH);
}


/*设备名称*/
void vBLE_E95_BroadcastName_Set(CHAR * name)
{
	CHAR wk_cmd[40] = {0x01,0xFC,0x07};
	CHAR wk_len = strlen(name);
	
	BLE_MODE_CTRL(SEND_MODE_CMD);

	if(wk_len > 20)
	{
		wk_len = strlen("NAME_ERROR");
		wk_cmd[3] = wk_len;
		strcpy(&wk_cmd[4],"NAME_ERROR");
	}
	else
	{
		wk_cmd[3] = wk_len;
		strcpy(&wk_cmd[4],name);
	}
	BLE_UART_REV_IDX = 0;
	BLE_UART_REV_OVER_TIME = 0;
	BLE_UART_REV_COMPLETE = FALSE;
	BLE_UART_SEND_DATA((UCHAR*)wk_cmd,wk_len + 4);
	vBLE_WaitUartResp(NULL,0,200);	
}

/*广播间隔*/
//void vBLE_E95_AdvInterval_Set(USHORT interval)
//{
//	
//}

/*广播数据设定*/
void vBLE_E95_BroadcastDataSet(CHAR *broadcast_data,UCHAR data_len,UCHAR save_en)
{
	CHAR wk_cmd[40] = {0x01,0xFC,0x0D};
	CHAR wk_len = 1 + data_len;

	BLE_MODE_CTRL(SEND_MODE_CMD);

	wk_cmd[3] = wk_len;
	wk_cmd[4] = save_en;
	memcpy(&wk_cmd[5],broadcast_data,data_len);
	
	BLE_UART_REV_IDX = 0;
	BLE_UART_REV_OVER_TIME = 0;
	BLE_UART_REV_COMPLETE = FALSE;
	BLE_UART_SEND_DATA((UCHAR*)wk_cmd,wk_len + 4);
	vBLE_WaitUartResp(NULL,0,200);	
}

/*设置UUID*/
//void vBLE_E95_UUID_Set(UCHAR item,USHORT uuid)
//{
//	CHAR wk_cmd[40] = {0x01,0xFC,0x11};
//	CHAR wk_len = 3;
	
//	wk_cmd[3] = wk_len;
//	wk_cmd[4] = item;
//	wk_cmd[5] = uuid & 0xFF;
//	wk_cmd[6] = (uuid >> 8) & 0xFF;

//	BLE_UART_REV_IDX = 0;
//	BLE_UART_SEND_DATA(wk_cmd,wk_len + 4);
//}

/*蓝牙状态设定*/
void vBLE_E95_State_Set(UCHAR state)
{
	CHAR wk_cmd[10] = {0x01,0xFC,0x13};
	CHAR wk_len = 1;

	BLE_MODE_CTRL(SEND_MODE_CMD);

	wk_cmd[3] = wk_len;
	wk_cmd[4] = state;
	BLE_UART_REV_IDX = 0;
	BLE_UART_REV_OVER_TIME = 0;
	BLE_UART_REV_COMPLETE = FALSE;
	BLE_UART_SEND_DATA((UCHAR*)wk_cmd,wk_len + 4);
	vBLE_WaitUartResp(NULL,0,200);	
	
	BLE_MODE_CTRL(SEND_MODE_TOUCHUAN);
}

/*蓝牙状态获取*/
void vBLE_E95_State_Check(UCHAR *ble_state)
{
	UCHAR wk_state = STATE_CHECK_BLE_UNKNOW;
	//CHAR wk_cmd[5] = {0x01,0xFC,0x14,0x00};

	if(BLE_IS_POWER_ON())
	{
#if 0
		BLE_MODE_CTRL(SEND_MODE_CMD);

		BLE_UART_REV_IDX = 0;
		BLE_UART_REV_OVER_TIME = 0;
		BLE_UART_REV_COMPLETE = FALSE;
		memset(u8_gUart1RevBuffer,0,sizeof(u8_gUart1RevBuffer));

		BLE_UART_SEND_DATA(wk_cmd,4);
		vBLE_WaitUartResp(&wk_state,1,100);
#endif
	wk_state = (BLE_IS_CONNECTED())?STATE_CHECK_BLE_CONNNECTED:STATE_CHECK_BLE_IDLE;
	}

	//BLE_POWER_CTRL(1);
	*ble_state = wk_state;
}


void vBLE_E95_GetMacAdddress(UCHAR *pMac)
{
	CHAR wk_cmd[5] = {0x01,0xFC,0x23,0x00};
	
	BLE_MODE_CTRL(SEND_MODE_CMD);
	
	BLE_UART_REV_IDX = 0;
	BLE_UART_REV_OVER_TIME = 0;
	BLE_UART_REV_COMPLETE = FALSE;
	memset(BLE_UART_REV_BUFF,0,BLE_UART_REV_BUFF_LEN);

	BLE_UART_SEND_DATA((UCHAR*)wk_cmd,4);
	vBLE_WaitUartResp(pMac,6,200);

}


//void vBLE_E95_GetMtu(UCHAR *pMac)
//{
//	CHAR wk_cmd[5] = {0x01,0xFC,0x18,0x01,0x05};
//	BLE_UART_REV_IDX = 0;
//	BLE_UART_REV_OVER_TIME = 0;
//	BLE_UART_REV_COMPLETE = FALSE;
//	memset(u8_gUart1RevBuffer,0,sizeof(u8_gUart1RevBuffer));
//
//	BLE_UART_SEND_DATA(wk_cmd,5);
//
//	vTIM_1msStart(50);
//	while(!vTIM_1msIsTimeout())
//	{
//		if(BLE_UART_REV_COMPLETE == TRUE)
//		{
//			BLE_UART_REV_COMPLETE = FALSE;
//			memcpy(pMac,&u8_gUart1RevBuffer[5],6);
//			
//			BLE_UART_REV_IDX = 0;
//			BLE_UART_REV_OVER_TIME = 0;
//			BLE_UART_REV_COMPLETE = FALSE;
//			memset(u8_gUart1RevBuffer,0,sizeof(u8_gUart1RevBuffer));
//			break;
//		}
//	}
//	vTIM_1msStart(0);
//}

/*发射功率设置*/
static void vBLE_E95_TxPower_Set(void)
{
	CHAR wk_cmd[10] = {0x01,0xFC,0x0b,0x01};
	wk_cmd[4] = 0x08;	//0x07：3dbm  0x08:4dbm
	
	BLE_MODE_CTRL(SEND_MODE_CMD);
	
	BLE_UART_REV_IDX = 0;
	BLE_UART_REV_OVER_TIME = 0;
	BLE_UART_REV_COMPLETE = FALSE;

	BLE_UART_SEND_DATA((UCHAR*)wk_cmd, 5);
	vBLE_WaitUartResp(NULL,0,200);
}

static void vBLE_E95_UartConfig(void)
{
	CHAR wk_cmd[10] = {0x01,0xFC,0x01,0x03,0x02,0x00,0x0A};
	BLE_MODE_CTRL(SEND_MODE_CMD);
	
	BLE_UART_REV_IDX = 0;
	BLE_UART_REV_OVER_TIME = 0;
	BLE_UART_REV_COMPLETE = FALSE;

	BLE_UART_SEND_DATA((UCHAR*)wk_cmd, 7);
	vBLE_WaitUartResp(NULL,0,500);
}


//void vBLE_E95_GetPower(void)
//{
//	CHAR wk_cmd[5] = {0x01,0xFC,0x0C,0X00};
//	BLE_UART_REV_IDX = 0;
//	BLE_UART_REV_OVER_TIME = 0;
//	BLE_UART_REV_COMPLETE = FALSE;
//	memset(u8_gUart1RevBuffer,0,sizeof(u8_gUart1RevBuffer));
//
//	BLE_UART_SEND_DATA(wk_cmd,4);
//
//	vTIM_1msStart(50);
//	while(!vTIM_1msIsTimeout())
//	{
//		if(BLE_UART_REV_COMPLETE == TRUE)
//		{
//			BLE_UART_REV_COMPLETE = FALSE;
//			//memcpy(pMac,&u8_gUart1RevBuffer[5],6);
//			
//			BLE_UART_REV_IDX = 0;
//			BLE_UART_REV_OVER_TIME = 0;
//			BLE_UART_REV_COMPLETE = FALSE;
//			memset(u8_gUart1RevBuffer,0,sizeof(u8_gUart1RevBuffer));
//			break;
//		}
//	}
//	vTIM_1msStart(0);
//}



void vBLE_E95_Init(void)
{
    GPIO_InitType GPIO_InitStructure;    
	
	static CHAR wk_broadcast_name[20] = {0};
	static CHAR wk_broadcast_data[40] = {0};
	UCHAR wk_len;

	GPIO_InitStruct(&GPIO_InitStructure);

	/*
		PA15 reset
		PB8 bt_ctrl
		PB9 pwr ctrl
		PC13 mode ctrl	
	*/
	GPIO_InitStructure.Pin		  	= GPIO_PIN_15;
	GPIO_InitStructure.GPIO_Pull	= GPIO_No_Pull;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
	GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.Pin		  	= GPIO_PIN_8 |GPIO_PIN_9;
	GPIO_InitStructure.GPIO_Pull	= GPIO_No_Pull;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
	GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.Pin		  	= GPIO_PIN_13;
	GPIO_InitStructure.GPIO_Pull	= GPIO_No_Pull;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
	GPIO_InitPeripheral(GPIOC, &GPIO_InitStructure);

	/*
		PC14 BLE Status
		PC15 DOUT_IND
		PB4 CONN_IND
		PB3 BUSY_IND
	*/
	GPIO_InitStructure.Pin		  = GPIO_PIN_14 |GPIO_PIN_15;
	GPIO_InitStructure.GPIO_Pull	= GPIO_Pull_Up;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Input;
	GPIO_InitPeripheral(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.Pin		  = GPIO_PIN_3 |GPIO_PIN_4;
	GPIO_InitStructure.GPIO_Pull	= GPIO_Pull_Up;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Input;
	GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);


	BLE_POWER_CTRL(0);
	BLE_MODE_CTRL(SEND_MODE_CMD);
	BLE_BT_CTRL(0);

	vBLE_E95_Reset();
	
	_delay_ms(10);
	vBLE_E95_UartConfig();
	
	_delay_ms(50);
	vBLE_E95_GetMacAdddress(u8_aBleMacAddress);

	sprintf(wk_broadcast_name,"NAVAC-NST3-%02X%02X%02X",(int)u8_aBleMacAddress[3],(int)u8_aBleMacAddress[4],(int)u8_aBleMacAddress[5]);
	vBLE_E95_BroadcastName_Set(wk_broadcast_name);
	_delay_ms(50);

	wk_broadcast_data[1] = 0xFF;	/*厂商自定义数据*/
	wk_len = strlen("NAVAC-NST3-");
	wk_broadcast_data[0] = 1 + wk_len+ 6;
	strcpy(&wk_broadcast_data[2],"NAVAC-NST3-");
	memcpy(&wk_broadcast_data[2+wk_len],u8_aBleMacAddress,6);
	
	vBLE_E95_BroadcastDataSet(wk_broadcast_data,strlen(wk_broadcast_data),0);
	vBLE_E95_TxPower_Set();
	_delay_ms(50);
	vBLE_E95_State_Set(STATE_SET_BLE_ON);
	
	BLE_MODE_CTRL(SEND_MODE_TOUCHUAN);
}


void ucBLE_E95_Notify_Data(CHAR *pData,UCHAR len)
{
	UCHAR wk_retry;
	CHAR wk_cmd[40] = {0x01,0xFC,0x41};
	CHAR wk_len = len;

	if(STATE_CHECK_BLE_CONNNECTED == u8_gBleState)
	{
		wk_cmd[3] = wk_len;
		memcpy(&wk_cmd[4],pData,len);

		for(wk_retry = 0;wk_retry<5;wk_retry++)
		{
			BLE_UART_REV_IDX = 0;
			BLE_UART_REV_OVER_TIME = 0;
			BLE_UART_REV_COMPLETE = FALSE;
			memset(BLE_UART_REV_BUFF,0,BLE_UART_REV_BUFF_LEN);
			BLE_UART_SEND_DATA((UCHAR*)wk_cmd,wk_len + 4);
			
			vTIM_1msStart(1000);
			while(!vTIM_1msIsTimeout())
			{			
				if(BLE_UART_REV_COMPLETE == TRUE)
				{
					if(BLE_UART_REV_BUFF[4] == 0x01)
					{
						BLE_UART_REV_COMPLETE = FALSE;				
						break;	//失败，重试
					}
					else
					{
						BLE_UART_REV_COMPLETE = FALSE;				
						BLE_UART_REV_IDX = 0;
						BLE_UART_REV_OVER_TIME = 0;
						memset(BLE_UART_REV_BUFF,0,BLE_UART_REV_BUFF_LEN);
						return;
					}
				}
			}
			vTIM_1msStart(0);
		}

		
		//_delay_ms(50);	/*等待数据彻底发送完成*/
	}
}



void vBLE_E95_SendData(UCHAR *dat,UCHAR len,UCHAR mode)	//0:命令模式  1:透传
{
	if(mode)
	{
		BLE_MODE_CTRL(SEND_MODE_TOUCHUAN);
		BLE_UART_SEND_DATA(dat,len);
		_delay_ms(10);
	}
	else
	{
		ucBLE_E95_Notify_Data((CHAR*)dat,len);
	}
}

