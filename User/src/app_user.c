#include "sys_type.h"
#include "n32l40x.h"
#include "bsp_io.h"
#include "bsp_usart.h"
#include "bsp_timer.h"
#include "bsp_wdg.h"
#include "ble_E95.h"
#include "ADS1230.h"
#include "app_io.h"
#include "app_system.h"
#include "app_sample.h"
#include "app_user.h"
#include "app_flash.h"

extern const UCHAR VERSION[3];
extern const UCHAR VERSION_S;
void vUserInterfaceShankhands(ST_LCD_FORM *pform);
void vUserInterfaceShankhands1(ST_LCD_FORM *pform);
void vUserInterfaceConfigParamRequest(ST_LCD_FORM *pform);
void vUserInterfaceConfigParamSeting(ST_LCD_FORM *pform);
void vUserInterfaceRefriListRequest(ST_LCD_FORM *pform);
void vUserInterfacePTRequest(ST_LCD_FORM *pform);
void vUserInterfaceDisablePowerDown(ST_LCD_FORM *pform);
void vDebugInterfacePressureAdcRequest(ST_LCD_FORM *pform);
void vDebugInterfacePressureCalibration(ST_LCD_FORM *pform);
void vDebugInterfaceCaliParamRead(ST_LCD_FORM *pform);
void vDebugInterfaceN32StatusRead(ST_LCD_FORM *pform);
void vDebugInterfacePowerDownCtrl(ST_LCD_FORM *pform);
void vDebugInterfaceComLostCheckCtrl(ST_LCD_FORM *pform);
void vSendCmdToLcd(UCHAR cmd);

USART_Module* USARTx_Active = USART_LCD1;
UCHAR aucRxMsgPool[RECV_POOL_SIZE];

UCHAR u8_gSlaveParamIsChanged =FALSE;
UCHAR u8_gSlaveIsError =FALSE;
UCHAR u8_gLCDParam[2][4] = {0};
USHORT u16_ComLostOt1 = 0;
USHORT u16_ComLostOt2 = 0;
UCHAR u8_gComLostEnable = TRUE;



UCHAR vCheckSumCal(UCHAR* InData,UCHAR InLen)
{
	UCHAR wk_check_val = 0;
	UCHAR wk_idx;

	for(wk_idx = 0;wk_idx < InLen;wk_idx++)
	{
		wk_check_val += InData[wk_idx];
	}
	return (wk_check_val);
}

UCHAR ucLCDFrameCheck(ST_LCD_FORM *pstForm, USHORT usDatSize)
{
    UCHAR ucRet = TRUE;
	UCHAR CheckSumRev =0;
	UCHAR CheckSumCal = 0;

    /* addr check */
    if(pstForm->head != 0x55)
    {
        ucRet = FALSE;
    }
    if(usDatSize < 5)
    {
        ucRet = FALSE;
    }
    
    /*checksum check*/
    if(ucRet == TRUE)
    {
		CheckSumCal = vCheckSumCal((UCHAR*)pstForm,usDatSize - 1);
		CheckSumRev = ((UCHAR *)pstForm)[usDatSize - 1];		
        if(CheckSumRev != CheckSumCal)
        {
            ucRet = FALSE;
        }
    }
    return ucRet;
}

UCHAR ucUserInterfaceOfLCD(void)
{
	UCHAR wk_ret = FALSE;
    USHORT usPacketSize = 0;
	ST_LCD_FORM * pstLCDform = (ST_LCD_FORM *)aucRxMsgPool;		//for lcd
    
    if(ucBSP_UartReceive(USART_LCD1,aucRxMsgPool, &usPacketSize) == TRUE)
    {
		if(ucLCDFrameCheck(pstLCDform,usPacketSize) == TRUE)
		{
			u16_ComLostOt1 = 0;
			USARTx_Active = USART_LCD1;
			wk_ret = ucInterfaceOfLCD(pstLCDform);
		}
    }    
	if(ucBSP_UartReceive(USART_LCD2,aucRxMsgPool, &usPacketSize) == TRUE)
    {
		if(ucLCDFrameCheck(pstLCDform,usPacketSize) == TRUE)
		{
			u16_ComLostOt2 = 0;
			USARTx_Active = USART_LCD2;
			wk_ret = ucInterfaceOfLCD(pstLCDform);
		}
    }
	return wk_ret;
}

UCHAR ucInterfaceOfLCD(ST_LCD_FORM *pform)
{
	UCHAR wk_ret = FALSE;
	UCHAR wk_cmd = pform->cmd;
	
	switch(wk_cmd)
	{
		case CMD_LCD_SHANKHANDS:
			vUserInterfaceShankhands(pform);
			break;
		case 0x01:
			vUserInterfaceShankhands1(pform);
			break;
		case CMD_LCD_CONFIG_PARAM_REQ:
			vUserInterfaceConfigParamRequest(pform);
			break;
		case CMD_LCD_CONFIG_PARAM_SET:
			vUserInterfaceConfigParamSeting(pform);
			break;
		case CMD_LCD_REFRI_LIST_REQ:
			vUserInterfaceRefriListRequest(pform);
			break;
		case CMD_LCD_P_T_REQ:
			vUserInterfacePTRequest(pform);
			break;
		case 0x02:
			vUserInterfaceDisablePowerDown(pform);
			break;

		case 0x31:
			vDebugInterfacePressureAdcRequest(pform);
			break;
		case 0x41:
		case 0x42:
		case 0x45:
			vDebugInterfacePressureCalibration(pform);
			break;
		case 0x48:
			vDebugInterfaceCaliParamRead(pform);
			break;
		case 0x50:
			vDebugInterfaceN32StatusRead(pform);
			break;
		case 0x51:
			vDebugInterfacePowerDownCtrl(pform);
			break;
		case 0x52:
			vDebugInterfaceComLostCheckCtrl(pform);
			break;
		default:
			break;
	}
	return wk_ret;
}
void vUserInterfaceShankhands(ST_LCD_FORM *pform)
{
	static UCHAR wk_lcd_active = 0;
	USHORT wk_idx = 0;
	UCHAR wk_send_data[50] = {0};
	ST_LCD_FORM * pstLCDform = (ST_LCD_FORM *)&wk_send_data;

	pstLCDform->head = 0xA5;
	pstLCDform->cmd = pform->cmd;
	pstLCDform->data_value[wk_idx++] = u8_gLcdPowerOnEvent;
	if(USARTx_Active == USART_LCD1)	//高压侧
	{
		pstLCDform->data_value[wk_idx++] = u8_gLCDParam[0][0];
		pstLCDform->data_value[wk_idx++] = u8_gLCDParam[0][1];
		pstLCDform->data_value[wk_idx++] = u8_gLCDParam[0][2];
		pstLCDform->data_value[wk_idx++] = u8_gLCDParam[0][3];
		pstLCDform->data_value[wk_idx++] = VERSION_S;
		wk_lcd_active |= 0x01;
	}
	else if(USARTx_Active == USART_LCD2)	//低压侧
	{
		pstLCDform->data_value[wk_idx++] = u8_gLCDParam[1][0];
		pstLCDform->data_value[wk_idx++] = u8_gLCDParam[1][1];
		pstLCDform->data_value[wk_idx++] = u8_gLCDParam[1][2];
		pstLCDform->data_value[wk_idx++] = u8_gLCDParam[1][3];
		pstLCDform->data_value[wk_idx++] = VERSION_S;
		wk_lcd_active |= 0x02;
	}
	pstLCDform->len = wk_idx;
	pstLCDform->data_value[wk_idx++] = vCheckSumCal(wk_send_data, pstLCDform->len + 4);
	
	ucBSP_UartSendDataPoll(USARTx_Active,wk_send_data,pstLCDform->len + 5);

	//两个屏幕都请求之后，就置成唤醒事件，
	//如果LCD因为esd导致重启再次查询时,这时候返回的是EVENT_KEY_WAKE_UP，这样LCD就不会跑开机动画了
	if(wk_lcd_active == (0x01 & 0x02))
	{
		wk_lcd_active = 0;
		u8_gLcdPowerOnEvent = EVENT_KEY_WAKE_UP;
	}
}

