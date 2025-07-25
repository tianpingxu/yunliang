#ifndef BLE_E95_H
#define BLE_E95_H

#include "bsp_usart.h"

#define BLE_UART_SEND_DATA(pdata,len)	ucBSP_UartSendDataPoll(USART_BLE,(pdata),(len))
#define BLE_UART_REV_COMPLETE			st_gUsartBLE.RevComplete//_gUartPram.Uart1RevComplete
#define BLE_UART_REV_OVER_TIME			st_gUsartBLE.ucRecvTimeout//st_gUartPram.Uart1RevOverTime
#define BLE_UART_REV_IDX				st_gUsartBLE.usRecvIdx//st_gUartPram.Uart1RevIdx
#define BLE_UART_REV_BUFF				st_gUsartBLE.pRecvDataPool//st_gUartPram.Uart1RevIdx
#define BLE_UART_REV_BUFF_LEN			sizeof(u8_gUartRevPool1)

#define SEND_MODE_CMD			(0)
#define SEND_MODE_TOUCHUAN		(1)

#define BLE_MODE_CTRL(x)	GPIO_WriteBit(GPIOC, GPIO_PIN_13,(x == LOW) ? Bit_RESET : Bit_SET)/*0:指令模式  1:透传模式*/
#define BLE_POWER_CTRL(x)	GPIO_WriteBit(GPIOB, GPIO_PIN_9,(x == LOW) ? Bit_RESET : Bit_SET)/*0:唤醒模式  1:休眠模式*/
#define BLE_BT_CTRL(x)		GPIO_WriteBit(GPIOB, GPIO_PIN_8,(x == LOW) ? Bit_RESET : Bit_SET)/*0:开始广播  1:关闭蓝牙功能*/
#define BLE_RESET_CTRL(x)	GPIO_WriteBit(GPIOA, GPIO_PIN_15,(x == LOW) ? Bit_RESET : Bit_SET)

#define BLE_IS_POWER_ON()	(ucBLE_CheckPowerOn() == TRUE)//((P5_7 == 0)?1:0)
#define BLE_IS_CMD_MODE()	(ucBLE_CheckMode() == TRUE)//((P6_0 == 0)?1:0)
#define BLE_IS_CONNECTED()	(ucBLE_CheckConnect() == TRUE)//((P8_6 == 0)?1:0)
#define BLE_IS_BUSY()		(ucBLE_CheckBusy() == TRUE)//((P8_7 == 0)?1:0)
#define BLE_IS_ACTIVE()		(ucBLE_CheckActive() == TRUE)//((P6_1 == 0)?TRUE:FALSE)

#define UUID_ITEM_TRANSMITTAL_SERVICE	0x01
#define UUID_ITEM_TRANSMITTAL_RECEIVE	0x02	/*E95<--APP*/
#define UUID_ITEM_TRANSMITTAL_SEND		0x03	/*E95-->APP*/


#define STATE_SET_BLE_OFF				0
#define STATE_SET_BLE_ON				1
#define STATE_SET_BLE_BROADCAST			2

#define STATE_CHECK_BLE_IDLE			0
#define STATE_CHECK_BLE_BROADCASTING	1
#define STATE_CHECK_BLE_CONNNECTED		2
#define STATE_CHECK_BLE_UNKNOW			0xFF



extern UCHAR u8_gBleState;
extern UCHAR u8_aBleMacAddress[12];
extern UCHAR u8_gBleModuleOffLineWithMcu;


void ucBLE_E95_Notify_Data(CHAR *pData,UCHAR len);
void vBLE_E95_Init(void);
void vBLE_E95_State_Check(UCHAR *ble_state);
void vBLE_E95_BroadcastName_Set(CHAR * name);
void vBLE_E95_State_Set(UCHAR state);
void vBLE_E95_BroadcastDataSet(CHAR *broadcast_data,UCHAR data_len,UCHAR save_en);
void vBLE_E95_SendData(UCHAR *dat,UCHAR len,UCHAR mode);	//0:命令模式  1:透传
UCHAR ucBLE_CheckActive(void);

#endif
