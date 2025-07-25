#include "oled_spi.h"

SPI_InitType SPI_InitStructure;
DMA_InitType DMA_InitStructure;
GPIO_InitType GPIO_InitStructure;

extern uint8_t OLED_SRAM[8][128]; // 图像储存在SRAM里

void OLED_SPI_Init(void){
    /* System clocks configuration ---------------------------------------------*/
    RCC_Configuration();

    /* GPIO configuration ------------------------------------------------------*/
    GPIO_Configuration();
	delay_ms(5);

    /* SPI_MASTER configuration ------------------------------------------------------*/
    SPI_InitStructure.DataDirection = SPI_DIR_SINGLELINE_TX;
    SPI_InitStructure.SpiMode       = SPI_MODE_MASTER;
    SPI_InitStructure.DataLen       = SPI_DATA_SIZE_8BITS;
    SPI_InitStructure.CLKPOL        = SPI_CLKPOL_LOW;
    SPI_InitStructure.CLKPHA        = SPI_CLKPHA_FIRST_EDGE;
    SPI_InitStructure.NSS           = SPI_NSS_SOFT;
    SPI_InitStructure.BaudRatePres  = SPI_BR_PRESCALER_8;
    SPI_InitStructure.FirstBit      = SPI_FB_MSB;
    SPI_InitStructure.CRCPoly       = 7;
    SPI_Init(SPI_MASTER, &SPI_InitStructure);

    /* Enable SPI_MASTER NSS output for master mode */
    SPI_SSOutputEnable(SPI_MASTER, ENABLE);

    /* Enable SPI_MASTER */
    SPI_Enable(SPI_MASTER, ENABLE);
}		

void OLED_DMA_Init(void){
    /* SPI_MASTER_Tx_DMA_Channel configuration ---------------------------------------------*/
    DMA_DeInit(SPI_MASTER_Tx_DMA_Channel);
    DMA_InitStructure.PeriphAddr = (uint32_t)SPI_MASTER_DR_Base;
    DMA_InitStructure.MemAddr    = (uint32_t)OLED_SRAM;
    DMA_InitStructure.Direction  = DMA_DIR_PERIPH_DST;
    DMA_InitStructure.BufSize        = 1024;
    DMA_InitStructure.PeriphInc      = DMA_PERIPH_INC_DISABLE;
    DMA_InitStructure.DMA_MemoryInc  = DMA_MEM_INC_ENABLE;
    DMA_InitStructure.PeriphDataSize = DMA_PERIPH_DATA_SIZE_BYTE;
    DMA_InitStructure.MemDataSize    = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.CircularMode   = DMA_MODE_CIRCULAR;
    DMA_InitStructure.Priority       = DMA_PRIORITY_MEDIUM;
    DMA_InitStructure.Mem2Mem        = DMA_M2M_DISABLE;
    DMA_Init(SPI_MASTER_Tx_DMA_Channel, &DMA_InitStructure);

    /* Enable SPI_MASTER Tx request */
    SPI_I2S_EnableDma(SPI_MASTER, SPI_I2S_DMA_TX, ENABLE);

    /* Enable DMA Channel4 */
    DMA_RequestRemap(DMA_REMAP_SPI1_TX,DMA, SPI_MASTER_Tx_DMA_Channel, ENABLE);
    DMA_EnableChannel(SPI_MASTER_Tx_DMA_Channel, ENABLE);
}

void RCC_Configuration(void)
{
    /* PCLK2 = HCLK/2 */
    RCC_ConfigPclk2(RCC_HCLK_DIV1);

    /* Enable peripheral clocks --------------------------------------------------*/
    /* Enable DMA clock */
    RCC_EnableAHBPeriphClk(RCC_AHB_PERIPH_DMA, ENABLE);

    /* Enable GPIO clock for SPI_MASTER */
    RCC_EnableAPB2PeriphClk(SPI_MASTER_GPIO_CLK | RCC_APB2_PERIPH_AFIO, ENABLE);

    /* Enable SPI_MASTER Periph clock */
    RCC_EnableAPB2PeriphClk(SPI_MASTER_CLK, ENABLE);
}

/**
 * @brief  Configures the different GPIO ports.
 */
void GPIO_Configuration(void)
{
    GPIO_InitStruct(&GPIO_InitStructure);
    /* Configure SPI_MASTER pins: NSS, SCK and MOSI */
    GPIO_InitStructure.Pin        = OLED_SCL | OLED_SDA;
	GPIO_InitStructure.GPIO_Pull = GPIO_Pull_Up;
    GPIO_InitStructure.GPIO_Slew_Rate = GPIO_Slew_Rate_High;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF0_SPI1;
    GPIO_InitPeripheral(SPI_MASTER_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.Pin        = OLED_NSS | OLED_RST | OLED_DC;
	GPIO_InitStructure.GPIO_Pull = GPIO_Pull_Up;
    GPIO_InitStructure.GPIO_Slew_Rate = GPIO_Slew_Rate_High;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitPeripheral(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.Pin        = OLED_EN;
	GPIO_InitStructure.GPIO_Pull = GPIO_Pull_Up;
    GPIO_InitStructure.GPIO_Slew_Rate = GPIO_Slew_Rate_High;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);

    GPIO_SetBits(GPIOA, OLED_EN);

    GPIO_SetBits(GPIOA, OLED_SCL | OLED_SDA); // PA5 and PA7上拉
    
    GPIO_ResetBits(GPIOC, SPI_MASTER_PIN_NSS);
}