void vUserInterfaceShankhands1(ST_LCD_FORM *pform)
{
	static UCHAR wk_lcd_active = 0;
	USHORT wk_idx = 0;
	UCHAR wk_data = 0;
	UCHAR wk_send_data[50] = {0};
	ST_LCD_FORM * pstLCDform = (ST_LCD_FORM *)&wk_send_data;

	pstLCDform->head = 0xA5;
	pstLCDform->cmd = pform->cmd;
	pstLCDform->data_value[wk_idx++] = u8_gLcdPowerOnEvent;
	if(USARTx_Active == USART_LCD1)	//高压侧
	{
		pstLCDform->data_value[wk_idx++] = u8_gLCDParam[0][0];
		pstLCDform->data_value[wk_idx++] = u8_gLCDParam[0][1];
		pstLCDform->data_value[wk_idx++] = u8_gLCDParam[0][2];
		pstLCDform->data_value[wk_idx++] = u8_gLCDParam[0][3];
		
		pstLCDform->data_value[wk_idx++] = st_gSysParam.refrigerant_type;
		pstLCDform->data_value[wk_idx++] = st_gSysParam.pressure_unit;
		pstLCDform->data_value[wk_idx++] = st_gSysParam.temp_unit;
		pstLCDform->data_value[wk_idx++] = st_gSysParam.shutdown_time;
		pstLCDform->data_value[wk_idx++] = st_gSysParam.lowpower_time;
		pstLCDform->data_value[wk_idx++] = VERSION[0];
		pstLCDform->data_value[wk_idx++] = VERSION[1];
		pstLCDform->data_value[wk_idx++] = VERSION[2];
		pstLCDform->data_value[wk_idx++] = 1;
		pstLCDform->data_value[wk_idx++] = 2;
		pstLCDform->data_value[wk_idx++] = 3;
		pstLCDform->data_value[wk_idx++] = 4;
		pstLCDform->data_value[wk_idx++] = 5;
		pstLCDform->data_value[wk_idx++] = 6;
		pstLCDform->data_value[wk_idx++] = (st_gHL_Pressure[0].p_0001xbar_report >> 0) & 0xFF;
		pstLCDform->data_value[wk_idx++] = (st_gHL_Pressure[0].p_0001xbar_report >> 8) & 0xFF;
		pstLCDform->data_value[wk_idx++] = (st_gHL_Pressure[0].p_0001xbar_report >> 16) & 0xFF;
		pstLCDform->data_value[wk_idx++] = (st_gHL_Pressure[0].p_0001xbar_report >> 24) & 0xFF;
		pstLCDform->data_value[wk_idx++] = s16_gHL_Temp[0] & 0xFF;
		pstLCDform->data_value[wk_idx++] = (s16_gHL_Temp[0]>>8) & 0xFF;

		if(ucPlugInsertStatus == STATUS_PLUG_REMOVE)
		{
			wk_data = 0 << 4;
		}
		else
		{
			if(IN_CHARGING_ERROR())
			{
				wk_data = 3 << 4;
			}
			else if(IN_CHARGING_COMPLETE())
			{
				wk_data = 2 << 4;
			}
			else if(IN_CHARGING())
			{
				wk_data = 1 << 4;
			}
		}		
		wk_data |= u8_gBatSocLevel & 0x0F;
		pstLCDform->data_value[wk_idx++] = wk_data & 0xFF;
		wk_data = (STATE_CHECK_BLE_CONNNECTED == u8_gBleState)?1:0;
		pstLCDform->data_value[wk_idx++] = wk_data;
		wk_lcd_active |= 0x01;
	}
	else if(USARTx_Active == USART_LCD2)	//低压侧
	{
		pstLCDform->data_value[wk_idx++] = u8_gLCDParam[1][0];
		pstLCDform->data_value[wk_idx++] = u8_gLCDParam[1][1];
		pstLCDform->data_value[wk_idx++] = u8_gLCDParam[1][2];
		pstLCDform->data_value[wk_idx++] = u8_gLCDParam[1][3];

		pstLCDform->data_value[wk_idx++] = st_gSysParam.refrigerant_type;
		pstLCDform->data_value[wk_idx++] = st_gSysParam.pressure_unit;
		pstLCDform->data_value[wk_idx++] = st_gSysParam.temp_unit;
		pstLCDform->data_value[wk_idx++] = st_gSysParam.shutdown_time;
		pstLCDform->data_value[wk_idx++] = st_gSysParam.lowpower_time;
		pstLCDform->data_value[wk_idx++] = VERSION[0];
		pstLCDform->data_value[wk_idx++] = VERSION[1];
		pstLCDform->data_value[wk_idx++] = VERSION[2];
		pstLCDform->data_value[wk_idx++] = 1;
		pstLCDform->data_value[wk_idx++] = 2;
		pstLCDform->data_value[wk_idx++] = 3;
		pstLCDform->data_value[wk_idx++] = 4;
		pstLCDform->data_value[wk_idx++] = 5;
		pstLCDform->data_value[wk_idx++] = 6;
		pstLCDform->data_value[wk_idx++] = (st_gHL_Pressure[1].p_0001xbar_report >> 0) & 0xFF;
		pstLCDform->data_value[wk_idx++] = (st_gHL_Pressure[1].p_0001xbar_report >> 8) & 0xFF;
		pstLCDform->data_value[wk_idx++] = (st_gHL_Pressure[1].p_0001xbar_report >> 16) & 0xFF;
		pstLCDform->data_value[wk_idx++] = (st_gHL_Pressure[1].p_0001xbar_report >> 24) & 0xFF;
		pstLCDform->data_value[wk_idx++] = s16_gHL_Temp[1] & 0xFF;
		pstLCDform->data_value[wk_idx++] = (s16_gHL_Temp[1]>>8) & 0xFF;
		wk_data = (IN_CHARGING())?(0x01<<4):wk_data;
		wk_data = (IN_CHARGING_COMPLETE())?(0x02<<4):wk_data;
		wk_data |= u8_gBatSocLevel & 0x0F;
		pstLCDform->data_value[wk_idx++] = wk_data & 0xFF;
		wk_data = (STATE_CHECK_BLE_CONNNECTED == u8_gBleState)?1:0;
		pstLCDform->data_value[wk_idx++] = wk_data;
		wk_lcd_active |= 0x02;
	}
	pstLCDform->data_value[wk_idx++] = VERSION_S;
	pstLCDform->len = wk_idx;
	pstLCDform->data_value[wk_idx++] = vCheckSumCal(wk_send_data, pstLCDform->len + 4);
	
	ucBSP_UartSendDataPoll(USARTx_Active,wk_send_data,pstLCDform->len + 5);
	
	//两个屏幕都请求之后，就置成唤醒事件，
	//如果LCD因为esd导致重启再次查询时,这时候返回的是EVENT_KEY_WAKE_UP，这样LCD就不会跑开机动画了
	if(wk_lcd_active == (0x01 & 0x02))
	{
		wk_lcd_active = 0;
		u8_gLcdPowerOnEvent = EVENT_KEY_WAKE_UP;
	}
	u8_gSocAdjustLimit = TRUE;
}

