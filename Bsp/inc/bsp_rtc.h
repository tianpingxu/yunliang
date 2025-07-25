#ifndef		BSP_RTC_H
#define		BSP_RTC_H

#include "n32l40x.h"
#include <time.h>



void vRtc_Init(void);
UCHAR ucBSP_RtcSetTime(struct tm * pstTime);
void vBSP_RtcGetTime(struct tm * pstTime);
ULONG ulAPI_GetRtcTime(void);
void vAPI_RtcTimeUpdate(void);


#endif
