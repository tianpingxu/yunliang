/****************************************************************************
#ifdef DOC

File Name		:bsp_io.h

Description		:

Remark			:

Date			:19/05/17


#endif
*****************************************************************************/
#ifndef		BSP_IO_H
#define		BSP_IO_H


/****************************************************************************
 define macro
*****************************************************************************/
//#define P_AVCC_EN(value) 			GPIO_WriteBit(GPIOB, GPIO_PIN_2,(value == LOW) ? Bit_RESET : Bit_SET)
#define P_SYSTEM_POWER_CTRL(value) 		GPIO_WriteBit(GPIOB, GPIO_PIN_2,(value == LOW) ? Bit_RESET : Bit_SET)
#define P_LCD_POWER_CTRL(value) 			GPIO_WriteBit(GPIOB, GPIO_PIN_15,(value == LOW) ? Bit_RESET : Bit_SET)
#define P_CHARGE_ENABLE_CTRL(value) 			GPIO_WriteBit(GPIOA, GPIO_PIN_12,(value == ON) ? Bit_RESET : Bit_SET)
#define P_LED1_CTRL(value) 				GPIO_WriteBit(GPIOB, GPIO_PIN_6,(value == LOW) ? Bit_RESET : Bit_SET)
#define P_LED2_CTRL(value) 				GPIO_WriteBit(GPIOB, GPIO_PIN_7,(value == LOW) ? Bit_RESET : Bit_SET)

#define P_ENCODE_IS_PRESS() 		((GPIO_ReadInputDataBit(GPIOB, GPIO_PIN_5) == Bit_RESET) ? TRUE : FALSE)
#define P_PWR_KEY_IS_PRESS() 		((GPIO_ReadInputDataBit(GPIOA, GPIO_PIN_8) == Bit_RESET) ? TRUE : FALSE)
#define P_IN_CHARGING() 				((GPIO_ReadInputDataBit(GPIOB, GPIO_PIN_0) == Bit_RESET) ? TRUE : FALSE)
#define P_CHARGING_COMPLETE() 		((GPIO_ReadInputDataBit(GPIOB, GPIO_PIN_0) == Bit_RESET) ? TRUE : FALSE)
#define P_PLUG_IS_INSERT() 			((GPIO_ReadInputDataBit(GPIOA, GPIO_PIN_11) == Bit_RESET) ? TRUE : FALSE)

#define P_HALL_A_IS_PRESS() 		((GPIO_ReadInputDataBit(GPIOB, GPIO_PIN_15) == Bit_RESET) ? TRUE : FALSE)
#define P_HALL_B_IS_PRESS() 		((GPIO_ReadInputDataBit(GPIOB, GPIO_PIN_14) == Bit_RESET) ? TRUE : FALSE)

#define BEEP_ON() 			GPIO_SetBits(GPIOA, GPIO_PIN_0);
#define BEEP_OFF() 			GPIO_ResetBits(GPIOA, GPIO_PIN_0);


/****************************************************************************
 define type (STRUCTURE, UNION)
*****************************************************************************/
/****************************************************************************
 define type (Enumerated type ENUM)
*****************************************************************************/

/****************************************************************************
 Variable declaration
*****************************************************************************/

/****ot************************************************************************
 Protype declaration
*****************************************************************************/
void vBSP_GpioDeinit(GPIO_Module* GPIOx, USHORT Pin);
void  vBSP_Port_FullSpeed(void);

#endif	/* CB_H */

