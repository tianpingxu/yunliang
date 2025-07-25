#ifndef		BSP_SPI_H
#define		BSP_SPI_H



typedef enum 
{
    SPI_OK       = 0,
    SPI_ERROR    = 1,
    SPI_BUSY     = 2,
    SPI_TIMEOUT  = 3
} SPI_STATUS;

#define W25Q64_SPI	(SPI1)


void vBSP_SpiInit(void);
void vBSP_SpiDeInit(void);
SPI_STATUS ucBSP_SpiTransmitReceive(SPI_Module* SPIx, UCHAR *pTxData, UCHAR *pRxData, USHORT Size, ULONG Timeout);


#endif
