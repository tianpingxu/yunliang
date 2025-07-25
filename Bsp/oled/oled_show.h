#ifndef __OLED_SHOW_H__
#define __OLED_SHOW_H__

#include "n32l40x.h"
#include <string.h>

#include "delay.h"

#include "api.h"

#include "protocol.h"

#define Font_Size_F6X8  1
#define Font_Size_F8x16 0

#define LOW_VOL     90
#define HIGH_VOL    150


typedef enum{
    READY_COMPLETE = 1,
    COMMUNICATION_NOMAL,
    COMMUNICATION_DISCONNECT,
    MOTOR_BLOCKED,
    NAIL_UP_OK,
    NAIL_LOCATION_EER,
    HARDWARE_EER,
    EER_CODE
}DEV_INFO;

typedef struct {
    uint8_t dev_vol_now;
    uint8_t dev_vol_old;

    uint8_t dev_err_now;
    uint8_t dev_err_old;

    uint8_t dev_err_code_now;
    uint8_t dev_err_code_old;

}DEV_STATE;



void oled_show(void);

void oled_show_line1(void);
void oled_show_line2(uint16_t vol);
void oled_show_line3(uint8_t dev_info, uint8_t err_code);

#endif