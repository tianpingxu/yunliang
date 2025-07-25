#include "motor.h"

void motor_init(void) {
    RCC_EnableAPB2PeriphClk(
        RCC_APB2_PERIPH_GPIOA | RCC_APB2_PERIPH_GPIOB, ENABLE);
    
    GPIO_InitType GPIO_InitStructure;
    /*
		motor power on
	*/
    GPIO_InitStructure.Pin		  	= MOTOR_POWER_PIN;
    GPIO_InitStructure.GPIO_Pull	= GPIO_No_Pull;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
    GPIO_InitPeripheral(MOTOR_POWER_PORT, &GPIO_InitStructure);
    //默认 power on
    GPIO_SetBits(MOTOR_POWER_PORT, MOTOR_POWER_PIN);

	/*
		motor ch1 | ch2
	*/
	GPIO_InitStructure.Pin		  = MOTOR_CH1_PIN | MOTOR_CH2_PIN;
    GPIO_InitStructure.GPIO_Pull	= GPIO_No_Pull;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
	GPIO_InitPeripheral(MOTOR_CH_PORT, &GPIO_InitStructure);

    //默认 brake
    GPIO_SetBits(MOTOR_CH_PORT, MOTOR_CH1_PIN);
    GPIO_SetBits(MOTOR_CH_PORT, MOTOR_CH2_PIN);

	hall_init();
    motor_tim3init();
}

void motor_tim3init(void) {
    ULONG ulPrescalerValue = 0;
    TIM_TimeBaseInitType TIM_TimeBaseStructure;
    NVIC_InitType NVIC_InitStructure;
    
    /* TIM3 clock enable */
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM3, ENABLE);
    
    /* Compute the prescaler value to have TIMx counter clock equal to 100 kHz */
    ulPrescalerValue = (ULONG) (SystemCoreClock / 1000000) - 1;

    /* Time base configuration */
    TIM_TimeBaseStructure.Period    = 10 - 1;  // 10 us
    TIM_TimeBaseStructure.Prescaler = ulPrescalerValue;
    TIM_TimeBaseStructure.ClkDiv    = 0;
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;
    TIM_InitTimeBase(TIM3, &TIM_TimeBaseStructure);
    /* Prescaler configuration */
    TIM_ConfigPrescaler(TIM3, ulPrescalerValue, TIM_PSC_RELOAD_MODE_IMMEDIATE);

    /* Enable the TIM3 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* TIM3 enable update irq */
    TIM_ConfigInt(TIM3, TIM_INT_UPDATE, ENABLE);
    /* TIM3 enable counter */
    TIM_Enable(TIM3, ENABLE);
}


uint8_t motor_location = 0;
uint8_t current_rotate_dir = 0;
uint8_t next_rotate_dir = 0;
uint8_t next_rotate_dir_flag = 0;

uint16_t cnt_10us_over = 0;
uint16_t cnt_1ms_over_1 = 0;

uint8_t motor_start_flag = 0;
uint8_t rotate_run_flag = 0;

void motor_set_dir(uint8_t dir) {
    if(!current_rotate_dir){
        if(motor_location != dir) {
            current_rotate_dir = dir;
            motor_start_flag = 1;
            rotate_run_flag = 1;
            cnt_10us_over = 0;
            cnt_1ms_over_1 = 0;
        }
    }else {
        if(!next_rotate_dir) {
            next_rotate_dir = dir;
        }
    }
}

uint8_t motor_pwm = 0;
uint16_t cnt_10us_1 = 0;
uint16_t cnt_10us_2 = 0;
uint16_t cnt_1ms_over_2 = 0;
void TIM3_IRQHandler(void)
{
	if (TIM_GetIntStatus(TIM3, TIM_INT_UPDATE) != RESET)
	{
        TIM_ClrIntPendingBit(TIM3, TIM_INT_UPDATE);

        if(rotate_run_flag) {
            cnt_10us_1++;
            if (cnt_10us_1 <= 100) {
                if(cnt_10us_1 <= 80) {
                    if(current_rotate_dir == MOTOR_DIR_RIGHT) {
                        GPIO_SetBits(MOTOR_CH_PORT, MOTOR_CH1_PIN);
                    }else if(current_rotate_dir == MOTOR_DIR_LEFT) {
                        GPIO_SetBits(MOTOR_CH_PORT, MOTOR_CH2_PIN);
                    }
                }else {
                    GPIO_ResetBits(MOTOR_CH_PORT, MOTOR_CH1_PIN);
                    GPIO_ResetBits(MOTOR_CH_PORT, MOTOR_CH2_PIN);
                }
            }else {
                cnt_10us_1 = 0;
            }
        }

        // 超时计时
        if(motor_start_flag) {
            cnt_10us_over++;
            if(cnt_10us_over >= 100) {
                cnt_10us_over = 0;
                cnt_1ms_over_1++;
                if(cnt_1ms_over_1 >= 1000) {
                    cnt_1ms_over_1 = 0;
                    motor_start_flag = 0;
                    oled_show_line3(MOTOR_BLOCKED, 0);
                    motor_location = MOTOR_LOCATION_ERR;

                    if(current_rotate_dir == MOTOR_LOCATION_RIGHT) {
                        SET_BIT(DEV_ERR_STATE, 0);
                    }else if(current_rotate_dir == MOTOR_LOCATION_LEFT) {
                        SET_BIT(DEV_ERR_STATE, 1);
                    }

                    rotate_run_flag = 0;
				    current_rotate_dir = 0;
                    motor_mode(MOTOR_DIR_BRAKE);
                    if(next_rotate_dir) {
                        next_rotate_dir_flag = 1;
                    }
                }
            }
        }

        if(next_rotate_dir_flag) {
            cnt_10us_2++;
            if(cnt_10us_2 >= 100) {
                cnt_10us_2 = 0;
                cnt_1ms_over_2++;
                if(cnt_1ms_over_2 >= 200) {
                    cnt_1ms_over_2 = 0;
                    next_rotate_dir_flag = 0;
                    motor_set_dir(next_rotate_dir);
                    next_rotate_dir = 0;
                }
            }
        }
    }
}

