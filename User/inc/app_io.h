#ifndef APP_IO_H
#define APP_IO_H

#define STATUS_PLUG_INSERT	(1)
#define STATUS_PLUG_REMOVE	(0)

#define KEY_IO_UP		(1)
#define KEY_IO_DOWN		(0)
#define KEY_UP_UP		(0)
#define KEY_UP_DOWN		(1)
#define KEY_DOWN_DOWN	(2)
#define KEY_DOWN_UP		(3)

#define ENCODE_EVENT_NULL	(0)
#define ENCODE_EVENT_SW		(1)
#define ENCODE_EVENT_RIGHT	(2)
#define ENCODE_EVENT_LEFT	(3)
#define ENCODE_EVENT_KEY	(4)

typedef enum
{
	KEY_STATE_NULL = 0,	
	KEY_STATE_RLS,		
	KEY_STATE_ONCE,	
	KEY_STATE_DOUBLE,
	KEY_STATE_LONG,		
	KEY_STATE_LONG_1S	
}EN_KEY_STATE;

typedef struct
{
	EN_KEY_STATE KeyState;	
	UCHAR KeyDownCnt;

	UCHAR key_double_event;
	UCHAR key_once_event;
}ST_KEY;

#define KEY_EVENT_1		(1 << 0)
#define KEY_EVENT_2		(1 << 1)
#define KEY_EVENT_3		(1 << 2)
#define KEY_EVENT_4		(1 << 3)
#define KEY_EVENT_5		(1 << 4)
#define KEY_EVENT_6		(1 << 5)
#define KEY_EVENT_7		(1 << 6)
#define KEY_EVENT_8		(1 << 7)
#define KEY_EVENT_ALL	(0xFF)
#define KEY_EVENT_NULL	(0)


extern ST_KEY st_gKey;
//extern UCHAR ucChgStatus;
extern UCHAR  ucSwStatus;
extern UCHAR  ucChgCompleteStatus;
extern UCHAR  ucPlugInsertStatus;
extern UCHAR  ucEncodeStatus;

UCHAR vSysPowerOn(void);
void vKeyCheck(UCHAR period);
//void vEncodeCheck(void);
void vAPI_IoStatus(void);


#endif
