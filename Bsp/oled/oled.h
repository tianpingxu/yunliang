#ifndef __OLED_H__
#define __OLED_H__

#include "n32l40x.h"
#include <string.h>


//#define OLED_SCL_CLR() GPIO_ResetBits(GPIOA, GPIO_PIN_5) // 时钟
//#define OLED_SCL_SET() GPIO_SetBits(GPIOA, GPIO_PIN_5)

//#define OLED_SDA_LOW() GPIO_ResetBits(GPIOA, GPIO_PIN_7) // MOSI主设备输出
//#define OLED_SDA_HIGH() GPIO_SetBits(GPIOA, GPIO_PIN_7)

#define OLED_RST_OFF() GPIO_ResetBits(GPIOC, GPIO_PIN_14) // 接低电平复位
#define OLED_RST_ON() GPIO_SetBits(GPIOC, GPIO_PIN_14)

#define OLED_DC_CMD() GPIO_ResetBits(GPIOC, GPIO_PIN_15) // 模式
#define OLED_DC_DAT() GPIO_SetBits(GPIOC, GPIO_PIN_15)


void OLED_Init(void);
void OLED_Reg_Init(void);
void OLED_SendCmd(u8 TxData);

void OLED_ColorTurn(uint8_t i);
void OLED_DisplayTurn(uint8_t i);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);

void OLED_Clear(uint8_t led);

void OLED_DrawPoint(uint8_t x, uint8_t y);
void OLED_ClearPoint(uint8_t x, uint8_t y);

void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void OLED_ClearLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

void OLED_DrawCircle(uint8_t x, uint8_t y, uint8_t r);
void OLED_ClearCircle(uint8_t x, uint8_t y, uint8_t r);

void OLED_ZF(uint8_t x, uint8_t y, uint8_t *ascii, uint8_t Font_Size);
void OLED_ZFC(uint8_t x, uint8_t y, uint8_t *str, uint8_t Font_Size);
void OLED_ZFC_Horizontal_center(uint8_t y, uint8_t *str, uint8_t Font_Size);
void OLED_ZFC_Vorizontal_center(uint8_t x, uint8_t *str, uint8_t Font_Size);

void OLED_ONE_HZ(uint8_t x, uint8_t y, uint8_t page, uint8_t num);
void OLED_Long_HZ(uint8_t x, uint8_t y, uint8_t page, uint8_t num1, uint8_t num2);
void OLED_Long_HZ_Horizontal_center(uint8_t y, uint8_t page, uint8_t num1, uint8_t num2);
void OLED_Long_HZ_Vorizontal_center(uint8_t x, uint8_t page, uint8_t num1, uint8_t num2);
void OLED_ShowPicture(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t BMP);

#endif

