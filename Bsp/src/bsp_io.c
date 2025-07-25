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
#include "sys_type.h"
#include "n32l40x.h"
#include "bsp_io.h"
#include "app_io.h"
#include "app_system.h"

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
void vBSP_GpioDeinit(GPIO_Module* GPIOx, USHORT Pin)
{
    ULONG position = 0x00U;
    ULONG iocurrent = 0x00U;
    ULONG tmp = 0x00U;
    ULONG GPIO_Pin = Pin;
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    /* Check the parameters */
    assert_param(IS_GPIO_PIN_AVAILABLE(GPIOx,GPIO_Pin));
    /* Configure the port pins */
    while ((GPIO_Pin >> position) != 0)
    {
        /* Get the IO position */
        iocurrent = (GPIO_Pin) & ((ULONG)0x01 << position);

        if(iocurrent)
        {
            /*------------------------- EXTI Mode Configuration --------------------*/
            /* Clear the External Interrupt or Event for the current IO */
            tmp = AFIO->EXTI_CFG[position>>2];
            tmp &= (0x0FuL << (4u*(position & 0x03u)));
            if(tmp == (GPIO_GET_INDEX(GPIOx)<<(4u * (position & 0x03u))))
            {
                /* Clear EXTI line configuration */
                EXTI->IMASK &= ~(iocurrent);
                EXTI->EMASK &= ~(iocurrent);
                
                /* Clear Rising Falling edge configuration */
                EXTI->RT_CFG &= ~(iocurrent);
                EXTI->FT_CFG &= ~(iocurrent);
                tmp = 0x0FuL << (4u * (position & 0x03u));
                AFIO->EXTI_CFG[position >> 2u] &= ~tmp;
            }
            
            
            /*------------------------- GPIO Mode Configuration --------------------*/
            /* Configure IO Direction in Input Floting Mode */
            GPIOx->PMODE &= ~(GPIO_PMODE0_Msk << (position * 2U));

            /* Configure the default Alternate Function in current IO */
            if(position & 0x08)
                GPIOx->AFH |= ((ULONG)0xF << ((ULONG)(position & (ULONG)0x07) * 4U));
            else
                GPIOx->AFL |= ((ULONG)0xF << ((ULONG)(position & (ULONG)0x07) * 4U));
          
            /* Configure the default value IO Output Type */
            GPIOx->POTYPE  &= ~(GPIO_POTYPE_POT_0 << position) ;
          
            /* Deactivate the Pull-up oand Pull-down resistor for the current IO */
            GPIOx->PUPD &= ~(GPIO_PUPD0_Msk << (position * 2U));
      
        }
     position++;
    }
}

/******************************************************************************
#ifdef DOC
FUNCTION        : vPort_FullSpeed
DESCRIPTION     : The GPIO configuration of Full Speed Mode
INPUT           : none
OUTPUT          : none
UPDATE          : -
DATE            : 2019/5/27
******************************************************************************/
void vBSP_Port_FullSpeed(void)
{
    GPIO_InitType GPIO_InitStructure;    
	EXTI_InitType EXTI_InitStructure;
    NVIC_InitType NVIC_InitStructure;
    
    /* Configure all GPIO as analog to reduce current consumption on non used IOs */
    /* Warning : Reconfiguring all GPIO will close the connexion with the debugger */
    /* Enable GPIOs clock */
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB, ENABLE);
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOC, ENABLE);
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOD, ENABLE);
    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_AFIO, ENABLE);

    /*
    	beep
	*/
    GPIO_InitStructure.Pin		  	= GPIO_PIN_0;
    GPIO_InitStructure.GPIO_Pull	= GPIO_No_Pull;
    GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
    
    BEEP_OFF();
}


void HAL_GPIO_EXTI_Callback(USHORT GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_15)
	{
		if(P_HALL_A_IS_PRESS())
		{
			
		}
	}
	else if(GPIO_Pin == GPIO_PIN_14)
	{
		if(P_HALL_B_IS_PRESS())
		{
			
		}
	}
}