void vUserInterfaceConfigParamRequest(ST_LCD_FORM *pform)
{
	USHORT wk_idx = 0;
	UCHAR wk_send_data[50] = {0};
	ST_LCD_FORM * pstLCDform = (ST_LCD_FORM *)&wk_send_data;

	pstLCDform->head = 0xA5;
	pstLCDform->cmd = pform->cmd;
	pstLCDform->data_value[wk_idx++] = st_gSysParam.refrigerant_type;
	pstLCDform->data_value[wk_idx++] = st_gSysParam.pressure_unit;
	pstLCDform->data_value[wk_idx++] = st_gSysParam.temp_unit;
	pstLCDform->data_value[wk_idx++] = st_gSysParam.shutdown_time;
	pstLCDform->data_value[wk_idx++] = st_gSysParam.lowpower_time;
	pstLCDform->data_value[wk_idx++] = VERSION[0];
	pstLCDform->data_value[wk_idx++] = VERSION[1];
	pstLCDform->data_value[wk_idx++] = VERSION[2];
	
	pstLCDform->data_value[wk_idx++] = 1;
	pstLCDform->data_value[wk_idx++] = 2;
	pstLCDform->data_value[wk_idx++] = 3;
	pstLCDform->data_value[wk_idx++] = 4;
	pstLCDform->data_value[wk_idx++] = 5;
	pstLCDform->data_value[wk_idx++] = 6;
	
//	pstLCDform->data_value[wk_idx++] = u8_aBleMacAddress[0];
//	pstLCDform->data_value[wk_idx++] = u8_aBleMacAddress[1];
//	pstLCDform->data_value[wk_idx++] = u8_aBleMacAddress[2];
//	pstLCDform->data_value[wk_idx++] = u8_aBleMacAddress[3];
//	pstLCDform->data_value[wk_idx++] = u8_aBleMacAddress[4];
//	pstLCDform->data_value[wk_idx++] = u8_aBleMacAddress[5];
	pstLCDform->len = wk_idx;
	pstLCDform->data_value[wk_idx++] = vCheckSumCal(wk_send_data, pstLCDform->len + 4);
	ucBSP_UartSendDataPoll(USARTx_Active,wk_send_data,pstLCDform->len + 5);
}

void vUserInterfaceConfigParamSeting(ST_LCD_FORM *pform)
{
	USHORT wk_idx = 0;
	UCHAR wk_send_data[50] = {0};
	ST_LCD_FORM * pstLCDform = (ST_LCD_FORM *)&wk_send_data;
	UCHAR wk_pre_shutdown_time = st_gSysParam.shutdown_time;
	UCHAR wk_pre_lowpower_time = st_gSysParam.lowpower_time;

	st_gSysParam.refrigerant_type = pform->data_value[wk_idx++];
	st_gSysParam.pressure_unit = pform->data_value[wk_idx++];
	st_gSysParam.temp_unit 	= pform->data_value[wk_idx++];
	st_gSysParam.shutdown_time = pform->data_value[wk_idx++];
	st_gSysParam.lowpower_time = pform->data_value[wk_idx++];
	
	ucFlashWriteData(ADDR_REFIR_TYPE_SELECT,(UCHAR*)&st_gSysParam,sizeof(st_gSysParam));

	if(wk_pre_shutdown_time != st_gSysParam.shutdown_time)
	{
		u32_gPowerDownTimeAdd = 0;
	}
	if(wk_pre_lowpower_time != st_gSysParam.lowpower_time)
	{
		u32_gLowPowerTimeAdd = 0;
	}

	wk_idx = 0;
	pstLCDform->head = 0xA5;
	pstLCDform->cmd = pform->cmd;
	pstLCDform->data_value[wk_idx++] = 1;
	pstLCDform->len = wk_idx;
	pstLCDform->data_value[wk_idx++] = vCheckSumCal(wk_send_data, pstLCDform->len + 4);
	ucBSP_UartSendDataPoll(USARTx_Active,wk_send_data,pstLCDform->len + 5);
	
}

void vUserInterfaceRefriListRequest(ST_LCD_FORM *pform)
{
//	UCHAR wk_idx = 0;
//	UCHAR wk_len;
//	UCHAR wk_refri_name[20] = {0};
	
//	//从flash中读取一条发送一条
//	do
//	{
//		FLASH_If_ReadByte(0,&wk_refri_name[wk_idx++],1)
//	}
//	while (wk_refri_name[wk_idx--] != '\0');

//	wk_len = 
	
}

