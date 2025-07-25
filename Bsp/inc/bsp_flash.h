/****************************************************************************
#ifdef DOC

File Name		:bsp_flash.h

Description		:

Remark			:

Date			:19/05/17

Copyright		:Techsum Power Technology Co., Ltd.

#endif
*****************************************************************************/
#ifndef		BSP_FLASH_H
#define		BSP_FLASH_H



/****************************************************************************
 define macro
*****************************************************************************/
#define SECTOR_SIZE	 ((ULONG)0x800)                         

/****************************************************************************
 define type (STRUCTURE, UNION)
*****************************************************************************/

/****************************************************************************
 define type (Enumerated type ENUM)
*****************************************************************************/

/****************************************************************************
 Variable declaration
*****************************************************************************/


/****************************************************************************
 Prototype declaration
*****************************************************************************/
void vBSP_McuFlashInit(void);
UCHAR FLASH_If_WriteWord(ULONG destination, ULONG *p_source, ULONG length);
void FLASH_If_ReadWord(ULONG destination, ULONG *p_source, USHORT cnt);
void FLASH_If_ReadByte(ULONG destination, UCHAR *p_source, USHORT cnt);
UCHAR vRewriteDownLoadFlag(UCHAR flag);
UCHAR ucEraseSector(ULONG addr);

#endif	/* FLASH_H */

