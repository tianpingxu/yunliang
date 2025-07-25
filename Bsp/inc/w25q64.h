#ifndef W25Q64_H
#define W25Q64_H

#define FLASH_CHIP_SIZE	(((uint32_t)4 << 10)<<10)	/*4M*/

#define W25Q64_SELECT(x)	GPIO_WriteBit(GPIOA, GPIO_PIN_4,(x == LOW) ? Bit_RESET : Bit_SET)

#define SECTOR_ERASE	0X20
#define PAGE_WRITE		0X02
#define SECTOR_WTIRE	0X32
#define READ_DATA		0X03
#define WRITE_ENABLE	0X06
#define WRITE_DISABLE	0X04


extern void W25Q64_init(void);
//extern uint8_t w25q64_sendbyte(uint8_t data);
extern void w25q64_WriteEnable(void);
extern void w25q64_ChipErase(void);
extern void w25q64_SectorErase(uint32_t SectorAddr);
extern void w25q64_ReadData(uint32_t address, uint8_t *out_data, uint16_t size);
extern void w25q64_WaitForWriteEnd(void);
extern uint8_t w25q64_WriteQuadPage(uint32_t address, uint8_t *in_data);
extern uint8_t w25q64_WritePage(uint32_t address, uint8_t *in_data, uint16_t size);
//extern HAL_StatusTypeDef w5q64_WriteAgain(uint32_t SectorAddr, uint8_t    *writedata);
//extern HAL_StatusTypeDef w25q64_CheckWriteEnd(void);
//extern void w25q64_ReadData_IT(ULONG address, UCHAR *data, USHORT size);
//extern HAL_StatusTypeDef w25q64_WritePage_IT(ULONG address, UCHAR *data, USHORT size);

#endif


