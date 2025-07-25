#include "sys_type.h"
#include "n32l40x.h"
#include "bsp_io.h"
#include "bsp_timer.h"
#include "ADS1230.h"
#include "app_io.h"
#include "app_user.h"
#include "app_sample.h"


ST_KEY st_gKey;

void vCheckIOTemple(UCHAR ucIoIn, UCHAR *ucIoCnt, UCHAR * ucIoChatt, UCHAR delay_active,UCHAR delay_deactive, UCHAR ucIoChattdelay, UCHAR * out)
{
    if(*out == 0)
    {
        if(ucIoIn)
        {
            (*ucIoCnt)++;
            *ucIoChatt = 0;
            if(*ucIoCnt > delay_active)
            {
                *out = 1;
                *ucIoCnt = 0;
            }
        }
        else
        {
            (*ucIoChatt)++;
            if(*ucIoChatt >= ucIoChattdelay)
            {
                *ucIoCnt = 0;
            }
        }
    }
    else
    {
        if(ucIoIn == 0)
        {
            (*ucIoCnt)++;
            *ucIoChatt = 0;
            if(*ucIoCnt > delay_deactive)
            {
                *out = 0;
                *ucIoCnt = 0;
            }
        }
        else
        {
            (*ucIoChatt)++;
            if(*ucIoChatt >= ucIoChattdelay)
            {
                *ucIoCnt = 0;
            }
        }
    }
}

UCHAR ucSwDelay, ucSwChatt, ucSwStatus;
UCHAR ucEncodeDelay, ucEncodeChatt, ucEncodeStatus;
//UCHAR ucChgDelay, ucChgChatt, ucChgStatus;
UCHAR ucChgCompleteDelay, ucChgCompleteChatt, ucChgCompleteStatus;
UCHAR uPlugInsertDelay, ucPlugInsertChatt, ucPlugInsertStatus;
void vAPI_IoStatus(void)
{
   // UCHAR ucSwOld = ucSwStatus;
    vCheckIOTemple((P_PWR_KEY_IS_PRESS() == ON),
                    &ucSwDelay,
                    &ucSwChatt,
                    2,
                    2,
                    1,
                    &ucSwStatus);
                    
    vCheckIOTemple((P_ENCODE_IS_PRESS() == ON),
                    &ucEncodeDelay,
                    &ucEncodeChatt,
                    2,
                    2,
                    1,
                    &ucEncodeStatus);
                    
    vCheckIOTemple((P_CHARGING_COMPLETE() == ON),
                    &ucChgCompleteDelay,
                    &ucChgCompleteChatt,
                    20,
                    20,
                    5,
                    &ucChgCompleteStatus);
	                
	vCheckIOTemple((P_PLUG_IS_INSERT() == ON),
					&uPlugInsertDelay,
					&ucPlugInsertChatt,
					5,
					5,
					1,
					&ucPlugInsertStatus);				
}



UCHAR vSysPowerOn(void)
{
	static USHORT wk_time = 0;
	UCHAR wk_ret = FALSE;

	wk_time = ((P_PWR_KEY_IS_PRESS() == TRUE))?(wk_time+ 50):0;
	if(wk_time >= 1800)
	{
		wk_time = 0;
		wk_ret = TRUE;
		P_SYSTEM_POWER_CTRL(ON);	/*维持住开机*/
	}
	else if(wk_time == 500) 
	{
		vADS1230_Init();
		vBSP_DelayMs(200);
		vADS1230_Offset1();
		vADS1230_Offset2();
	}
	else if(wk_time >= 1500) 
	{
		//vSamplePressureTemp();
		//sample adc
	}
	return wk_ret;
}

void vKeyCheck(UCHAR period)
{
	static USHORT wk_key_down_time = 0;
	static UCHAR wk_key_up_time = 0;

	UCHAR wk_key_step;
	static UCHAR wk_pre_key_io = KEY_IO_DOWN;
	UCHAR wk_now_key_io = (P_PWR_KEY_IS_PRESS())?KEY_IO_DOWN:KEY_IO_UP;

	if((wk_pre_key_io == KEY_IO_UP) && (wk_now_key_io == KEY_IO_UP))
	{
		wk_key_step = KEY_UP_UP;
	}
	else if((wk_pre_key_io == KEY_IO_UP) && (wk_now_key_io == KEY_IO_DOWN))
	{
		wk_key_step = KEY_UP_DOWN;
		st_gKey.KeyState = KEY_STATE_RLS;
		st_gKey.key_double_event = KEY_EVENT_NULL;
		st_gKey.key_once_event = KEY_EVENT_NULL;
	}
	else if((wk_pre_key_io == KEY_IO_DOWN) && (wk_now_key_io == KEY_IO_DOWN))
	{
		wk_key_step = KEY_DOWN_DOWN;
	}
	else if((wk_pre_key_io == KEY_IO_DOWN) && (wk_now_key_io == KEY_IO_UP))
	{
		wk_key_step = KEY_DOWN_UP;
	}
	wk_pre_key_io = wk_now_key_io;


	if(st_gKey.KeyState == KEY_STATE_RLS)
	{
		switch(wk_key_step)
		{
			case KEY_UP_UP:
				wk_key_up_time+=period;
				if(wk_key_up_time >= 200)	/*50 * 10 = 500MS*/
				{
					wk_key_up_time = 0;
					wk_key_down_time = 0;
					//st_gKey.KeyState = KEY_STATE_NULL;
					if(st_gKey.KeyDownCnt == 1)
					{
						st_gKey.KeyState = KEY_STATE_ONCE;
					}
					else if(st_gKey.KeyDownCnt == 2)
					{
						st_gKey.KeyState = KEY_STATE_DOUBLE;
					}
					st_gKey.KeyDownCnt = 0;
				}
				break;
			case KEY_UP_DOWN:
				wk_key_down_time = 0;
				break;
			case KEY_DOWN_DOWN:
				wk_key_down_time+=period;
				if(wk_key_down_time >= 2000)
				{
					wk_key_down_time = 2000;
					st_gKey.KeyState = KEY_STATE_LONG;
				}
				else if(wk_key_down_time == 1000)
				{
					//st_gKey.KeyState = KEY_STATE_LONG_1S;
				}	
				break;
			case KEY_DOWN_UP:
				if((wk_key_down_time >30) && (wk_key_down_time < 1000))
				{
					st_gKey.KeyDownCnt++;
				}
				wk_key_up_time = 0;
				break;
			default:
				break;
		}
	}
}





//void vEncodeCheck(void)
//{
//	if(st_gKey.KeyState == KEY_STATE_ONCE)
//	{
//		st_gKey.KeyState = KEY_STATE_NULL;
//		u8_gEncodeEvent = ENCODE_EVENT_KEY;
//	}

//	if(u8_gEncodeEvent != ENCODE_EVENT_NULL)
//	{
//		ucSendEncodeToSlave(ADDR_LDC_MS,u8_gEncodeEvent);
//		u8_gEncodeEvent = ENCODE_EVENT_NULL;
//	}
//}
