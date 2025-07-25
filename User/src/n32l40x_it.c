/*****************************************************************************
 * Copyright (c) 2022, Nations Technologies Inc.
 *
 * All rights reserved.
 * ****************************************************************************
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Nations' name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY NATIONS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL NATIONS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ****************************************************************************/

/**
 * @file n32l40x_it.c
 * @author Nations
 * @version V1.2.2
 *
 * @copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
 */
#include "n32l40x.h"
#include "sys_type.h"
#include "misc.h"
#include "n32l40x_it.h"
#include "bsp_timer.h"
/** @addtogroup N32L40x_StdPeriph_Template
 * @{
 */

void SYSTICK_Callback(void);
void HAL_TIM_PeriodElapsedCallback(TIM_Module *htim);
void HAL_GPIO_EXTI_Callback(USHORT GPIO_Pin);
void HAL_UART_RxCpltCallback(USART_Module *huart);
void HAL_UART_IdleCallback(USART_Module *huart);
void UART_TxCallback(USART_Module* USARTx);
void HAL_UART_ErrorCallback(USART_Module *huart);

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

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
 * @brief  This function handles SysTick Handler.
 */
void SysTick_Handler(void)
{
	NVIC_ClearPendingIRQ(SysTick_IRQn);

    /* USER CODE END SysTick_IRQn 0 */
	  HAL_IncTick();
    // SYSTICK_Callback();
}


// void TIM2_IRQHandler(void)
// {
// 	if (TIM_GetIntStatus(TIM2, TIM_INT_UPDATE) != RESET)
// 	{
// 		TIM_ClrIntPendingBit(TIM2, TIM_INT_UPDATE);
// 		HAL_TIM_PeriodElapsedCallback(TIM2);
// 	}
// }

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
//     if (USART_GetIntStatus(USART2, USART_INT_RXDNE) != RESET)
//     {
// 	    USART_ClrIntPendingBit(USART2,USART_INT_RXDNE);
//      HAL_UART_RxCpltCallback(USART2);
//     }

//     if (USART_GetIntStatus(USART2, USART_INT_TXC) != RESET)
//     {
// 	    USART_ClrIntPendingBit(USART2,USART_INT_TXC);
// 		UART_TxCallback(USART2);
//     }
	
//     if (USART_GetIntStatus(USART2, USART_INT_IDLEF) != RESET)
//     {
// 		USART_GetFlagStatus(USART2, USART_INT_IDLEF);
// 		USART_ReceiveData(USART2);
// 		HAL_UART_IdleCallback(USART2);
//     }
    
//     if (USART_GetIntStatus(USART2, USART_INT_ERRF) != RESET)
//     {
// 		HAL_UART_ErrorCallback(USART2);
//     }
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

/******************************************************************************/
/*                 N32L40x Peripherals Interrupt Handlers                     */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_n32l40x.s).                                                 */
/******************************************************************************/

/**
 * @brief  This function handles PPP interrupt request.
 */
/*void PPP_IRQHandler(void)
{
}*/

/**
 * @}
 */
