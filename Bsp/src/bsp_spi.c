#include "sys_type.h"
#include "n32l40x.h"
#include "bsp_spi.h"
#include "bsp_timer.h"


void vSPI_MspInit(void)
{
	GPIO_InitType GPIO_InitStructure;
	
	GPIO_InitStruct(&GPIO_InitStructure);
	
	/*!< sFLASH_SPI_CS_GPIO, sFLASH_SPI_MOSI_GPIO, sFLASH_SPI_MISO_GPIO
		 and sFLASH_SPI_SCK_GPIO Periph clock enable */
	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA, ENABLE);
	
	/*!< sFLASH_SPI Periph clock enable */
	RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_SPI1|RCC_APB2_PERIPH_AFIO, ENABLE);
	
	/*!< Configure sFLASH_SPI pins: SCK */
	GPIO_InitStructure.Pin		  = GPIO_PIN_5;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Alternate = GPIO_AF0_SPI1;
	GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
	
	/*!< Configure sFLASH_SPI pins: MOSI */
	GPIO_InitStructure.Pin = GPIO_PIN_7;
	GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
	
	/*!< Configure sFLASH_SPI pins: MISO */
	GPIO_InitStructure.Pin		 = GPIO_PIN_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Input;
	GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
	
	/*!< Configure sFLASH_CS_PIN pin: sFLASH Card CS pin */
	GPIO_InitStructure.Pin		 = GPIO_PIN_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
}


void vSPI_MspDeInit(void)
{
	GPIO_InitType GPIO_InitStructure;
	
	GPIO_InitStruct(&GPIO_InitStructure);
	GPIO_InitStructure.Pin			= GPIO_PIN_4 |GPIO_PIN_5 |GPIO_PIN_6| GPIO_PIN_7;
	GPIO_InitStructure.GPIO_Pull	= GPIO_No_Pull;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Analog;
	GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
}


void vBSP_SpiInit(void)
{
    SPI_InitType SPI_InitStructure;

	vSPI_MspInit();

    /*!< SPI configuration */
    SPI_InitStructure.DataDirection = SPI_DIR_DOUBLELINE_FULLDUPLEX;
    SPI_InitStructure.SpiMode       = SPI_MODE_MASTER;
    SPI_InitStructure.DataLen       = SPI_DATA_SIZE_8BITS;
    SPI_InitStructure.CLKPOL        = SPI_CLKPOL_LOW;
    SPI_InitStructure.CLKPHA        = SPI_CLKPHA_FIRST_EDGE;
    SPI_InitStructure.NSS           = SPI_NSS_SOFT;

    SPI_InitStructure.BaudRatePres = SPI_BR_PRESCALER_2;

    SPI_InitStructure.FirstBit = SPI_FB_MSB;
    SPI_InitStructure.CRCPoly  = 7;
    SPI_Init(SPI1, &SPI_InitStructure);

    /*!< Enable the sFLASH_SPI  */
    SPI_Enable(SPI1, ENABLE);

  //  SPI_I2S_EnableInt(SPI1, SPI_I2S_INT_TE, ENABLE);
  //  SPI_I2S_EnableInt(SPI1, SPI_I2S_INT_RNE, ENABLE);

}


void vBSP_SpiDeInit(void)
{
	vSPI_MspDeInit();
	SPI_I2S_DeInit(SPI1);
}

SPI_STATUS ucBSP_SpiTransmitReceive(SPI_Module* SPIx, UCHAR *pTxData, UCHAR *pRxData, USHORT Size, ULONG Timeout)
{
    SPI_STATUS ucRet = SPI_OK;
	USHORT wk_idx = 0;
	ULONG tickstart = 0U;
	UCHAR txallowed = TRUE;
	
	if ((pTxData == NULL) || (pRxData == NULL) ||(Size == 0))
	{
		return SPI_ERROR;
	}

	if(SPI_I2S_GetStatus(SPIx,SPI_I2S_BUSY_FLAG) !=RESET)
	{
		return SPI_BUSY;
	}
	
	/* Init tickstart for timeout management*/
	tickstart = HAL_GetTick();

	while(wk_idx < Size)
	{
		/* check TXE flag */
		if(txallowed && (SPI_I2S_GetStatus(SPIx,SPI_I2S_TE_FLAG) !=RESET))
		{
			SPI_I2S_TransmitData(SPIx,*(pTxData + wk_idx));
			/* Next Data is a reception (Rx). Tx not allowed */
			txallowed = FALSE;
		}

		/*wait rx */
		if(SPI_I2S_GetStatus(SPIx,SPI_I2S_RNE_FLAG) !=RESET)
		{
			*(pRxData + wk_idx) = SPI_I2S_ReceiveData(SPIx);
			wk_idx++;
			/* Next Data is a Transmission (Tx). Tx is allowed */
			txallowed = TRUE;
		}
		
		if ((Timeout != 0xFFFFFFFFU) && ((HAL_GetTick() - tickstart) >=  Timeout))
		{
			ucRet = SPI_TIMEOUT;
			break;
		}
	}

    return ucRet;
}

