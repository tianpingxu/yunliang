#ifndef APP_SYSTEM_H
#define APP_SYSTEM_H

typedef enum
{
    API_COMP_EQUAL = 0,		
    API_COMP_OVER_EQUAL,	
    API_COMP_OVER			
} API_COMP_MODE;

typedef struct
{
    SHORT *DATA1;				
    SHORT *DATA2;				
    SHORT *count;				
    API_COMP_MODE COMPAREMODE;	
} st_state;



//#define SYS_STATE_INIT		(0)	
//#define SYS_STATE_NORMAL	(1)	
//#define SYS_STATE_LOWPOWER	(2)	
//#define SYS_STATE_STOP		(3)	
//#define SYS_STATE_CHARG_IN_STOP		(4)	
//#define SYS_STATE_KEY_POWER_ON		(5)	
//#define SYS_STATE_CHARG_IN_LOWPOWER		(6)	
typedef enum
{
    SYS_STATE_INIT = 0,		
    SYS_STATE_NORMAL,	
    SYS_STATE_LOWPOWER,	
    SYS_STATE_STOP,
    SYS_STATE_CHARG_IN_STOP,
    SYS_STATE_KEY_POWER_ON,
    SYS_STATE_CHARG_IN_LOWPOWER,
} EN_SYS_STATUS;


#define EVENT_KEY_POWER_ON		(1)
#define EVENT_PLUG_POWER_ON		(2)
#define EVENT_KEY_WAKE_UP		(3)
#define EVENT_PLUG_WAKE_UP		(4)

#define IN_CHARGING_COMPLETE()	(u8_gChgComplete == TRUE)//(((ucPlugInsertStatus == STATUS_PLUG_INSERT) && (ucChgCompleteStatus == TRUE) && (s16_gBatVoltage >= 4100)) ||\
								//((ucPlugInsertStatus == STATUS_PLUG_INSERT) && (s16_gBatVoltage >= BAT_VOL_MAX)))
								
#define IN_CHARGING()			(u8_gCharging == TRUE) //(((ucPlugInsertStatus == STATUS_PLUG_INSERT) && (ucChgCompleteStatus == TRUE) && (s16_gBatVoltage < 4100)) ||\
								//((ucPlugInsertStatus == STATUS_PLUG_INSERT) && (ucChgCompleteStatus == FALSE)))

//#define IN_CHARGING()				((ucPlugInsertStatus == STATUS_PLUG_INSERT) && ((ucChgCompleteStatus == FALSE) ||(s16_gBatVoltage < BAT_VOL_MAX)))
#define IN_CHARGING_ERROR()			((ucPlugInsertStatus == STATUS_PLUG_INSERT) && (u16_gFaultStatus & FAULT_BAT_OT))


typedef struct
{
	UCHAR refrigerant_type;	//冷媒类型
	UCHAR pressure_unit;
	UCHAR temp_unit;
	UCHAR shutdown_time;
	UCHAR lowpower_time;
}SYS_PARAM;

#define UNIT_TEMP_F		(0)
#define UNIT_TEMP_C		(1)

#define UNIT_PRESSURE_MPA		(0)
#define UNIT_PRESSURE_BAR		(1)
#define UNIT_PRESSURE_KGF		(2)
#define UNIT_PRESSURE_PSI		(3)
#define UNIT_PRESSURE_KPA		(4)

#define TIME_SHUTDOWN_OFF		(0)
#define TIME_SHUTDOWN_1H		(1)
#define TIME_SHUTDOWN_2H		(2)

#define TIME_LOWPOWER_OFF		(0)
#define TIME_LOWPOWER_2MIN		(1)
#define TIME_LOWPOWER_5MIN		(2)
#define TIME_LOWPOWER_10MIN		(3)


#define WAKE_INTERRUPT_WKT	((UCHAR)1<<0)
#define WAKE_INTERRUPT_PLUG	((UCHAR)1<<1)
#define WAKE_INTERRUPT_SW	((UCHAR)1<<2)

#define LOWPOWER_LOOP_PERIOD	(1000)


#define SET_INTERRUPT_FLAG(x) 		(u8_gWakeUpEvent |= (x))
#define CLEAR_INTERRUPT_FLAG(x) 	(u8_gWakeUpEvent &= (~(x)))

#define FAULT_BAT_OT	((UCHAR)1<<0)
#define FAULT_LOW_VOL	((UCHAR)1<<1)


extern EN_SYS_STATUS u8_gSysState;
extern UCHAR u8_gWakeUpEvent;
extern SYS_PARAM st_gSysParam;
extern UCHAR u8_SysPowerDownCmd;
extern UCHAR u8_gLcdPowerOnEvent;
extern ULONG u32_gPowerDownTimeAdd;
extern ULONG u32_gLowPowerTimeAdd;
extern UCHAR u8_PowerDownEnable;
extern USHORT u16_gFaultStatus;
extern UCHAR u8_gChgComplete;
extern UCHAR u8_gCharging;



void vSetSysState(UCHAR st);
void vStateInit(void);
void vStateNormal(void);
void vStateLowpower(void);
void vStateStop(void);
void vSoftInit(void);
void vPowerOnEventCheck(void);
void vPowerOnEventCheck(void);
void vStateKeyPowerOn(void);
void vStateChargeInStop(void);
void vChargeManage(void);
void vStateChargeInLowpower(void);
void ucChargeStateCheck(void);


#endif
