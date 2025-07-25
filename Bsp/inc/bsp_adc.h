/****************************************************************************
#ifdef DOC

File Name		:bsp_adc.h

Description		:

Remark			:

Date			:19/05/17

Copyright		:Techsum Power Technology Co., Ltd.

#endif
*****************************************************************************/
#ifndef		BSP_ADC_H
#define		BSP_ADC_H

#include "sys_type.h"


/****************************************************************************
 define macro
*****************************************************************************/
#define ADC_REF_VOLT        (3000)
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
void vBSP_ADC_Init(void);
void vBSP_ADC_DeInit(void);

void vBsp_Adc_ShutDown(void);

USHORT usGetMCUADCVolt(UCHAR channel);

uint32_t vPower_Motor(void);

uint32_t vPower_Device(void);

#endif	/* ADC_H */