uint16_t cnt_20ms = 0;
void rotate_over_time(void) {
    if(motor_location == MOTOR_LOCATION_ERR) {
        if(!nail_hall_err_flag) {
            cnt_20ms++;
            if(cnt_20ms >= 150) {
                cnt_20ms = 0;
                nail_hall_err_flag = 1;
                SET_BIT(DEV_ERR_STATE, 6);
            }
        }
    }else {
        cnt_20ms = 0;
    }
}

void motor_mode(uint8_t mode){
    if(mode == MOTOR_POWER_ON) {
        GPIO_SetBits(MOTOR_POWER_PORT, MOTOR_POWER_PIN);
    }else if(mode == MOTOR_POWER_OFF) {
        GPIO_ResetBits(MOTOR_POWER_PORT, MOTOR_POWER_PIN);
    }else if(mode == MOTOR_DIR_FORWARD) {
        GPIO_SetBits(MOTOR_CH_PORT, MOTOR_CH1_PIN);
        GPIO_ResetBits(MOTOR_CH_PORT, MOTOR_CH2_PIN);
    }else if(mode == MOTOR_DIR_BACKWARD) {
        GPIO_SetBits(MOTOR_CH_PORT, MOTOR_CH2_PIN);
        GPIO_ResetBits(MOTOR_CH_PORT, MOTOR_CH1_PIN);
    }else if(mode == MOTOR_DIR_COAST) {
        GPIO_ResetBits(MOTOR_CH_PORT, MOTOR_CH1_PIN);
        GPIO_ResetBits(MOTOR_CH_PORT, MOTOR_CH2_PIN);
    }else if(mode == MOTOR_DIR_BRAKE) {
        GPIO_SetBits(MOTOR_CH_PORT, MOTOR_CH1_PIN);
        GPIO_SetBits(MOTOR_CH_PORT, MOTOR_CH2_PIN);
    }else {
    }
}

void hall_init(void) {
    GPIO_InitType GPIO_InitStructure;
    /*PB15 - HALL A   PB14 - HALL B */
	GPIO_InitStructure.Pin 	  = HALL_A_PIN;
	GPIO_InitStructure.GPIO_Pull	  = GPIO_Pull_Up;
	GPIO_InitStructure.GPIO_Mode	  = GPIO_Mode_Input;
	GPIO_InitPeripheral(HALL_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pin 	  = HALL_B_PIN;
	GPIO_InitStructure.GPIO_Pull	  = GPIO_Pull_Up;
	GPIO_InitStructure.GPIO_Mode	  = GPIO_Mode_Input;
	GPIO_InitPeripheral(HALL_PORT, &GPIO_InitStructure); 
}

void hall_detect_power(void) {
    if(!GPIO_ReadInputDataBit(HALL_PORT, HALL_A_PIN)) {
        motor_location = MOTOR_LOCATION_RIGHT;
        oled_show_line3(READY_COMPLETE, 0);
    }else if(!GPIO_ReadInputDataBit(HALL_PORT, HALL_B_PIN)) {
        motor_location = MOTOR_LOCATION_LEFT;
        oled_show_line3(NAIL_UP_OK, 0);
    }else{
        motor_location = MOTOR_LOCATION_ERR;
        oled_show_line3(NAIL_LOCATION_EER, 0);
    }
}

void hall_detect(void) {
    if(!GPIO_ReadInputDataBit(HALL_PORT, HALL_A_PIN)) {          //HALL_A 准备完成
        nail_hall_err_flag = 0;
        CLEAR_BIT(DEV_ERR_STATE, 6);
        motor_location = MOTOR_LOCATION_RIGHT;
        if(current_rotate_dir == motor_location) {
            oled_show_line3(READY_COMPLETE, 0);
            motor_start_flag = 0;
            CLEAR_BIT(DEV_ERR_STATE, 0);
            // 刹车
            rotate_run_flag = 0;
            current_rotate_dir = 0;
            motor_mode(MOTOR_DIR_BRAKE);
            if(next_rotate_dir) {
                next_rotate_dir_flag = 1;
            }
            oled_show_line3(READY_COMPLETE, 0);
        }
    }else if(!GPIO_ReadInputDataBit(HALL_PORT, HALL_B_PIN)) {    //HALL_B 落钉完成
        nail_hall_err_flag = 0;
        CLEAR_BIT(DEV_ERR_STATE, 6);
        motor_location = MOTOR_LOCATION_LEFT;
        if(current_rotate_dir == motor_location) {
            oled_show_line3(NAIL_UP_OK, 0);
            motor_start_flag = 0;
            CLEAR_BIT(DEV_ERR_STATE, 1);
            // 刹车
            rotate_run_flag = 0;
            current_rotate_dir = 0;
            motor_mode(MOTOR_DIR_BRAKE);
            if(next_rotate_dir) {
                next_rotate_dir_flag = 1;
            }
            oled_show_line3(NAIL_UP_OK, 0);
        }
    }else {
        motor_location = MOTOR_LOCATION_ERR;
    }
}
