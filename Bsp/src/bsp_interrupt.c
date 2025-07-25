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
#include "sys_type.h"
#include "misc.h"
#include "bsp_timer.h"

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
void SYSTICK_Callback(void);
void HAL_RTCEx_WakeUpTimerEventCallback(void);
void HAL_TIM_PeriodElapsedCallback(TIM_Module *htim);
void HAL_GPIO_EXTI_Callback(USHORT GPIO_Pin);
void HAL_UART_ErrorCallback(USART_Module *huart);
void HAL_UART_RxCpltCallback(USART_Module *huart);
void HAL_UART_TxCpltCallback(USART_Module *huart);
void HAL_UART_IdleCallback(USART_Module *huart);
void UART_TxCallback(USART_Module* USARTx);

/******************************************************************************
#ifdef DOC
FUNCTION		: 
DESCRIPTION		: 
INPUT			: none
OUTPUT			: none
UPDATE			: -
DATE			: 2019/5/17
******************************************************************************/

/**
 * @brief  This function handles NMI exception.
 */
void NMI_Handler(void)
{
}

/**
 * @brief  This function handles Hard Fault exception.
 */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/**
 * @brief  This function handles Memory Manage exception.
 */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}
/**
 * @brief  This function handles Bus Fault exception.
 */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
 * @brief  This function handles Usage Fault exception.
 */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/**
 * @brief  This function handles SVCall exception.
 */
void SVC_Handler(void)
{
}

/**
 * @brief  This function handles Debug Monitor exception.
 */
void DebugMon_Handler(void)
{
}