void vUserInterfacePTRequest(ST_LCD_FORM *pform)
{
	UCHAR wk_hl = pform->data_value[0];
	UCHAR wk_data = 0;
	USHORT wk_idx = 0;
	UCHAR wk_send_data[50] = {0};
	ST_LCD_FORM * pstLCDform = (ST_LCD_FORM *)&wk_send_data;

	pstLCDform->head = 0xA5;
	pstLCDform->cmd = pform->cmd;
	
	if(wk_hl == 2)	//拨码器有操作，重置休眠和关机累积时间
	{
		u32_gLowPowerTimeAdd = 0;
		u32_gPowerDownTimeAdd = 0;
		return;
	}
	else if(wk_hl == 1)	//低压数据
	{
		u8_gLCDParam[1][0] = pform->data_value[1];
		u8_gLCDParam[1][1] = pform->data_value[2];
		u8_gLCDParam[1][2] = pform->data_value[3];
		u8_gLCDParam[1][3] = pform->data_value[4];
		pstLCDform->data_value[wk_idx++] = (st_gHL_Pressure[1].p_0001xbar_report >> 0) & 0xFF;
		pstLCDform->data_value[wk_idx++] = (st_gHL_Pressure[1].p_0001xbar_report >> 8) & 0xFF;
		pstLCDform->data_value[wk_idx++] = (st_gHL_Pressure[1].p_0001xbar_report >> 16) & 0xFF;
		pstLCDform->data_value[wk_idx++] = (st_gHL_Pressure[1].p_0001xbar_report >> 24) & 0xFF;
		pstLCDform->data_value[wk_idx++] = s16_gHL_Temp[1] & 0xFF;
		pstLCDform->data_value[wk_idx++] = (s16_gHL_Temp[1]>>8) & 0xFF;
	}
	else if(wk_hl == 0)	//高压数据
	{
		u8_gLCDParam[0][0] = pform->data_value[1];
		u8_gLCDParam[0][1] = pform->data_value[2];
		u8_gLCDParam[0][2] = pform->data_value[3];
		u8_gLCDParam[0][3] = pform->data_value[4];
		pstLCDform->data_value[wk_idx++] = (st_gHL_Pressure[0].p_0001xbar_report >> 0) & 0xFF;
		pstLCDform->data_value[wk_idx++] = (st_gHL_Pressure[0].p_0001xbar_report >> 8) & 0xFF;
		pstLCDform->data_value[wk_idx++] = (st_gHL_Pressure[0].p_0001xbar_report >> 16) & 0xFF;
		pstLCDform->data_value[wk_idx++] = (st_gHL_Pressure[0].p_0001xbar_report >> 24) & 0xFF;
		pstLCDform->data_value[wk_idx++] = s16_gHL_Temp[0] & 0xFF;
		pstLCDform->data_value[wk_idx++] = (s16_gHL_Temp[0]>>8) & 0xFF;
	}
	//printf("LOW:%ld\r\nHIGH:%ld\r\n",g_adc_low,g_adc_high );
	if(ucPlugInsertStatus == STATUS_PLUG_REMOVE)
	{
		wk_data = 0 << 4;
	}
	else
	{
		if(IN_CHARGING_ERROR())
		{
			wk_data = 3 << 4;
		}
		else if(IN_CHARGING_COMPLETE())
		{
			wk_data = 2 << 4;
		}
		else if(IN_CHARGING())
		{
			wk_data = 1 << 4;
		}
	}		
	wk_data |= u8_gBatSocLevel & 0x0F;
	pstLCDform->data_value[wk_idx++] = wk_data & 0xFF;
	wk_data = (STATE_CHECK_BLE_CONNNECTED == u8_gBleState)?1:0;
	pstLCDform->data_value[wk_idx++] = wk_data;
	pstLCDform->data_value[wk_idx++] = 0;

	pstLCDform->len = wk_idx;
	pstLCDform->data_value[wk_idx++] = vCheckSumCal(wk_send_data, pstLCDform->len + 4);
	ucBSP_UartSendDataPoll(USARTx_Active,wk_send_data,pstLCDform->len + 5);

	u8_gSocAdjustLimit = TRUE;
}


void vUserInterfaceDisablePowerDown(ST_LCD_FORM *pform)
{
	if(pform->data_value[0] == 1)
	{
		u8_PowerDownEnable = FALSE;
	}
	else if(pform->data_value[0] == 0)
	{
		u8_PowerDownEnable = TRUE;
		u32_gPowerDownTimeAdd = 0;
	}
}

void vUserInterfaceEnterLowpower(UCHAR dat)
{
	USHORT wk_idx = 0;
	UCHAR wk_send_data[50] = {0};
	ST_LCD_FORM * pstLCDform = (ST_LCD_FORM *)&wk_send_data;
	
	vBSP_DelayMs(100);
	
	pstLCDform->head = 0xA5;
	pstLCDform->cmd = CMD_LCD_P_T_REQ;
	
	
	pstLCDform->data_value[wk_idx++] = 0;
	pstLCDform->data_value[wk_idx++] = 0;
	pstLCDform->data_value[wk_idx++] = 0;
	pstLCDform->data_value[wk_idx++] = 0;
	pstLCDform->data_value[wk_idx++] = 0;
	pstLCDform->data_value[wk_idx++] = 0;
	pstLCDform->data_value[wk_idx++] = 0;
	pstLCDform->data_value[wk_idx++] = 0;
	pstLCDform->data_value[wk_idx++] = dat;

	pstLCDform->len = wk_idx;
	pstLCDform->data_value[wk_idx++] = vCheckSumCal(wk_send_data, pstLCDform->len + 4);
	ucBSP_UartSendDataPoll(USART_LCD1,wk_send_data,pstLCDform->len + 5);	
	ucBSP_UartSendDataPoll(USART_LCD2,wk_send_data,pstLCDform->len + 5);
	ucBSP_UartSendDataPoll(USART_LCD1,wk_send_data,pstLCDform->len + 5);	
	ucBSP_UartSendDataPoll(USART_LCD2,wk_send_data,pstLCDform->len + 5);	
}


