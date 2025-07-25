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
#include "bsp_io.h"
#include "bsp_wdg.h"
#include "bsp_adc.h"
#include "bsp_timer.h"

/****************************************************************************
 define macro
*****************************************************************************/

/****************************************************************************
 define type (STRUCTURE, UNION)
*****************************************************************************/

/****************************************************************************
 Private variables
*****************************************************************************/

/****************************************************************************
 Variable declaration
*****************************************************************************/


/****************************************************************************
 Prototype declaration
*****************************************************************************/


/******************************************************************************
#ifdef DOC
FUNCTION		: HAL_ADC_MspInit
DESCRIPTION		: 
INPUT			: none
OUTPUT			: none
UPDATE			: -
DATE			: 2019/6/6
******************************************************************************/
void HAL_ADC_MspInit(void)
{
  GPIO_InitType GPIO_InitStructure;
/* revision # 004, TP-BA150H, ruijianqiang, 20200813 S */
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO clock ****************************************/
  RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);
  /*##-2- Configure peripheral GPIO ##########################################*/
  /* ADC Channel GPIO pin configuration */
  GPIO_InitStruct(&GPIO_InitStructure);
  GPIO_InitStructure.Pin	   = GPIO_PIN_1 | GPIO_PIN_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Analog;
  GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
  
  /*##-3- Configure the NVIC #################################################*/
  /* NVIC configuration for DMA transfer complete interrupt (USART1_TX) */
}
/******************************************************************************
#ifdef DOC
FUNCTION		: ADC_Init
DESCRIPTION		: 
INPUT			: none
OUTPUT			: none
UPDATE			: -
DATE			: 2019/6/6
******************************************************************************/
void HAL_ADC_MspDeInit(void)
{

  /*##-1- Reset peripherals ##################################################*/
  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* De-initialize the ADC Channel GPIO pin */
	vBSP_GpioDeinit(GPIOA, GPIO_PIN_1);
	vBSP_GpioDeinit(GPIOA, GPIO_PIN_2);
}

/******************************************************************************
#ifdef DOC
FUNCTION		: ADC_Init
DESCRIPTION		: 
INPUT			: none
OUTPUT			: none
UPDATE			: -
DATE			: 2019/6/6
******************************************************************************/
void vBSP_ADC_Init(void)
{
	ADC_InitType ADC_InitStructure;

	/* Enable ADC clocks */
	RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_ADC, ENABLE);
	/* RCC_ADCHCLK_DIV4*/
	ADC_ConfigClk(ADC_CTRL3_CKMOD_AHB, RCC_ADCHCLK_DIV4);
	RCC_ConfigAdc1mClk(RCC_ADC1MCLK_SRC_HSI, RCC_ADC1MCLK_DIV16);  //selsect HSI as RCC ADC1M CLK Source   


	// vBSP_ADC_DeInit();

	HAL_ADC_MspInit();

	/* ADC configuration ------------------------------------------------------*/
	ADC_InitStructure.MultiChEn	   = DISABLE;
	ADC_InitStructure.ContinueConvEn = DISABLE;
	ADC_InitStructure.ExtTrigSelect  = ADC_EXT_TRIGCONV_NONE;
	ADC_InitStructure.DatAlign	   = ADC_DAT_ALIGN_R;
	ADC_InitStructure.ChsNumber	   = 1;
	ADC_Init(ADC, &ADC_InitStructure);
	
	/* Enable ADC */
	ADC_Enable(ADC, ENABLE);
	/* Check ADC Ready */
	while(ADC_GetFlagStatusNew(ADC,ADC_FLAG_RDY) == RESET);
	/* Start ADC1 calibration */
	ADC_StartCalibration(ADC);
	/* Check the end of ADC1 calibration */
	while (ADC_GetCalibrationStatus(ADC));

	// ADC_ConfigRegularChannel(ADC, ADC_CH_3_PA2, 1, ADC_SAMP_TIME_28CYCLES5);
}
/******************************************************************************
#ifdef DOC
FUNCTION		: ADC_DeInit
DESCRIPTION		: 
INPUT			: none
OUTPUT			: none
UPDATE			: -
DATE			: 2019/6/6
******************************************************************************/
void vBSP_ADC_DeInit(void)
{
	HAL_ADC_MspDeInit();
    ADC_DeInit(ADC);
}


