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
 * @file    cmt2300_hal.c
 * @brief   CMT2300 hardware abstraction layer
 *
 * @version 1.1
 * @date    Feb 08 2017
 * @author  CMOSTEK R@D
 */
 
#include "cmt2300_hal.h"
#include "cmt_spi3.h"


/*! ********************************************************
* @name    Cmt2300_InitGpio
* @desc    Initializes the CMT2300 interface GPIOs.
* *********************************************************/
void Cmt2300_InitGpio(void)
{
    cmt_spi3_init();
}

/*! ********************************************************
* @name    Cmt2300_ReadReg
* @desc    Read the CMT2300 register at the specified address.
* @param   addr: register address
* @return  Register value
* *********************************************************/
u8 Cmt2300_ReadReg(u8 addr)
{
    u8 dat = 0xFF;
    cmt_spi3_read(addr, &dat);
	
    return dat;
}

/*! ********************************************************
* @name    Cmt2300_WriteReg
* @desc    Write the CMT2300 register at the specified address.
* @param   addr: register address
*          dat: register value
* *********************************************************/
void Cmt2300_WriteReg(u8 addr, u8 dat)
{
    cmt_spi3_write(addr, dat);
}

/*! ********************************************************
* @name    Cmt2300_ReadFifo
* @desc    Reads the contents of the CMT2300 FIFO.
* @param   buf: buffer where to copy the FIFO read data
*          len: number of bytes to be read from the FIFO
* *********************************************************/
void Cmt2300_ReadFifo(u8 buf[], u16 len)
{
    cmt_spi3_read_fifo(buf, len);
}

/*! ********************************************************
* @name    Cmt2300_WriteFifo
* @desc    Writes the buffer contents to the CMT2300 FIFO.
* @param   buf: buffer containing data to be put on the FIFO
*          len: number of bytes to be written to the FIFO
* *********************************************************/
void Cmt2300_WriteFifo(const u8 buf[], u16 len)
{
    cmt_spi3_write_fifo(buf, len);
}