void vDebugInterfacePressureAdcRequest(ST_LCD_FORM *pform)
{
	USHORT wk_idx = 0;
	UCHAR wk_send_data[50] = {0};
	ST_LCD_FORM * pstLCDform = (ST_LCD_FORM *)&wk_send_data;

	extern LONG g_adc_high,g_adc_low;
	pstLCDform->head = 0xA5;
	pstLCDform->cmd = pform->cmd;
	pstLCDform->data_value[wk_idx++] = (g_adc_high >> 0) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (g_adc_high >> 8) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (g_adc_high >> 16) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (g_adc_high >> 24) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (st_gHL_Pressure[0].p_0001xbar >> 0) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (st_gHL_Pressure[0].p_0001xbar >> 8) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (st_gHL_Pressure[0].p_0001xbar >> 16) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (st_gHL_Pressure[0].p_0001xbar >> 24) & 0xFF;

	pstLCDform->data_value[wk_idx++] = (g_adc_low >> 0) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (g_adc_low >> 8) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (g_adc_low >> 16) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (g_adc_low >> 24) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (st_gHL_Pressure[1].p_0001xbar >> 0) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (st_gHL_Pressure[1].p_0001xbar >> 8) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (st_gHL_Pressure[1].p_0001xbar >> 16) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (st_gHL_Pressure[1].p_0001xbar >> 24) & 0xFF;
	pstLCDform->len = wk_idx;
	pstLCDform->data_value[wk_idx++] = vCheckSumCal(wk_send_data, pstLCDform->len + 4);
	ucBSP_UartSendDataPoll(USARTx_Active,wk_send_data,pstLCDform->len + 5);	
}


void vDebugInterfacePressureCalibration(ST_LCD_FORM *pform)
{
	static UCHAR wk_cali_flag = 0;
	static LONG wk_h_0bar_adc_val = 0;
	static LONG wk_h_30bar_adc_val = 0;
	static LONG wk_l_0bar_adc_val = 0;
	static LONG wk_l_30bar_adc_val = 0;
	UCHAR wk_idx;
	UCHAR wk_send_data[50] = {0};
	ST_LCD_FORM * pstLCDform = (ST_LCD_FORM *)&wk_send_data;
	switch(pform->cmd)
	{
		case 0x41:
			for(wk_idx = 0;wk_idx < ADC_BUFF_MAX;wk_idx++)
			{
				g_adc_high = adc_buff_inc(ADC_TYPE_ADS1230_1,vADS1230_Read1());
				g_adc_low = adc_buff_inc(ADC_TYPE_ADS1230_2,vADS1230_Read2());
			}
			wk_cali_flag |= 0x01;
			st_gPressureCaliParam.cali_flag |= 0x01;
			wk_h_0bar_adc_val = g_adc_high;
			wk_l_0bar_adc_val = g_adc_low;
			break;
		case 0x42:
			for(wk_idx = 0;wk_idx < ADC_BUFF_MAX;wk_idx++)
			{
				g_adc_high = adc_buff_inc(ADC_TYPE_ADS1230_1,vADS1230_Read1());
				g_adc_low = adc_buff_inc(ADC_TYPE_ADS1230_2,vADS1230_Read2());
			}
			wk_cali_flag |= 0x02;
			st_gPressureCaliParam.cali_flag |= 0x02;
			wk_h_30bar_adc_val = g_adc_high;
			wk_l_30bar_adc_val = g_adc_low;
			break;
		case 0x45:
			wk_cali_flag |= 0x04;
			//if((st_gHL_Pressure[0].p_mpa > -0.12f) && (st_gHL_Pressure[0].p_mpa < 0.12f))
			{
				st_gPressureCaliParam.pressure_h_offset2 += -st_gHL_Pressure[0].p_0001xbar;
			}
			//if((st_gHL_Pressure[1].p_mpa > -0.12f) && (st_gHL_Pressure[1].p_mpa < 0.12f))
			{
				st_gPressureCaliParam.pressure_l_offset2 += -st_gHL_Pressure[1].p_0001xbar;
			}
			break;
		default:
			break;
	}

	if(wk_cali_flag == (0x01 | 0x02))
	{
		wk_cali_flag = 0;
		st_gPressureCaliParam.pressure_h_factor = (double)30000 / (wk_h_30bar_adc_val - wk_h_0bar_adc_val);
		st_gPressureCaliParam.pressure_h_offset1 = -((double)wk_h_0bar_adc_val * st_gPressureCaliParam.pressure_h_factor);
		st_gPressureCaliParam.pressure_l_factor = (double)30000 / (wk_l_30bar_adc_val - wk_l_0bar_adc_val);
		st_gPressureCaliParam.pressure_l_offset1 = -((double)wk_l_0bar_adc_val * st_gPressureCaliParam.pressure_l_factor);

		ucFlashWriteData(ADDR_HIGHT_PRESSURE_PARAM,(UCHAR*)&st_gPressureCaliParam,sizeof(ST_PRESSURE_CALI));
		
		wk_h_0bar_adc_val = 0;
		wk_h_30bar_adc_val = 0;
		wk_l_0bar_adc_val = 0;
		wk_l_30bar_adc_val = 0;
	}
	else if(wk_cali_flag & 0x04)	//去皮
	{
		//st_gPressureCaliParam.cali_flag |= 0x02;
		//vw25q64WriteData
	}
	wk_idx = 0;
	pstLCDform->head = 0xA5;
	pstLCDform->cmd = pform->cmd;
	pstLCDform->data_value[wk_idx++] = (g_adc_high >> 0) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (g_adc_high >> 8) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (g_adc_high >> 16) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (g_adc_high >> 24) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (g_adc_low >> 0) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (g_adc_low >> 8) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (g_adc_low >> 16) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (g_adc_low >> 24) & 0xFF;
	pstLCDform->len = wk_idx;
	pstLCDform->data_value[wk_idx++] = vCheckSumCal(wk_send_data, pstLCDform->len + 4);
	ucBSP_UartSendDataPoll(USARTx_Active,wk_send_data,pstLCDform->len + 5); 
}

void vDebugInterfaceCaliParamRead(ST_LCD_FORM *pform)
{
	UCHAR wk_idx = 0;
	float wk_data;
	ULONG wk_report_data;
	UCHAR wk_send_data[50] = {0};
	ST_LCD_FORM * pstLCDform = (ST_LCD_FORM *)&wk_send_data;
	
	pstLCDform->head = 0xA5;
	pstLCDform->cmd = pform->cmd;
	wk_data = st_gPressureCaliParam.pressure_h_factor;
	wk_report_data = *((ULONG*)&wk_data);
	pstLCDform->data_value[wk_idx++] = (wk_report_data >> 0) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (wk_report_data >> 8) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (wk_report_data >> 16) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (wk_report_data >> 24) & 0xFF;
	wk_data = st_gPressureCaliParam.pressure_h_offset1;
	wk_report_data = *((ULONG*)&wk_data);
	pstLCDform->data_value[wk_idx++] = (wk_report_data >> 0) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (wk_report_data >> 8) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (wk_report_data >> 16) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (wk_report_data >> 24) & 0xFF;
	wk_data = st_gPressureCaliParam.pressure_h_offset2;
	wk_report_data = *((ULONG*)&wk_data);
	pstLCDform->data_value[wk_idx++] = (wk_report_data >> 0) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (wk_report_data >> 8) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (wk_report_data >> 16) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (wk_report_data >> 24) & 0xFF;
	wk_data = st_gPressureCaliParam.pressure_l_factor;
	wk_report_data = *((ULONG*)&wk_data);
	pstLCDform->data_value[wk_idx++] = (wk_report_data >> 0) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (wk_report_data >> 8) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (wk_report_data >> 16) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (wk_report_data >> 24) & 0xFF;
	wk_data = st_gPressureCaliParam.pressure_l_offset1;
	wk_report_data = *((ULONG*)&wk_data);
	pstLCDform->data_value[wk_idx++] = (wk_report_data >> 0) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (wk_report_data >> 8) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (wk_report_data >> 16) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (wk_report_data >> 24) & 0xFF;
	wk_data = st_gPressureCaliParam.pressure_l_offset2;
	wk_report_data = *((ULONG*)&wk_data);
	pstLCDform->data_value[wk_idx++] = (wk_report_data >> 0) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (wk_report_data >> 8) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (wk_report_data >> 16) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (wk_report_data >> 24) & 0xFF;
	pstLCDform->data_value[wk_idx++] = st_gPressureCaliParam.cali_flag;
	pstLCDform->len = wk_idx;
	pstLCDform->data_value[wk_idx++] = vCheckSumCal(wk_send_data, pstLCDform->len + 4);
	ucBSP_UartSendDataPoll(USARTx_Active,wk_send_data,pstLCDform->len + 5); 
}