UCHAR vBsp_ADC_PollForConversion(USHORT TimeOut)
{
	UCHAR wk_ret = TRUE;
	ULONG wk_start = 0;
    
	// wk_start = HAL_GetTick();
	while(ADC_GetFlagStatus(ADC,ADC_FLAG_ENDC)==0)
	{
		// vBSP_WDG_Refresh();
		// if(HAL_GetTick() - wk_start > TimeOut)
		// {
			// wk_ret = FALSE;
		// 	break;
		// }
		wk_start++;
		vBSP_DelayMs(1);
		if (wk_start >= TimeOut) {
			wk_ret = FALSE;
			break;
		}
	}

	return wk_ret;
}

/******************************************************************************
#ifdef DOC
FUNCTION		: 
DESCRIPTION		: 
INPUT			: none          
OUTPUT			: none
UPDATE			: -
DATE			: 2019/6/6
******************************************************************************/ 
USHORT usGetMCUADCVolt(UCHAR channel)
{
    USHORT usAdcValue = 0;
	USHORT usVolt = 0;    
	USHORT wk_vol[5];
	UCHAR wk_idx;
	USHORT wk_max = 0;
	USHORT wk_min = 0xFFFF;

	/*ADC_CH_2_PA1*/
	/*ADC_CH_3_PA2*/
	ADC_ConfigRegularChannel(ADC, channel, 1, ADC_SAMP_TIME_28CYCLES5);
	for(wk_idx = 0;wk_idx < 5;wk_idx++)
	{
		ADC_EnableSoftwareStartConv(ADC,ENABLE);
		if(vBsp_ADC_PollForConversion(10) == TRUE)
		{
			ADC_ClearFlag(ADC,ADC_FLAG_ENDC);
			ADC_ClearFlag(ADC,ADC_FLAG_STR);
			wk_vol[wk_idx]=ADC_GetDat(ADC);
			usAdcValue+=wk_vol[wk_idx];
			wk_max = (wk_vol[wk_idx] > wk_max)?wk_vol[wk_idx]:wk_max;
			wk_min = (wk_vol[wk_idx] < wk_min)?wk_vol[wk_idx]:wk_min;
		}
	}
	usAdcValue = (usAdcValue - wk_max - wk_min) / 3;
	usVolt = (ULONG)usAdcValue * ADC_REF_VOLT / 4096;
    return usVolt;
}

#define  ADC_BUFF_MAX	(6)
SHORT Power_Voltage;
static LONG adc_buff[ADC_BUFF_MAX];

LONG adc_buff_handle(LONG value)
{
    LONG adc_value_min;
    LONG adc_value_max;

    static uint8_t buff_cnt=0;
    static LONG new_value=0;
    adc_buff[buff_cnt++] = value;
    if(buff_cnt == ADC_BUFF_MAX)
    {
        LONG total_value=adc_buff[0];
        adc_value_min = adc_buff[0];
        adc_value_max = adc_buff[0];
		uint8_t i;
        for(i = 1; i<ADC_BUFF_MAX; i++)
        {
            total_value+=adc_buff[i];
            adc_value_max = (adc_buff[i] > adc_value_max)?adc_buff[i]:adc_value_max;
            adc_value_min = (adc_buff[i] < adc_value_min)?adc_buff[i]:adc_value_min;
        }
        buff_cnt=0;
        new_value = (total_value -adc_value_max -adc_value_min) /(ADC_BUFF_MAX-2);
    }
    if(new_value == 0)
        return value;
    return new_value;
}

uint32_t vPower_Motor(void){
	USHORT wk_motor_vol = 0;

	wk_motor_vol = usGetMCUADCVolt(ADC_CH_2_PA1);

	// printf("motor_vol = %d\r\n", wk_motor_vol);
	return wk_motor_vol;
}

uint32_t vPower_Device(void)
{
	USHORT wk_dev_vol = 0;

	wk_dev_vol = usGetMCUADCVolt(ADC_CH_3_PA2);
	wk_dev_vol = (ULONG)wk_dev_vol * (512 + 82) / 82;
	Power_Voltage = adc_buff_handle(wk_dev_vol);

    // printf("dev_vol = %d\r\n", Power_Voltage);
	return Power_Voltage;
}


void vBsp_Adc_ShutDown(void)
{
	vBSP_ADC_DeInit();
}
