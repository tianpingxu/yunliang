#ifndef __CMT_SPI3_H
#define __CMT_SPI3_H

#include "n32l40x.h"
#include "typedefs.h"
#include "delay.h"

#define CMT_CSB_GPIO            GPIOB
#define CMT_CSB_GPIO_PIN        GPIO_PIN_4

#define CMT_FCSB_GPIO           GPIOB
#define CMT_FCSB_GPIO_PIN       GPIO_PIN_3

#define CMT_SCL_GPIO            GPIOB
#define CMT_SCL_GPIO_PIN        GPIO_PIN_6

#define CMT_SDA_GPIO            GPIOB
#define CMT_SDA_GPIO_PIN        GPIO_PIN_5

#define cmt_spi3_csb_out()      GPIO_Pin_Setting(CMT_CSB_GPIO, CMT_CSB_GPIO_PIN, GPIO_Slew_Rate_High, GPIO_Mode_Out_PP)
#define cmt_spi3_fcsb_out()     GPIO_Pin_Setting(CMT_FCSB_GPIO, CMT_FCSB_GPIO_PIN, GPIO_Slew_Rate_High, GPIO_Mode_Out_PP)
#define cmt_spi3_scl_out()      GPIO_Pin_Setting(CMT_SCL_GPIO, CMT_SCL_GPIO_PIN, GPIO_Slew_Rate_High, GPIO_Mode_Out_PP)
#define cmt_spi3_sda_out()      GPIO_Pin_Setting(CMT_SDA_GPIO, CMT_SDA_GPIO_PIN, GPIO_Slew_Rate_High, GPIO_Mode_Out_PP)
#define cmt_spi3_sda_in()       GPIO_Pin_Setting(CMT_SDA_GPIO, CMT_SDA_GPIO_PIN, GPIO_Slew_Rate_High, GPIO_Mode_Input)

#define cmt_spi3_csb_1()        GPIO_SetBits(CMT_CSB_GPIO, CMT_CSB_GPIO_PIN)
#define cmt_spi3_csb_0()        GPIO_ResetBits(CMT_CSB_GPIO, CMT_CSB_GPIO_PIN)

#define cmt_spi3_fcsb_1()       GPIO_SetBits(CMT_FCSB_GPIO, CMT_FCSB_GPIO_PIN)
#define cmt_spi3_fcsb_0()       GPIO_ResetBits(CMT_FCSB_GPIO, CMT_FCSB_GPIO_PIN)
    
#define cmt_spi3_scl_1()        GPIO_SetBits(CMT_SCL_GPIO, CMT_SCL_GPIO_PIN)
#define cmt_spi3_scl_0()        GPIO_ResetBits(CMT_SCL_GPIO, CMT_SCL_GPIO_PIN)

#define cmt_spi3_sda_1()        GPIO_SetBits(CMT_SDA_GPIO, CMT_SDA_GPIO_PIN)
#define cmt_spi3_sda_0()        GPIO_ResetBits(CMT_SDA_GPIO, CMT_SDA_GPIO_PIN)
#define cmt_spi3_sda_read()     GPIO_ReadInputDataBit(CMT_SDA_GPIO, CMT_SDA_GPIO_PIN)

#define Cmt2300_DelayMs(ms)            delay_ms(ms) //system_delay_ms(ms)
#define Cmt2300_DelayUs(us)            delay_us(us)//system_delay_us(us)
#define Cmt2300_GetTickCount()         0//g_nSysTickCount


__inline void cmt_spi3_delay(void);
__inline void cmt_spi3_delay_us(void);

void cmt_spi3_init(void);

void cmt_spi3_send(u8 data8);
u8 cmt_spi3_recv(void);

void cmt_spi3_write(u8 addr, u8 dat);
void cmt_spi3_read(u8 addr, u8* p_dat);

void cmt_spi3_write_fifo(const u8* p_buf, u16 len);
void cmt_spi3_read_fifo(u8* p_buf, u16 len);

#endif
