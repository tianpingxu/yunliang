#include "n32l40x.h"
#include <time.h>
#include "sys_type.h"
#include "bsp_rtc.h"
#include "app_system.h"

static time_t stUTCTime;



#define TIME_FORMAT_HAL2LIB(haldate, haltime, librtc)    do{\
		(librtc).tm_year	 = (haldate).Year + 100;\
		(librtc).tm_mon 	 = (haldate).Month - 1;\
		(librtc).tm_mday	 = (haldate).Date;\
		(librtc).tm_wday	 = (haldate).WeekDay;\
		(librtc).tm_hour	 = (haltime).Hours;\
		(librtc).tm_min 	 = (haltime).Minutes;\
		(librtc).tm_sec 	 = (haltime).Seconds;\
		(librtc).tm_isdst	 = 0;\
		}while(0)
	
#define TIME_FORMAT_LIB2HAL(librtc, haldate, haltime)    do{\
		(haldate).Year		= (librtc).tm_year - 100;\
		(haldate).Month 	= (librtc).tm_mon + 1;\
		(haldate).Date		= (librtc).tm_mday;\
		(haldate).WeekDay	= (librtc).tm_wday;\
		(haltime).Hours 	= (librtc).tm_hour;\
		(haltime).Minutes	= (librtc).tm_min;\
		(haltime).Seconds	= (librtc).tm_sec;\
		}while(0)




void vRtc_Init(void)
{
	RTC_InitType		  RTC_InitStructure;

	//	RTC_DeInit();
	/* RTC clock source select 1:HSE/128 2:LSE 3:LSI*/
	/* Enable the PWR clock */
	RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_PWR, ENABLE);
	/* Allow access to RTC */
	PWR_BackupAccessEnable(ENABLE);
	/* Disable RTC clock */
	RCC_EnableRtcClk(DISABLE);	 

	/* Enable the LSI OSC */
	RCC_EnableLsi(ENABLE);
	/*wait LSI Ready*/
	while (RCC_GetFlagStatus(RCC_CTRLSTS_FLAG_LSIRD) == RESET);
	RCC_ConfigRtcClk(RCC_RTCCLK_SRC_LSI);
	/* Enable the RTC Clock */
	RCC_EnableRtcClk(ENABLE);
	RTC_WaitForSynchro();

	/* The time base should be 1ms
	 Time base = ((RTC_ASYNCH_PREDIV + 1) * (RTC_SYNCH_PREDIV + 1)) / RTC_CLOCK
	 LSI as RTC clock
	   Time base = ((399 + 1) * (99 + 1)) / 40Khz
				 = 1000ms
	*/
	/* Configure the RTC data register and RTC prescaler */
	RTC_InitStructure.RTC_AsynchPrediv = 99;
	RTC_InitStructure.RTC_SynchPrediv = 399;
	RTC_InitStructure.RTC_HourFormat = RTC_24HOUR_FORMAT;
	/* Check on RTC init */
	RTC_Init(&RTC_InitStructure);    
}

UCHAR ucBSP_RtcSetTime(struct tm * pstTime)
{
    RTC_DateType  sdatestructure;
    RTC_TimeType  stimestructure;

	stimestructure.H12 = RTC_AM_H12;
    TIME_FORMAT_LIB2HAL((*pstTime), sdatestructure,stimestructure);
    
	RTC_SetDate(RTC_FORMAT_BIN, &sdatestructure);
	RTC_ConfigTime(RTC_FORMAT_BIN, &stimestructure);
    return TRUE;
}


void vBSP_RtcGetTime(struct tm * pstTime)
{
    RTC_TimeType Time;
    RTC_DateType Date; 
    
    RTC_GetTime(RTC_FORMAT_BIN, &Time);
    RTC_GetDate(RTC_FORMAT_BIN, &Date);
    TIME_FORMAT_HAL2LIB(Date,Time, (*pstTime));
}


void vAPI_RtcTimeUpdate(void)
{
    struct tm stTime;
    vBSP_RtcGetTime(&stTime);
    stUTCTime = mktime(&stTime);
}

ULONG ulAPI_GetRtcTime(void)
{
    return (ULONG)stUTCTime;
}

void vAPI_RtcTimeInit(void)
{
    /* init stbmstime value */
    vAPI_RtcTimeUpdate();
}

void HAL_RTCEx_WakeUpTimerEventCallback(void)
{
	SET_INTERRUPT_FLAG(WAKE_INTERRUPT_WKT);
}