void vDebugInterfaceN32StatusRead(ST_LCD_FORM *pform)
{
	UCHAR wk_idx = 0;
	UCHAR wk_send_data[50] = {0};
	ST_LCD_FORM * pstLCDform = (ST_LCD_FORM *)&wk_send_data;
		
	pstLCDform->head = 0xA5;
	pstLCDform->cmd = pform->cmd;
	pstLCDform->data_value[wk_idx++] = (s16_gBatVoltage >> 0) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (s16_gBatVoltage >> 8) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (s16_gBatTemp >> 0) & 0xFF;
	pstLCDform->data_value[wk_idx++] = (s16_gBatTemp >> 8) & 0xFF;
	pstLCDform->len = wk_idx;
	pstLCDform->data_value[wk_idx++] = vCheckSumCal(wk_send_data, pstLCDform->len + 4);
	ucBSP_UartSendDataPoll(USARTx_Active,wk_send_data,pstLCDform->len + 5); 
}

void vDebugInterfacePowerDownCtrl(ST_LCD_FORM *pform)
{
	u8_SysPowerDownCmd = TRUE;
}
void vDebugInterfaceComLostCheckCtrl(ST_LCD_FORM *pform)
{
	UCHAR wk_idx = 0;
	UCHAR wk_send_data[50] = {0};
	ST_LCD_FORM * pstLCDform = (ST_LCD_FORM *)&wk_send_data;

	u8_gComLostEnable = !!pform->data_value[0];
	
	pstLCDform->head = 0xA5;
	pstLCDform->cmd = pform->cmd;
	pstLCDform->data_value[wk_idx++] = 0x01;
	pstLCDform->len = wk_idx;
	pstLCDform->data_value[wk_idx++] = vCheckSumCal(wk_send_data, pstLCDform->len + 4);
	ucBSP_UartSendDataPoll(USARTx_Active,wk_send_data,pstLCDform->len + 5); 
}

void vComLostOtCallback(UCHAR period)
{
	if(u8_gSysState == SYS_STATE_NORMAL)
	{
		if(u16_ComLostOt1 < COM_LOST_TIME)
		{
			u16_ComLostOt1+=period;
		}
		if(u16_ComLostOt2 < COM_LOST_TIME)
		{
			u16_ComLostOt2+=period;
		}
	}
	else
	{
		u16_ComLostOt1 = 0;
		u16_ComLostOt2 = 0;
	}
}





//void vUpdataLcd(void)
//{
//	static USHORT wk_preiod = 500;

//	wk_preiod+=50;
//	if(wk_preiod >= 100)
//	{
//		wk_preiod = 0;
//		vSendCmdToLcd(CMD_LCD_HIGH_LOW_PARAM);// CMD_LCD_HIGH_LOW_ADC
//	}
//}

//void vSlaveEventMain(void)
//{
//	if(u8_gSlaveParamIsChanged)
//	{
//		if(ucGetSlaveParam())
//		{
//			u8_gSlaveParamIsChanged = FALSE;
//		}
//	}



//	UCHAR wk_data[100];
//	UCHAR usDatSize;
//	UCHAR wk_idx = 0;
//	UCHAR wk_cmd[] = {0x55,0X00,ADDR_LDC_S,CMD_LCD_REQ_SLAVE_PARAM,0x5A};	//配置参数保存在从屏幕里
//	ST_LCD_FORM * pstLCDform = (ST_LCD_FORM *)wk_data;		

//	ucBSP_UartSendDataPoll(USART_LCD1,wk_cmd,5);
	
//	vTIM_1msStart(100);
//	while(!vTIM_1msIsTimeout())
//	{
//		if ( st_gUsartLCD.ucPacketCnt > 0)
//		{
//			usDatSize = st_gUsartLCD1.usRecvPacketSize[wk_idx];
//			memcpy(wk_data,&st_gUsartLCD.ucRecvPacketPool[wk_idx][0],usDatSize);
//			if(ucLCDFrameCheck(pstLCDform,usDatSize))
//			{
//				wk_idx = 0;
//				st_gSysParam.refrigerant_type 	= pstLCDform->data_value[wk_idx++];
//				st_gSysParam.pressure_unit 		= pstLCDform->data_value[wk_idx++];
//				st_gSysParam.temp_unit 			= pstLCDform->data_value[wk_idx++];
//				st_gSysParam.shutdown_time 		= pstLCDform->data_value[wk_idx++];
//				st_gSysParam.lowpower_time 		= pstLCDform->data_value[wk_idx++];	
//				vTIM_1msStart(0);
//				return TRUE;
//			}
//			else
//			{
//				vTIM_1msStart(0);
//				return FALSE;
//			}
//		}
//	}

//	vTIM_1msStart(0);
	//return FALSE;
//}

//extern LONG g_adc_high,g_adc_low;

//void vSendCmdToLcd(UCHAR cmd)
//{
//	USHORT wk_idx = 0;
//	USHORT wk_data = 0;
//	UCHAR wk_send_data[50] = {0};
//	ST_LCD_FORM * pstLCDform = (ST_LCD_FORM *)&wk_send_data;	

