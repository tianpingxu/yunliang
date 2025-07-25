#ifndef __MOTOR_H__
#define __MOTOR_H__

#include <n32l40x.h>
#include <stdint.h>

#include "sys_type.h"

#include "oled_show.h"

#include "protocol.h"

#define MOTOR_POWER_PORT    GPIOA
#define MOTOR_POWER_PIN     GPIO_PIN_3

#define MOTOR_CH_PORT       GPIOB
#define MOTOR_CH1_PIN       GPIO_PIN_13
#define MOTOR_CH2_PIN       GPIO_PIN_12

#define HALL_PORT           GPIOB
#define HALL_A_PIN          GPIO_PIN_15
#define HALL_B_PIN          GPIO_PIN_14

#define MOTOR_DIR_0         0
#define MOTOR_DIR_1         1

#define MOTOR_DIR_LEFT           1
#define MOTOR_DIR_RIGHT          2

#define MOTOR_LOCATION_LEFT      1
#define MOTOR_LOCATION_RIGHT     2
#define MOTOR_LOCATION_ERR       3

typedef enum{
    MOTOR_POWER_ON,
    MOTOR_POWER_OFF,
    MOTOR_DIR_COAST,
    MOTOR_DIR_FORWARD,
    MOTOR_DIR_BACKWARD,
    MOTOR_DIR_BRAKE
}MOTOR_MOEE;


void motor_init(void);
void motor_tim3init(void);
void motor_mode(uint8_t mode);
void motor_set_dir(uint8_t dir);

void rotate_over_time(void);

void hall_init(void);
void hall_detect(void);
void hall_detect_power(void);

#endif