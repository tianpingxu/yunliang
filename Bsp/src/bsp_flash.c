/******************************************************************************
#ifdef DOC
File Name		: app_balance.c
Description		: Cell Balance
Remark			: -
Date			: 2019/05/17
Copyright		: Techsum Power Technology Co., Ltd.
#endif
******************************************************************************/
/****************************************************************************
 include files
*****************************************************************************/
#include "sys_type.h"
#include "n32l40x.h"
#include "bsp_io.h"
#include "bsp_flash.h"
#include "bsp_wdg.h"

/****************************************************************************
 define macro
*****************************************************************************/


/****************************************************************************
 define type (STRUCTURE, UNION)
*****************************************************************************/

/****************************************************************************
  Private variables
*****************************************************************************/

/****************************************************************************
 Variable declaration
*****************************************************************************/

/****************************************************************************
 Prototype declaration
*****************************************************************************/
UCHAR FLASH_If_WriteWord(ULONG destination, ULONG* p_source, ULONG length)
{
  ULONG i = 0;

  /* Unlock the Flash to enable the flash control register access *************/
  FLASH_Unlock();
  for (i = 0; i < length; i++)
  {
	vBSP_WDG_Refresh();
    /* Device voltage range supposed to be [2.7V to 3.6V], the operation willbe done by word */ 
    if (FLASH_ProgramWord(destination, *(ULONG*)(p_source+i)) == FLASH_COMPL)      
    {
     /* Check the written value */
      if (*(ULONG*)destination != *(ULONG*)(p_source+i))
      {
        /* Flash content doesn't match SRAM content */
        return(FALSE);
      }
      /* Increment FLASH destination address */
      destination += 4;
    }
    else
    {
      /* Error occurred while writing data in Flash memory */
	  FLASH_Lock();
      return (FALSE);
    }
  }
  /* Lock the Flash to disable the flash control register access (recommended to protect the FLASH memory against 
possible unwanted operation) *********/
  FLASH_Lock();

  return (TRUE);
}


void FLASH_If_ReadWord(ULONG destination, ULONG *p_source, USHORT cnt)
{
	for(USHORT idx = 0;idx<cnt;idx++)
	{
		*(p_source+idx) = *(((ULONG*)destination)+idx);
	}
}

void FLASH_If_ReadByte(ULONG destination, UCHAR *p_source, USHORT cnt)
{
	for(USHORT idx = 0;idx<cnt;idx++)
	{
		*(p_source+idx) = *(((UCHAR*)destination)+idx);
	}
}

/******************************************************************************
#ifdef DOC
FUNCTION		: 
DESCRIPTION		: 
INPUT			: none
OUTPUT			: none
UPDATE			: -
DATE			: 2019/5/17
******************************************************************************/
void vBSP_McuFlashInit(void)
{
	/* Configures the Internal High Speed oscillator */
	if(FLASH_HSICLOCK_ENABLE == FLASH_ClockInit())
	{
		/* Unlock the Program memory */
		FLASH_Unlock();
		FLASH_ClearFlag(FLASH_STS_CLRFLAG);
		/* Unlock the Program memory */
		FLASH_Lock();
	}
}


UCHAR ucEraseSector(ULONG addr)
{
	UCHAR wk_ret = TRUE;
	/* Unlocks the FLASH Program Erase Controller */
	FLASH_Unlock();
	if (FLASH_COMPL == FLASH_EraseOnePage(addr))
	{
		/*check erase result*/
		for(USHORT idx = 0;idx<SECTOR_SIZE;idx+=4)
		{
			if(*((ULONG*)(addr + idx)) != 0xFFFFFFFF)
			{
				wk_ret = FALSE;
			}
		}
	}
	else
	{
		wk_ret = TRUE;
	}
  	FLASH_Lock();
	return wk_ret;
}




