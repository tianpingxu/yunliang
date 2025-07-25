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
 * @file    cmt2300.h
 * @brief   CMT2300 transceiver RF chip driver
 *
 * @version 1.2
 * @date    Feb 08 2017
 * @author  CMOSTEK R@D
 */

#ifndef __CMT2300_H
#define __CMT2300_H

#include "typedefs.h"
#include "cmt2300_defs.h"
#include "cmt2300_hal.h"
#include "cmt_spi3.h"

#ifdef __cplusplus 
extern "C" { 
#endif

#define ENABLE_WAIT_CHIP_STATUS   /* Enable the wait when switch chip status */


/* ************************************************************************
   The following are for chip status controls.
*  ************************************************************************ */
void Cmt2300_SoftReset(void);
u8 Cmt2300_GetChipStatus(void);
BOOL_ Cmt2300_WaitChipStatus(u8 nStatus);
BOOL_ Cmt2300_GoSleep(void);
BOOL_ Cmt2300_GoStby(void);
BOOL_ Cmt2300_GoTFS(void);
BOOL_ Cmt2300_GoRFS(void);
BOOL_ Cmt2300_GoTx(void);
BOOL_ Cmt2300_GoRx(void);


/* ************************************************************************
*  The following are for chip interrupts, GPIO, FIFO operations.
*  ************************************************************************ */
void Cmt2300_ConfigGpio(u8 nGpioSel);
void Cmt2300_ConfigInterrupt(u8 nInt1Sel, u8 nInt2Sel);
void Cmt2300_SetInterruptPolar(BOOL_ bActiveHigh);
void Cmt2300_SetFifoThreshold(u8 nFifoThreshold);
void Cmt2300_EnableAntennaSwitch(u8 nMode);
void Cmt2300_EnableInterrupt(u8 nEnable);
void Cmt2300_EnableRxFifoAutoClear(BOOL_ bEnable);
void Cmt2300_EnableFifoMerge(BOOL_ bEnable);
void Cmt2300_EnableReadFifo(void);
void Cmt2300_EnableWriteFifo(void);
void Cmt2300_RestoreFifo(void);
u8 Cmt2300_ClearFifo(void);
u8 Cmt2300_ClearInterruptFlags(void);


/* ************************************************************************
*  The following are for Tx DIN operations in direct mode.
*  ************************************************************************ */
void Cmt2300_ConfigTxDin(u8 nDinSel);
void Cmt2300_EnableTxDin(BOOL_ bEnable);
void Cmt2300_EnableTxDinInvert(BOOL_ bEnable);


/* ************************************************************************
*  The following are general operations.
*  ************************************************************************ */
BOOL_ Cmt2300_IsExist(void);
u8 Cmt2300_GetRssiCode(void);
int Cmt2300_GetRssiDBm(void);
void Cmt2300_SetFrequencyChannel(u8 nChann);
void Cmt2300_SetFrequencyStep(u8 nOffset);
void Cmt2300_SetPayloadLength(u16 nLength);
void Cmt2300_EnableLfosc(BOOL_ bEnable);
void Cmt2300_EnableLfoscOutput(BOOL_ bEnable);


/* ************************************************************************
*  The following are for chip initializes.
*  ************************************************************************ */
void Cmt2300_Init(void);
BOOL_ Cmt2300_ConfigRegBank(u8 base_addr, const u8 bank[], u8 len);

#ifdef __cplusplus
} 
#endif

#endif
