#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#include <n32l40x.h>
#include <stdio.h>

#include "485.h"
#include "api.h"
#include "motor.h"

extern uint8_t DEV_ERR_STATE;

extern uint8_t nail_up_err_flag;
extern uint8_t nail_down_err_flag;
extern uint8_t jy_chat_timeout_flag;
extern uint8_t communication_err_flag;
extern uint8_t dev_vol_low_flag;
extern uint8_t dev_vol_high_flag;
extern uint8_t nail_hall_err_flag;
extern uint8_t remote_chat_timeout_flag;

#define	GET_BIT(x, bit)	((x & (1 << bit)) >> bit)	/* 获取第bit位 */
#define	SET_BIT(x, bit)	(x |= (1 << bit))	        /* 置位第bit位 */
#define	CLEAR_BIT(x, bit)	(x &= ~(1 << bit))	    /* 清零第bit位 */
 
#define HEADER_1           0xAA
#define HEADER_2           0x55

typedef enum {
	HEART_BEAT  = 0x25,
	NAIL_UP		= 0x26,	
	NAIL_DOWN	= 0x27,
}LOCAL_TO_CW;

typedef enum {
	NAIL_UP_INFO = 0x66,
	NAIL_DOWN_INFO,
	NET_PULL_OPEN_INFO,
	NET_PULL_CLOSE_INFO,
	STANDBY_RESPONSE_POLL,
	NAIL_UP_COMPLETE_INFO = 0xA6,
	NAIL_DOWN_COMPLETE_INFO,
	NET_PULL_OPEN_COMPLETE_INFO,
}CW_TO_LOCAL;

typedef enum {
	QUERY_COMPLETE = 0x22,
	QUERY_FRAME,
	HEART_BEAT_LOCAL,
	HEART_BEAT_CW,
	RCV_NAIL_UP_FRAME,
	RCV_NAIL_DOWN_FRAME,
	RCV_NET_PULL_FRAME,

	NAIL_UP_COMPLETE = 0x26,
	NAIL_DOWN_COMPLETE,
	NET_PULL_COMPLETE
}LOCAL_TO_JY;

typedef enum {
	LOCAL_DEV = 1,
	CW_DEV,
	JY_DEV,
	REMOTE_CTRL_DEV = 9
}SRC_DEST_ADDR;

typedef enum {
	NAIL_UP_STUCK,
	NAIL_DOWN_STUCK,
	COMMUNI_TIMEOUT,
	COMMUNI_ERR,
	VOL_LOW,
	VOL_HIGH,
	NAIL_HALL_ERR,
	RCV_REMOTE_CMD_ERR
}CW_ERR_INFO;

typedef enum {
	RF_ERR,
	RS485_ERR,
	NET_POET_ERR
}LOCAL_ERR_INFO;


void rx_data_hadle(uint8_t* buf, uint16_t len);

void cw_to_local_frame(uint8_t fun, uint8_t dev_state);

#endif