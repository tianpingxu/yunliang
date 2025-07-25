#ifndef		APP_USER_H
#define		APP_USER_H

#include "bsp_usart.h"
#include "app_system.h"

#define COM_LOST_TIME	(5000)


#define CMD_LCD_SHANKHANDS				(0x05)
#define CMD_LCD_CONFIG_PARAM_REQ		(0X10)
#define CMD_LCD_CONFIG_PARAM_SET		(0X11)
#define CMD_LCD_REFRI_LIST_REQ				(0X20)
#define CMD_LCD_P_T_REQ					(0X30)

#define CMD_LCD_REQ_SLAVE_PARAM		(0x10)



#define ADDR_MASTER		(0)	//n32主控
#define ADDR_LDC_M		(0X01)
#define ADDR_LDC_S		(0X02)
#define ADDR_LDC_MS		(ADDR_LDC_M | ADDR_LDC_S)

#pragma pack (1)
typedef struct
{
	UCHAR head;
	USHORT len;
	//UCHAR dst_addr;
	UCHAR cmd;
	UCHAR data_value[500];
}ST_LCD_FORM;
#pragma pack ()

extern UCHAR u8_gSlaveIsError;
extern USHORT u16_ComLostOt1;
extern USHORT u16_ComLostOt2;
extern UCHAR u8_gComLostEnable;


void vSlaveEventMain(void);
void vUpdataLcd(void);
UCHAR ucUserInterfaceOfLCD(void);
UCHAR ucInterfaceOfLCD(ST_LCD_FORM *pform);
UCHAR ucShakeHandsAndReady(void);
UCHAR ucInitLCDParam(void);
UCHAR ucGetSlaveParam(void);
void vUserInterfaceEnterLowpower(UCHAR dat);

#endif