//	pstLCDform->head = 0xA5;
//	pstLCDform->cmd = cmd;
//	if(CMD_LCD_HIGH_LOW_PARAM == cmd)
//	{
//		//高压数据
//		pstLCDform->data_value[wk_idx++] = st_gHL_Pressure[0].p_0001xbar & 0xFF;
//		pstLCDform->data_value[wk_idx++] = (st_gHL_Pressure[0].p_0001xbar>>8) & 0xFF;
//		pstLCDform->data_value[wk_idx++] = s16_gHL_Temp[0] & 0xFF;
//		pstLCDform->data_value[wk_idx++] = (s16_gHL_Temp[0]>>8) & 0xFF;
//		wk_data = (IN_CHARGING())?(0x01<<4):wk_data;
//		wk_data = (IN_CHARGING_COMPLETE())?(0x02<<4):wk_data;
//		wk_data |= u8_gBatSocLevel & 0x0F;
//		pstLCDform->data_value[wk_idx++] = wk_data & 0xFF;
//		pstLCDform->len = wk_idx;
//		pstLCDform->data_value[wk_idx++] = vCheckSumCal(wk_send_data, pstLCDform->len + 4);
//		//if(st_gHL_Pressure[0].p_0001xbar <= 8000 && st_gHL_Pressure[0].p_0001xbar >=-30)
//			//ucBSP_UartSendDataPoll(USART_LCD1,wk_send_data,pstLCDform->len + 5);

//		//低压数据
//		wk_idx = 0;
//		pstLCDform->data_value[wk_idx++] = st_gHL_Pressure[1].p_0001xbar & 0xFF;
//		pstLCDform->data_value[wk_idx++] = (st_gHL_Pressure[1].p_0001xbar>>8) & 0xFF;
//		pstLCDform->data_value[wk_idx++] = s16_gHL_Temp[1] & 0xFF;
//		pstLCDform->data_value[wk_idx++] = (s16_gHL_Temp[1]>>8) & 0xFF;
//		wk_data = (STATE_CHECK_BLE_CONNNECTED == u8_gBleState)?1:0;
//		pstLCDform->data_value[wk_idx++] = wk_data & 0xFF;
//		pstLCDform->len = wk_idx;
//		pstLCDform->data_value[wk_idx++] = vCheckSumCal(wk_send_data, pstLCDform->len + 4);
//		if(st_gHL_Pressure[1].p_0001xbar <= 8000 && st_gHL_Pressure[1].p_0001xbar >=-30)
//		{
//			ucBSP_UartSendDataPoll(USART_LCD2,wk_send_data,pstLCDform->len + 5);
//		}
			
//	}
//	else if(CMD_LCD_HIGH_LOW_ADC == cmd)
//	{
//		//高压数据
//		pstLCDform->data_value[wk_idx++] = g_adc_high & 0xFF;
//		pstLCDform->data_value[wk_idx++] = (g_adc_high>>8) & 0xFF;
//		pstLCDform->data_value[wk_idx++] = (g_adc_high>>16) & 0xFF;
//		pstLCDform->data_value[wk_idx++] = (g_adc_high>>24) & 0xFF;
//		pstLCDform->len = wk_idx;
//		pstLCDform->data_value[wk_idx++] = vCheckSumCal(wk_send_data, pstLCDform->len + 4);
//		ucBSP_UartSendDataPoll(USART_LCD1,wk_send_data,pstLCDform->len + 5);

//		//低压数据
//		wk_idx = 0;
//		pstLCDform->data_value[wk_idx++] = g_adc_low & 0xFF;
//		pstLCDform->data_value[wk_idx++] = (g_adc_low>>8) & 0xFF;
//		pstLCDform->data_value[wk_idx++] = (g_adc_low>>16) & 0xFF;
//		pstLCDform->data_value[wk_idx++] = (g_adc_low>>24) & 0xFF;
//		pstLCDform->len = wk_idx;
//		pstLCDform->data_value[wk_idx++] = vCheckSumCal(wk_send_data, pstLCDform->len + 4);
//		ucBSP_UartSendDataPoll(USART_LCD2,wk_send_data,pstLCDform->len + 5);
//	}
//	else if(CMD_LCD_COMMON_PARAM == cmd)
//	{
//		pstLCDform->data_value[wk_idx++] = st_gSysParam.refrigerant_type;
//		pstLCDform->data_value[wk_idx++] = st_gSysParam.pressure_unit;
//		pstLCDform->data_value[wk_idx++] = st_gSysParam.temp_unit;
//		pstLCDform->data_value[wk_idx++] = st_gSysParam.shutdown_time;
//		pstLCDform->data_value[wk_idx++] = st_gSysParam.lowpower_time;
//		pstLCDform->data_value[wk_idx++] = VERSION[0];
//		pstLCDform->data_value[wk_idx++] = VERSION[1];
//		pstLCDform->data_value[wk_idx++] = VERSION[2];
//		pstLCDform->data_value[wk_idx++] = u8_aBleMacAddress[0];
//		pstLCDform->data_value[wk_idx++] = u8_aBleMacAddress[1];
//		pstLCDform->data_value[wk_idx++] = u8_aBleMacAddress[2];
//		pstLCDform->data_value[wk_idx++] = u8_aBleMacAddress[3];
//		pstLCDform->data_value[wk_idx++] = u8_aBleMacAddress[4];
//		pstLCDform->data_value[wk_idx++] = u8_aBleMacAddress[5];
//		pstLCDform->len = wk_idx;
//		pstLCDform->data_value[wk_idx++] = vCheckSumCal(wk_send_data, pstLCDform->len + 4);
//		ucBSP_UartSendDataPoll(USART_LCD1,wk_send_data,pstLCDform->len + 5);
//	}
	
//}

//UCHAR ucShakeHandsAndReady(void)
//{
//	static UCHAR wk_step = 0;
//	static UCHAR wk_ack1 = FALSE;
//	static UCHAR wk_ack2 = FALSE;
//	UCHAR wk_ret = 2;	//0:FALSE 1:TRUE 2:BUSY
//    USHORT usPacketSize = 0;
//	ST_LCD_FORM * pstLCDform = (ST_LCD_FORM *)aucRxMsgPool;		//for lcd
//	UCHAR wk_cmd1[] = {0x55,0x01,0x00,CMD_LCD_SHANKHANDS,0x00,0x56};
//	UCHAR wk_cmd2[] = {0x55,0x01,0x00,CMD_LCD_SHANKHANDS,0x01,0x57};

