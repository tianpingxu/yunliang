/******************************************************************************
#ifdef DOC
File Name		: app_balance.c
Description		: Cell Balance
Remark			: -
Date			: 2019/05/17
Copyright		: Techsum Power Technology Co., Ltd.
#endif
******************************************************************************/
/****************************************************************************
 include files
*****************************************************************************/
#include "n32l40x.h"

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
/****************************************************************************
 Prototype declaration
*****************************************************************************/

/******************************************************************************
#ifdef DOC
FUNCTION		: vBSP_WDG_Init
DESCRIPTION		: 
INPUT			: none
OUTPUT			: none
UPDATE			: -
DATE			: 2019/5/17
******************************************************************************/
void vBSP_WDG_Init( void )
{
#if 0
	//WWDG_DeInit();
	RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_WWDG, DISABLE);	  
#ifndef DEBUG

	/* Enable WWDG clock */
	RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_WWDG, ENABLE);	  
	/* WWDG clock counter = (PCLK1(16MHz)/4096)/8 = 488 Hz (~2049 us)  */
	WWDG_SetPrescalerDiv(WWDG_PRESCALER_DIV8);	  
	/* Set Window value to 80; WWDG counter should be refreshed only when the counteris below 80 (and greater than 64) 
otherwise a reset will be generated */
	WWDG_SetWValue(127);
	/*
	Enable WWDG and set counter value to 127, WWDG timeout = ~2049 us * (127-63) = 131.136 ms
	In this case the refresh window is: ~2049 us * (127-127) =0ms < refresh window < ~2049us * 64 = 131.136 ms
	*/
	WWDG_Enable(127);
#endif
#else
	RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_WWDG, DISABLE);	  
	#ifndef DEBUG		
		RCC_EnableLsi(ENABLE);
		while (RCC_GetFlagStatus(RCC_CTRLSTS_FLAG_LSIRD) == RESET);
		
		IWDG_WriteConfig(IWDG_WRITE_ENABLE);
		IWDG_SetPrescalerDiv(IWDG_PRESCALER_DIV256);
		IWDG_CntReload(2048);
		IWDG_ReloadKey();
		IWDG_Enable();
	#endif

#endif
}
/******************************************************************************
#ifdef DOC
FUNCTION		: vBSP_WDG_Refresh
DESCRIPTION		: 
INPUT			: none
OUTPUT			: none
UPDATE			: -
DATE			: 2019/5/17
******************************************************************************/
void vBSP_WDG_Refresh( void )
{
  
//  	static char st = 0;
//    st = !st;
//    GPIO_WriteBit(GPIOC, GPIO_PIN_13, (Bit_OperateType)st);
  
  
#if 0
#ifndef DEBUG
	WWDG_SetCnt(127);
#endif
#else
#ifndef DEBUG
	IWDG_ReloadKey();
#endif
#endif
}

/******************************************************************************
#ifdef DOC
FUNCTION		: vBSP_WDG_Suspend
DESCRIPTION		: 
INPUT			: none
OUTPUT			: none
UPDATE			: -
DATE			: 2019/5/17
******************************************************************************/
void vBSP_WDG_Suspend( void )
{
#if 0
#ifndef DEBUG
	RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_WWDG, DISABLE);	  
#endif
#else

#endif

}

/******************************************************************************
#ifdef DOC
FUNCTION		: vBSP_WDG_Resume
DESCRIPTION		: 
INPUT			: none
OUTPUT			: none
UPDATE			: -
DATE			: 2019/5/17
******************************************************************************/
void vBSP_WDG_Resume( void )
{
#if 0
#ifndef DEBUG
    vBSP_WDG_Init();
#endif
#else
	
#endif

}






