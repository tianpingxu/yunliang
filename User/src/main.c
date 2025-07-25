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
 * @file main.c
 * @author Nations
 * @version V1.2.2
 *
 * @copyright Copyright (c) 2022, Nations Technologies Inc. All rights reserved.
 */
#include "sys_type.h"
#include "n32l40x.h"
#include "bsp_io.h"
#include "bsp_adc.h"
#include "bsp_usart.h"
#include "bsp_timer.h"

#include "485.h"
#include "protocol.h"

#include "oled.h"
#include "oled_show.h"

#include "delay.h"
#include "motor.h"

#include "rf.h"
#include "radio.h"
#include "cmt_spi3.h"
#include "function.h"

void SystemClock_Config(void);

uint16_t dev_vol = 0;
uint16_t motor_vol = 0;

//485
char *pbuf;
uint16_t len;

uint8_t beep_flag = 0;
uint8_t beep_cnt50ms = 0;

uint8_t beep_flag_first = 0;

uint8_t motor_state_first = 0;
 
/**
 * @brief   Main program
 */
int main(void)
{
	SystemClock_Config();
    // SysTick_Config(SystemCoreClock);
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	vBSP_Port_FullSpeed();
	vBSP_USARTInit(USART1);
	printf("vBSP_USART1Init\r\n");

	RFInit();
	RFRxMode();
	OLED_Init();
	rs485_init();
	motor_init();
	vBSP_ADC_Init();
	vBSP_Timer2Init();

	dev_vol = vPower_Device();
	oled_show_line2(dev_vol / 10);

	hall_detect_power();

	beep_flag_first = 1;

	while(1) {
		// 霍尔感应检测
		hall_detect();
		if(ucTIM_50msFlg) {
			ucTIM_50msFlg = FALSE;
			// 工作电压检测
			dev_vol = vPower_Device();
			oled_show_line2(dev_vol / 10);
			motor_vol = vPower_Motor();
			// 上电提醒
			if(beep_flag_first) {
				BEEP_ON();
				beep_cnt50ms++;
				if(beep_cnt50ms >= 10) {
					BEEP_OFF();
					beep_cnt50ms = 0;
					beep_flag_first = 0;
				}
			}
		}

		if(ucTIM_20msFlg) {
			ucTIM_20msFlg = FALSE;
			// 485数据处理
			pbuf = get_rebuff(&len);
			if(len >= 6) {
				rx_data_hadle((uint8_t *)pbuf, len);
				clean_rebuff();
			}
			rotate_over_time();
		}
		// radio
		RFRevData(revBuf);

	}
}


void SystemClock_Config(void)
{
    ULONG pllsrcclk;
    ULONG pllmul;
	ULONG latency;
    ErrorStatus HSIStartUpStatus;

    SystemCoreClock = 16000000;

	RCC_EnableHsi(ENABLE);    
	/* Wait till HSI is ready */
	HSIStartUpStatus = RCC_WaitHsiStable();
	if (HSIStartUpStatus == SUCCESS)
	{
		pllsrcclk = HSI_VALUE/2;
		latency = (SystemCoreClock/32000000);
		pllmul = (SystemCoreClock / pllsrcclk);
		pllmul = ((pllmul - 2) << 18);

		FLASH_SetLatency(latency);

		/* HCLK = SYSCLK */
		RCC_ConfigHclk(RCC_SYSCLK_DIV1);

		/* PCLK2 = HCLK */
		RCC_ConfigPclk2(RCC_HCLK_DIV1);

		/* PCLK1 = HCLK */
		RCC_ConfigPclk1(RCC_HCLK_DIV1);

		/* Disable PLL */
		RCC_EnablePll(DISABLE);

		RCC_ConfigPll(RCC_PLL_HSI_PRE_DIV2, pllmul, RCC_PLLDIVCLK_DISABLE);

		/* Enable PLL */
		RCC_EnablePll(ENABLE);

		/* Wait till PLL is ready */
		while (RCC_GetFlagStatus(RCC_CTRL_FLAG_PLLRDF) == RESET);

		/* Select PLL as system clock source */
		RCC_ConfigSysclk(RCC_SYSCLK_SRC_PLLCLK);

		/* Wait till PLL is used as system clock source */
		while (RCC_GetSysclkSrc() != 0x0C);
	}
    RCC_EnableHsi(ENABLE);

    /* Wait till HSI is ready */
    HSIStartUpStatus = RCC_WaitHsiStable();

    if (HSIStartUpStatus == SUCCESS)
    {
        /* Enable Prefetch Buffer */
        FLASH_PrefetchBufSet(FLASH_PrefetchBuf_EN);

        /* Select HSI as system clock source */
        RCC_ConfigSysclk(RCC_SYSCLK_SRC_HSI);

        /* Wait till HSI is used as system clock source */
        while (RCC_GetSysclkSrc() != 0x04)
        {
        }

        /* Flash 0 wait state */
        FLASH_SetLatency(FLASH_LATENCY_0);

        /* HCLK = SYSCLK */
        RCC_ConfigHclk(RCC_SYSCLK_DIV1);

        /* PCLK2 = HCLK */
        RCC_ConfigPclk2(RCC_HCLK_DIV1);

        /* PCLK1 = HCLK */
        RCC_ConfigPclk1(RCC_HCLK_DIV1);
    }
    else
    {
        /* If HSI fails to start-up, the application will have wrong clock
           configuration. User can add here some code to deal with this error */

        /* Go to infinite loop */
        while (1)
        {
        }
    }
	/* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
}


#ifdef USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param file pointer to the source file name
 * @param line assert_param error line source number
 */
void assert_failed(const uint8_t* expr, const uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}

#endif

/**
 * @}
 */

/**
 * @}
 */