//	switch(wk_step)
//	{
//		case 0:
//			//step1 ：检测从机是否开机
//			wk_ack1 = FALSE;
//			wk_ack2 = FALSE;
//			st_gUsartLCD1.usRecvIdx = 0;
//			st_gUsartLCD1.RevComplete = FALSE;
//			st_gUsartLCD2.usRecvIdx = 0;
//			st_gUsartLCD2.RevComplete = FALSE;
//			ucBSP_UartSendDataPoll(USART_LCD1,wk_cmd1,6);
//			ucBSP_UartSendDataPoll(USART_LCD2,wk_cmd1,6);
//			vTIM_1msStart(5000);
//			wk_step++;
//			wk_ret = 2;
//			break;
//		case 1:
//			if(!vTIM_1msIsTimeout())
//			{
//				vBSP_WDG_Refresh();
//				if(ucBSP_UartReceive(USART_LCD1,aucRxMsgPool, &usPacketSize) == TRUE)
//				{
//					if(ucLCDFrameCheck(pstLCDform,usPacketSize) == TRUE)
//					{
//						wk_ack1 = ucInterfaceOfLCD(pstLCDform);
//					}
//				}
//				if(ucBSP_UartReceive(USART_LCD2,aucRxMsgPool, &usPacketSize) == TRUE)
//				{
//					if(ucLCDFrameCheck(pstLCDform,usPacketSize) == TRUE)
//					{
//						wk_ack2 = ucInterfaceOfLCD(pstLCDform);
//					}
//				}
//				if(wk_ack1 & wk_ack2)
//				{
//					wk_step++;
//				}
//			}
//			else
//			{
//				wk_ret = FALSE;
//				wk_step = 0;
//			}
//			break;
			
//		case 2:
//			wk_ack1 = FALSE;
//			wk_ack2 = FALSE;
//			ucBSP_UartSendDataPoll(USART_LCD1,wk_cmd2,6);
//			ucBSP_UartSendDataPoll(USART_LCD2,wk_cmd2,6);
//			vTIM_1msStart(5000);
//			wk_step++;
//			wk_ret = 2;
//			break;
//		case 3:
//			if(!vTIM_1msIsTimeout())
//			{
//				vBSP_WDG_Refresh();
//				if(ucBSP_UartReceive(USART_LCD1,aucRxMsgPool, &usPacketSize) == TRUE)
//				{
//					if(ucLCDFrameCheck(pstLCDform,usPacketSize) == TRUE)
//					{
//						wk_ack1 = ucInterfaceOfLCD(pstLCDform);
//					}
//				}
//				if(ucBSP_UartReceive(USART_LCD2,aucRxMsgPool, &usPacketSize) == TRUE)
//				{
//					if(ucLCDFrameCheck(pstLCDform,usPacketSize) == TRUE)
//					{
//						wk_ack2 = ucInterfaceOfLCD(pstLCDform);
//					}
//				}
//				if(wk_ack1 & wk_ack2)
//				{
//					wk_step = 0;
//					wk_ret = TRUE;
//				}
//			}
//			else
//			{
//				wk_ret = FALSE;
//				wk_step = 0;
//			}
//			break;
//		default:
//			break;
//	}
//	return wk_ret;

//	//return (wk_ret1 & wk_ret2);
//}


//UCHAR ucInitLCDParam(void)
//{
//	static UCHAR wk_step = 0;
//	static UCHAR wk_ack = FALSE;
//	//static UCHAR wk_ret2 = FALSE;
//	UCHAR wk_ret = 2;	//0:FALSE 1:TRUE 2:BUSY
//    USHORT usPacketSize = 0;
//	ST_LCD_FORM * pstLCDform = (ST_LCD_FORM *)aucRxMsgPool;		//for lcd



//	switch(wk_step)
//	{
//		case 0:
//			//step1 ：将参数下发给从机
//			wk_ack = FALSE;
//			st_gUsartLCD1.usRecvIdx = 0;
//			st_gUsartLCD1.RevComplete = FALSE;
//			vSendCmdToLcd(CMD_LCD_COMMON_PARAM);
//			vTIM_1msStart(5000);
//			wk_step++;
//			wk_ret = 2;
//			break;
//		case 1:
//			if(!vTIM_1msIsTimeout())
//			{
//				vBSP_WDG_Refresh();
//				if(ucBSP_UartReceive(USART_LCD1,aucRxMsgPool, &usPacketSize) == TRUE)
//				{
//					if(ucLCDFrameCheck(pstLCDform,usPacketSize) == TRUE)
//					{
//						wk_ack = ucInterfaceOfLCD(pstLCDform);
//					}
//				}
//				if(wk_ack)
//				{
//					wk_step = 0;
//					wk_ret = TRUE;
//				}
//			}
//			else
//			{
//				wk_step = 0;
//				wk_ret = FALSE;
//			}
//			break;
//	}

//	return (wk_ret);


////	st_gUsartLCD1.usRecvIdx = 0;
////	st_gUsartLCD1.RevComplete = FALSE;

////	//step 将参数下发给从机
////	wk_ret1 = 0;
////	vSendCmdToLcd(CMD_LCD_COMMON_PARAM);
////	vTIM_1msStart(5000);
////	while(!vTIM_1msIsTimeout())
////	{
////		vBSP_WDG_Refresh();
////		if(ucBSP_UartReceive(USART_LCD1,aucRxMsgPool, &usPacketSize) == TRUE)
////		{
////			if(ucLCDFrameCheck(pstLCDform,usPacketSize) == TRUE)
////			{
////				wk_ret1 = ucInterfaceOfLCD(pstLCDform);
////			}
////		}
////		if(wk_ret1)
////		{
////			break;
////		}
////	}
	
//	//step2 将冷媒表下发给从机
	
	

//	//return (wk_ret1);
//}

//UCHAR ucGetSlaveParam(void)
//{
//	UCHAR wk_ret = FALSE;
//	static UCHAR wk_step = 0;
//    USHORT usPacketSize = 0;
//	ST_LCD_FORM * pstLCDform = (ST_LCD_FORM *)aucRxMsgPool;		//for lcd
//	UCHAR wk_cmd[] = {0x55,0x00,0x00,CMD_LCD_REQ_SLAVE_PARAM,0x65};

//	switch(wk_step)
//	{
//		case 0:
//			ucBSP_UartSendDataPoll(USART_LCD1,wk_cmd,5);
//			vTIM_1msStart(5000);
//			wk_step++;
//			//break;
//		case 1:
//			if(!vTIM_1msIsTimeout())
//			{
//				vBSP_WDG_Refresh();
//				if(ucBSP_UartReceive(USART_LCD1,aucRxMsgPool, &usPacketSize) == TRUE)
//				{
//					if(ucLCDFrameCheck(pstLCDform,usPacketSize) == TRUE)
//					{
//						wk_ret = ucInterfaceOfLCD(pstLCDform);
//						wk_step = 0;
//					}
//				}
//			}
//			else
//			{
//				wk_step = 0;
//			}
//			break;
//		default:
//			break;
//	}

//	return wk_ret;
//}
