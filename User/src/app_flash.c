#include "sys_type.h"
#include "w25q64.h"
#include "bsp_wdg.h"
#include "bsp_flash.h"
#include "app_flash.h"


/*该函数只能对第一个页0~255 进行写入*/
void vw25q64WriteData(ULONG addr,UCHAR* indata,UCHAR len)
{
	static UCHAR wk_page_buff[100];
	
	w25q64_ReadData(0, wk_page_buff,sizeof(wk_page_buff));
	memcpy(&wk_page_buff[addr],indata,len);
	w25q64_SectorErase(0);
	w25q64_WritePage(0, wk_page_buff,sizeof(wk_page_buff));	
}

UCHAR ucFlashWriteData(ULONG addr,UCHAR* indata,UCHAR len)
{
	UCHAR wk_retry;
	static UCHAR wk_page_buff[100];

	memset(wk_page_buff,0,100);
	FLASH_If_ReadByte(ADDR_START, wk_page_buff,sizeof(wk_page_buff));
	memcpy(&wk_page_buff[addr - ADDR_START],indata,len);

	for(wk_retry = 0;wk_retry < 5;wk_retry++)
	{
        vBSP_WDG_Refresh();
		if(ucEraseSector(ADDR_START) == TRUE)
		{
            vBSP_WDG_Refresh();
			if(FLASH_If_WriteWord(ADDR_START, (ULONG*)wk_page_buff,sizeof(wk_page_buff) / 4) == TRUE)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}