/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
    /* USER CODE BEGIN SysTick_IRQn 0 */
	NVIC_ClearPendingIRQ(SysTick_IRQn);

    /* USER CODE END SysTick_IRQn 0 */
	HAL_IncTick();
    // SYSTICK_Callback();
    /* USER CODE BEGIN SysTick_IRQn 1 */

    /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/*                 STM32F1xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f1xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
	if (TIM_GetIntStatus(TIM2, TIM_INT_UPDATE) != RESET)
	{
		TIM_ClrIntPendingBit(TIM2, TIM_INT_UPDATE);
		HAL_TIM_PeriodElapsedCallback(TIM2);
	}
}
// void TIM3_IRQHandler(void)
// {
// 	if (TIM_GetIntStatus(TIM3, TIM_INT_UPDATE) != RESET)
// 	{
// 		TIM_ClrIntPendingBit(TIM3, TIM_INT_UPDATE);
// 		HAL_TIM_PeriodElapsedCallback(TIM3);
// 	}
// }

void EXTI0_IRQHandler(void)
{
    if (RESET != EXTI_GetITStatus(EXTI_LINE0))
    {
        EXTI_ClrITPendBit(EXTI_LINE0);
		HAL_GPIO_EXTI_Callback(GPIO_PIN_0);
    }
}
void EXTI1_IRQHandler(void)
{
    if (RESET != EXTI_GetITStatus(EXTI_LINE1))
    {
        EXTI_ClrITPendBit(EXTI_LINE1);
		HAL_GPIO_EXTI_Callback(GPIO_PIN_1);
    }
}
void EXTI2_IRQHandler(void)
{
    if (RESET != EXTI_GetITStatus(EXTI_LINE2))
    {
        EXTI_ClrITPendBit(EXTI_LINE2);
		HAL_GPIO_EXTI_Callback(GPIO_PIN_2);
    }
}
void EXTI3_IRQHandler(void)
{
    if (RESET != EXTI_GetITStatus(EXTI_LINE3))
    {
        EXTI_ClrITPendBit(EXTI_LINE3);
		HAL_GPIO_EXTI_Callback(GPIO_PIN_3);
    }
}
void EXTI4_IRQHandler(void)
{
    if (RESET != EXTI_GetITStatus(EXTI_LINE4))
    {
        EXTI_ClrITPendBit(EXTI_LINE4);
		HAL_GPIO_EXTI_Callback(GPIO_PIN_4);
    }
}
void EXTI9_5_IRQHandler(void)
{
	USHORT GPIO_Pin;

	for(ULONG exit_line = EXTI_LINE5;exit_line <= EXTI_LINE9;exit_line <<= 1)
	{
		if (RESET != EXTI_GetITStatus(exit_line))
		{
			EXTI_ClrITPendBit(exit_line);
			GPIO_Pin = (USHORT)exit_line;
			HAL_GPIO_EXTI_Callback(GPIO_Pin);
		}
	}
}
void EXTI15_10_IRQHandler(void)
{
	USHORT GPIO_Pin;

	for(ULONG exit_line = EXTI_LINE10;exit_line <= EXTI_LINE15;exit_line <<= 1)
	{
		if (RESET != EXTI_GetITStatus(exit_line))
		{
			EXTI_ClrITPendBit(exit_line);
			GPIO_Pin = (USHORT)exit_line;
			HAL_GPIO_EXTI_Callback(GPIO_Pin);
		}
	}
}


void USART1_IRQHandler(void)
{
    if (USART_GetIntStatus(USART1, USART_INT_RXDNE) != RESET)
    {
	    USART_ClrIntPendingBit(USART1,USART_INT_RXDNE);
    	HAL_UART_RxCpltCallback(USART1);
    }

    if (USART_GetIntStatus(USART1, USART_INT_TXC) != RESET)
    {
	    USART_ClrIntPendingBit(USART1,USART_INT_TXC);
		UART_TxCallback(USART1);
    }
	
    if (USART_GetIntStatus(USART1, USART_INT_IDLEF) != RESET)
    {
		USART_ReceiveData(USART1);
		HAL_UART_IdleCallback(USART1);
    }
    
    if (USART_GetIntStatus(USART1, USART_INT_ERRF) != RESET)
    {
		HAL_UART_ErrorCallback(USART1);
    }
}

// void USART2_IRQHandler(void)
// {
//    if (USART_GetIntStatus(USART2, USART_INT_RXDNE) != RESET)
//    {
// 	    USART_ClrIntPendingBit(USART2,USART_INT_RXDNE);
//    	HAL_UART_RxCpltCallback(USART2);
//    }

//    if (USART_GetIntStatus(USART2, USART_INT_TXC) != RESET)
//    {
// 	    USART_ClrIntPendingBit(USART2,USART_INT_TXC);
// 		UART_TxCallback(USART2);
//    }
	
//    if (USART_GetIntStatus(USART2, USART_INT_IDLEF) != RESET)
//    {
// 		USART_GetFlagStatus(USART2, USART_INT_IDLEF);
// 		USART_ReceiveData(USART2);
// 		HAL_UART_IdleCallback(USART2);
//    }
   
//    if (USART_GetIntStatus(USART2, USART_INT_ERRF) != RESET)
//    {
// 		HAL_UART_ErrorCallback(USART2);
//    }
// }

void USART3_IRQHandler(void)
{
    if (USART_GetIntStatus(USART3, USART_INT_RXDNE) != RESET)
    {
	    USART_ClrIntPendingBit(USART3,USART_INT_RXDNE);
    	HAL_UART_RxCpltCallback(USART3);
    }

    if (USART_GetIntStatus(USART3, USART_INT_TXC) != RESET)
    {
	    USART_ClrIntPendingBit(USART3,USART_INT_TXC);
		UART_TxCallback(USART3);
    }
	
    if (USART_GetIntStatus(USART3, USART_INT_IDLEF) != RESET)
    {
		USART_GetFlagStatus(USART3, USART_INT_IDLEF);
		USART_ReceiveData(USART3);
		HAL_UART_IdleCallback(USART3);
    }
    
    if (USART_GetIntStatus(USART3, USART_INT_ERRF) != RESET)
    {
		HAL_UART_ErrorCallback(USART3);
    }
}


void UART5_IRQHandler(void)
{
    if (USART_GetIntStatus(UART5, USART_INT_RXDNE) != RESET)
    {
	    USART_ClrIntPendingBit(UART5,USART_INT_RXDNE);
    	HAL_UART_RxCpltCallback(UART5);
    }

    if (USART_GetIntStatus(UART5, USART_INT_TXC) != RESET)
    {
	    USART_ClrIntPendingBit(UART5,USART_INT_TXC);
		UART_TxCallback(UART5);
    }
	
    if (USART_GetIntStatus(UART5, USART_INT_IDLEF) != RESET)
    {
		USART_GetFlagStatus(UART5, USART_INT_IDLEF);
		USART_ReceiveData(UART5);
		HAL_UART_IdleCallback(UART5);
    }
    
    if (USART_GetIntStatus(UART5, USART_INT_ERRF) != RESET)
    {
		HAL_UART_ErrorCallback(UART5);
    }
}

// void RTC_WKUP_IRQHandler(void)
// {
//     if (RTC_GetITStatus(RTC_INT_WUT) != RESET)
//     {
//         RTC_ClrIntPendingBit(RTC_INT_WUT);
//         EXTI_ClrITPendBit(EXTI_LINE20);
// 		HAL_RTCEx_WakeUpTimerEventCallback();
//     }
// }


/**
  * @}
  */


