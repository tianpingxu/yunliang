/******************************************************************************
#ifdef DOC
File Name		: bsp_time.c
Description		: Cell Balance
Remark			: -
Date			: 2019/05/17
Copyright		: Techsum Power Technology Co., Ltd.
#endif
******************************************************************************/
/****************************************************************************
 include files
*****************************************************************************/
#include "sys_type.h"
#include "n32l40x.h"
#include "bsp_io.h"
#include "bsp_timer.h"
#include "bsp_wdg.h"
#include "delay.h"
/****************************************************************************
 define macro
*****************************************************************************/

/****************************************************************************
 define type (STRUCTURE, UNION)
*****************************************************************************/
void vKeyCheck(UCHAR period);
void vUartRevOtCallback(void);
void vComLostOtCallback(UCHAR period);

/****************************************************************************
 define type (Enumerated type ENUM)
*****************************************************************************/

/****************************************************************************
 Variable declaration
*****************************************************************************/
ULONG ulTimer1ms;
ULONG ulTimer1ms_1;
UCHAR ucTIM_50msFlg = TRUE;
UCHAR ucTIM_20msFlg = TRUE;

volatile USHORT	unTIM_1msCounter;

/****************************************************************************
 Prototype declaration
*****************************************************************************/
/******************************************************************************
#ifdef DOC
FUNCTION		: vBSP_TimerInit
DESCRIPTION		: used for fullspeed timebase
INPUT			: none
OUTPUT			: none
UPDATE			: -
DATE			: 2019/6/4
******************************************************************************/
void vBSP_Timer2Init(void)
{
    ULONG ulPrescalerValue = 0;
	  TIM_TimeBaseInitType TIM_TimeBaseStructure;
	  NVIC_InitType NVIC_InitStructure;
    
    /* TIM2 clock enable */
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM2, ENABLE);
    
    /* Compute the prescaler value to have TIMx counter clock equal to 100 kHz */
    ulPrescalerValue = (ULONG)(SystemCoreClock / 10000) - 1;
    /* Time base configuration */
    TIM_TimeBaseStructure.Period    = 10 - 1;	/*1 MS*/
    TIM_TimeBaseStructure.Prescaler = ulPrescalerValue;
    TIM_TimeBaseStructure.ClkDiv    = 0;
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;
    TIM_InitTimeBase(TIM2, &TIM_TimeBaseStructure);
    /* Prescaler configuration */
    TIM_ConfigPrescaler(TIM2, ulPrescalerValue, TIM_PSC_RELOAD_MODE_IMMEDIATE);

    /* Enable the TIM2 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /*##-2- Start the TIM Base generation in interrupt mode ####################*/
    /* Start Channel1 */
    ucTIM_20msFlg = FALSE;
    ucTIM_50msFlg = FALSE;
    
    /* TIM2 enable update irq */
    TIM_ConfigInt(TIM2, TIM_INT_UPDATE, ENABLE);
    /* TIM2 enable counter */
    TIM_Enable(TIM2, ENABLE);
}


/******************************************************************************
#ifdef DOC
FUNCTION		: vBSP_DelayMs
DESCRIPTION		: 
INPUT			: none
OUTPUT			: none
UPDATE			: -
DATE			: 2019/5/17
******************************************************************************/
void vBSP_DelayMs(ULONG msDly)
{
    // for(USHORT idx = 0;idx < msDly;idx++)
    // {
    //   vSysTickDelay(1000);	/*1ms*/
    // }
    delay_ms(msDly);
}

void vBSP_DelayUs(ULONG usDly)
{
    // for(USHORT idx = 0;idx < usDly;idx++)
    // {
    //   vSysTickDelay(1);	/*1us*/
    // }
    delay_us(usDly);
}

/****************************************************************************
#ifdef DOC
FUNCTION		: vTIM_1msStart
DESCRIPTION		: 1ms timer time setting
INPUT			: uctime[ms]
OUTPUT			: None
UPDATE			:
DATE			: 13/06/30
#endif
*****************************************************************************/
void vTIM_1msStart(USHORT usTime)
{
    unTIM_1msCounter = usTime;
}
/****************************************************************************
#ifdef DOC
FUNCTION		: vTIM_1msIsTimeout
DESCRIPTION		: Time-out confirmation of 1ms timer
INPUT			: None
OUTPUT			: 0:Unexpired 1:Time-out
UPDATE			:
DATE			: 13/06/30
#endif
*****************************************************************************/
UCHAR vTIM_1msIsTimeout(void)
{
    return (unTIM_1msCounter == 0);
}
/******************************************************************************
#ifdef DOC
FUNCTION		: HAL_TIM_PeriodElapsedCallback
DESCRIPTION		: 
INPUT			: none
OUTPUT			: none
UPDATE			: -
DATE			: 2019/5/17
******************************************************************************/

void HAL_TIM_PeriodElapsedCallback(TIM_Module *htim)
{
    if(htim == TIM2)
    {
      ulTimer1ms++;
      ulTimer1ms_1++;

      if(ulTimer1ms_1 >= 20) {
          ulTimer1ms_1 = 0;
          ucTIM_20msFlg = TRUE;
      }

      if( ulTimer1ms >= 50)
      {
          ulTimer1ms = 0;
          ucTIM_50msFlg = TRUE;
      }
    }
}




/******************************************************************************
#ifdef DOC
FUNCTION		: HAL_SYSTICK_Callback
DESCRIPTION		: 
INPUT			: none
OUTPUT			: none
UPDATE			: -
DATE			: 2019/5/17
******************************************************************************/
void SYSTICK_Callback(void)
{
    if(unTIM_1msCounter > 0)
    {
        unTIM_1msCounter--;
    }
  //  vBOOT_1MsCallback();
  //  FlashDriver1MsCallback();
  //  vUartRevOtCallback();
}


__IO uint32_t uwTick;
void HAL_IncTick(void)
{
    uwTick++;
}
uint32_t HAL_GetTick(void)
{
    return uwTick;
}

 void vSysTickDelay(__IO uint32_t Delay)
{
    uint32_t tickstart = HAL_GetTick();
    while((HAL_GetTick() - tickstart) < Delay);
}

void SysTick_Deinit(void)
{
	  SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk;
}

