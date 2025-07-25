#include "sys_type.h"
#include "n32l40x.h"
#include "bsp_spi.h"
#include "bsp_wdg.h"
#include "w25q64.h"



/* USER CODE BEGIN Includes */

#define FEED_WDG()		vBSP_WDG_Refresh();

#if 0
#define w25q64_sendbyte(x)	ucSPI_WriteRead(x)
#else
static UCHAR w25q64_sendbyte(UCHAR data)
{
	UCHAR wk_readdata = 0;
	UCHAR wk_writedata = data;
	
	while(ucBSP_SpiTransmitReceive(W25Q64_SPI,&wk_writedata,&wk_readdata,1,1000)!=SPI_OK);
	return wk_readdata;
}
#endif


void W25Q64_init(void)
{
    w25q64_WriteEnable();
}


void w25q64_WriteEnable(void)
{
    W25Q64_SELECT(LOW);
    w25q64_sendbyte(WRITE_ENABLE);
    W25Q64_SELECT(HIGH);
}


//uint8_t w25q64_sendbyte(uint8_t data)
//{
//    uint8_t wk_readdata = 0;
//    uint8_t wk_writedata = data;
//
//    while(HAL_SPI_TransmitReceive(&hSpi2Handle, &wk_writedata, &wk_readdata, 1, 1000) != HAL_OK);
//    return wk_readdata;
//}




//void w25q64_ChipErase(void)
//{
//    w25q64_WriteEnable();
//    w25q64_WaitForWriteEnd();

//    W25Q64_SELECT(LOW);
//    w25q64_sendbyte(0xC7);
//    W25Q64_SELECT(HIGH);
//    w25q64_WaitForWriteEnd();
//}


void w25q64_SectorErase(uint32_t SectorAddr)
{

    w25q64_WriteEnable();
    w25q64_WaitForWriteEnd();

    W25Q64_SELECT(LOW);

    w25q64_sendbyte(SECTOR_ERASE);

    w25q64_sendbyte((SectorAddr & 0xFF0000) >> 16);

    w25q64_sendbyte((SectorAddr & 0xFF00) >> 8);

    w25q64_sendbyte(SectorAddr & 0xFF);

    W25Q64_SELECT(HIGH);
    w25q64_WaitForWriteEnd();
}



void w25q64_ReadData(uint32_t address, uint8_t *out_data, uint16_t size)
{
    W25Q64_SELECT(LOW);

    w25q64_sendbyte(READ_DATA);
    w25q64_sendbyte((address >> 16) & 0xff);
    w25q64_sendbyte((address >> 8) & 0xff);
    w25q64_sendbyte(address & 0xff);

    while(size--)
    {
        *out_data = w25q64_sendbyte(0xff);
        out_data++;
    }
    W25Q64_SELECT(HIGH);
}



//uint8_t w25q64_WriteQuadPage(uint32_t address, uint8_t *in_data)
//{
//    uint32_t wk_address = address;
//    uint8_t *wk_data = in_data;
//    uint8_t state = TRUE;

//    for(wk_address = address; wk_address < (address + 1024); wk_address += 256)
//    {
//        if(TRUE == w25q64_WritePage(wk_address, wk_data, 256))
//        {
//            wk_data += 256;
//        }
//        else
//        {
//            state = FALSE;
//            break;
//        }
//    }
//    return state;
//}


uint8_t w25q64_WritePage(uint32_t address, uint8_t *in_data, uint16_t size)
{
	uint16_t i;
    uint8_t state = TRUE;
    static uint8_t wk_data[256];
    uint8_t *temp = in_data;
    uint16_t wk_length = size;
    w25q64_WriteEnable();
    //w25q64_WaitForWriteEnd();

    W25Q64_SELECT(LOW);

    w25q64_sendbyte(PAGE_WRITE);
    w25q64_sendbyte((address >> 16) & 0xff);
    w25q64_sendbyte((address >> 8) & 0xff);
    w25q64_sendbyte(address & 0xff);

    while(size--)
    {
        w25q64_sendbyte(*in_data);
        in_data++;
    }
    W25Q64_SELECT(HIGH);
    w25q64_WaitForWriteEnd();

    w25q64_ReadData(address, wk_data, wk_length);
    for( i = 0; i < wk_length; i++)
    {
        if(wk_data[i] != (*(temp + i)))
        {
            state = FALSE;
            break;
        }
    }
    return state;
}


void w25q64_WaitForWriteEnd(void)
{
    uint8_t FLASH_Status = 0;

    /* Select the FLASH: Chip Select low */

    W25Q64_SELECT(LOW);

    /* Send "Read Status Register" instruction */
    w25q64_sendbyte(0x05);

    /* Loop as long as the memory is busy with a write cycle */
    do
    {
        /* Send a dummy byte to generate the clock needed by the FLASH
        and put the value of the status register in FLASH_Status variable */
		FEED_WDG();	/*feed wdt*/
        FLASH_Status = w25q64_sendbyte(0xff);
    }
    while ((FLASH_Status & 0x01) == SET); /* Write in progress */

    /* Deselect the FLASH: Chip Select high */
    W25Q64_SELECT(HIGH);
}


//HAL_StatusTypeDef w5q64_WriteAgain(uint32_t SectorAddr, uint8_t *writedata)
//{
//    HAL_StatusTypeDef state = HAL_OK;
//
//    w25q64_SectorErase(SectorAddr);
//
//
//    if(HAL_OK == w25q64_WriteQuadPage(SectorAddr, writedata))
//    {
//
//    }
//    else
//    {
//        state = HAL_ERROR;
//    }
//
//    return state;
//}
//
//
//HAL_StatusTypeDef w25q64_CheckWriteEnd(void)
//{
//    HAL_StatusTypeDef FLASH_Status = HAL_OK;
//    UCHAR temp = 0;
//    /* Select the FLASH: Chip Select low */
//
//    W25Q64_SELECT(LOW);
//
//    /* Send "Read Status Register" instruction */
//    w25q64_sendbyte(0x05);
//
//    temp = w25q64_sendbyte(0xff);
//
//    if ((temp & 0x01) == SET) /* Write in progress */
//    {
//        FLASH_Status = HAL_ERROR;	//not end
//    }
//    else
//    {
//        //do nothing
//    }
//
//    /* Deselect the FLASH: Chip Select high */
//    W25Q64_SELECT(HIGH);
//
//    return FLASH_Status;
//}



