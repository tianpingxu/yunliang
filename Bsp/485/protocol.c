#include "protocol.h"

uint8_t DEV_ERR_STATE = 0;

uint8_t nail_up_err_flag = 0;
uint8_t nail_down_err_flag = 0;
uint8_t jy_chat_timeout_flag = 0;
uint8_t communication_err_flag = 0;
uint8_t dev_vol_low_flag = 0;
uint8_t dev_vol_high_flag = 0;
uint8_t nail_hall_err_flag = 0;
uint8_t remote_chat_timeout_flag = 0;

extern uint8_t beep_flag;
extern uint8_t Speed_up_flag;

uint8_t heart_frame[7] = {0xAA, 0x55, STANDBY_RESPONSE_POLL, CW_DEV, LOCAL_DEV};

void rx_data_hadle(uint8_t* buf, uint16_t len) {
    // 帧头校验
    if((buf[0] != HEADER_1) || (buf[1] != HEADER_2))
        return;
    // 和校验
    if(xy_frame_calc_xor(buf, 6) != buf[6]){
        communication_err_flag = 1;
        SET_BIT(DEV_ERR_STATE, 3);
        return;
    }
    // local to cw
    if((buf[3] == LOCAL_DEV) && (buf[4] == CW_DEV)) {
        switch (buf[2]) {
            // 查询心跳
            case HEART_BEAT:
                communication_err_flag = 0;
                CLEAR_BIT(DEV_ERR_STATE, 3);
                heart_frame[5] = DEV_ERR_STATE;
                heart_frame[6] = xy_frame_calc_xor(heart_frame, 6);
                RS485_SendStr_length(heart_frame, 7);
                break;
            // 起钉
            case NAIL_UP:
                communication_err_flag = 0;
                CLEAR_BIT(DEV_ERR_STATE, 3);
                remote_chat_timeout_flag = 0;
                CLEAR_BIT(DEV_ERR_STATE, 7);
                motor_set_dir(MOTOR_LOCATION_LEFT);
                cw_to_local_frame(NAIL_UP_INFO, DEV_ERR_STATE);
                break;
            // 落钉
            case NAIL_DOWN:
                communication_err_flag = 0;
                CLEAR_BIT(DEV_ERR_STATE, 3);
                remote_chat_timeout_flag = 0;
                CLEAR_BIT(DEV_ERR_STATE, 7);
                motor_set_dir(MOTOR_LOCATION_RIGHT);
                cw_to_local_frame(NAIL_DOWN_INFO, DEV_ERR_STATE);
                break;
            default :
                communication_err_flag = 1;
                SET_BIT(DEV_ERR_STATE, 3);
                heart_frame[5] = DEV_ERR_STATE;
                heart_frame[6] = xy_frame_calc_xor(heart_frame, 6);
                RS485_SendStr_length(heart_frame, 7);
                break;
        }
    }
}

void cw_to_local_frame(uint8_t fun, uint8_t dev_state) {
    uint8_t h_frame[7] = {0xAA, 0x55, STANDBY_RESPONSE_POLL, CW_DEV, LOCAL_DEV};
    h_frame[2] = fun;
    h_frame[5] = dev_state;
    heart_frame[6] = xy_frame_calc_xor(h_frame, 6);
    RS485_SendStr_length(h_frame, 7);
}

