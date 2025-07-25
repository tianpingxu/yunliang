/*
 * THE FOLLOWING FIRMWARE IS PROVIDED: (1) "AS IS" WITH NO WARRANTY; AND
 * (2)TO ENABLE ACCESS TO CODING INFORMATION TO GUIDE AND FACILITATE CUSTOMER.
 * CONSEQUENTLY, CMOSTEK SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * Copyright (C) CMOSTEK SZ.
 */

/*!
 * @file    cmt2300_hal.h
 * @brief   CMT2300 hardware abstraction layer
 *
 * @version 1.1
 * @date    Feb 08 2017
 * @author  CMOSTEK R@D
 */
 
#ifndef __CMT2300_HAL_H
#define __CMT2300_HAL_H

#include "typedefs.h"
#include "n32l40x.h"

#ifdef __cplusplus 
extern "C" { 
#endif

void Cmt2300_InitGpio(void);

u8 Cmt2300_ReadReg(u8 addr);
void Cmt2300_WriteReg(u8 addr, u8 dat);

void Cmt2300_ReadFifo(u8 buf[], u16 len);
void Cmt2300_WriteFifo(const u8 buf[], u16 len);

#ifdef __cplusplus 
} 
#endif

#endif
