/****************************************************************************
#ifdef DOC

File Name		:app_balance.h

Description		:

Remark			:

Date			:19/05/17

Copyright		:Techsum Power Technology Co., Ltd.

#endif
*****************************************************************************/
#ifndef		BSP_TIMER_H
#define		BSP_TIMER_H

#include "sys_type.h"

/****************************************************************************
 define macro
*****************************************************************************/

/****************************************************************************
 define type (STRUCTURE, UNION)
*****************************************************************************/

/****************************************************************************
 define type (Enumerated type ENUM)
*****************************************************************************/

/****************************************************************************
 Variable declaration
*****************************************************************************/
extern UCHAR ucTIM_50msFlg;
extern UCHAR ucTIM_20msFlg;
/****************************************************************************
 Prototype declaration
*****************************************************************************/
void HAL_IncTick(void);
uint32_t HAL_GetTick(void);
void vSysTickDelay(__IO uint32_t Delay);
void SysTick_Deinit(void);


void vBSP_Timer2Init(void);

void HAL_TIM_PeriodElapsedCallback(TIM_Module *htim);
void SYSTICK_Callback(void);

void vBSP_DelayMs(ULONG msDly);
void vBSP_DelayUs(ULONG usDly);

void vTIM_1msStart(USHORT usTime);
UCHAR vTIM_1msIsTimeout(void);

#endif	/* CB_H */

