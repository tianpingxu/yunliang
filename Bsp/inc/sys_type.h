/****************************************************************************
#ifdef DOC

File Name		:app_balance.h

Description		:

Remark			:

Date			:19/05/17

Copyright		:Techsum Power Technology Co., Ltd.

#endif
*****************************************************************************/
#ifndef		SYS_H
#define		SYS_H

#include "n32l40x.h"
#include <string.h>
#include <stdio.h>
/****************************************************************************
	define basic type
*****************************************************************************/

typedef unsigned char		UCHAR;
typedef unsigned short		USHORT;
typedef unsigned long		ULONG;
typedef char				CHAR;
typedef short				SHORT;
typedef long				LONG;

typedef unsigned char		BOOL;
typedef unsigned char		BYTE;
typedef unsigned short		WORD;
typedef unsigned long		DWORD;
typedef unsigned long long		ULLONG;
/****************************************************************************
	define Boolean
*****************************************************************************/
#ifndef NULL
#define NULL ((void *)0)
#endif
/****************************************************************************
 define macro
*****************************************************************************/
#ifndef TRUE
  /** Value is true (boolean_t type) */
  #define TRUE        (1u)
#endif

#ifndef FALSE
  /** Value is false (boolean_t type) */
  #define FALSE       (0u)
#endif  

#define	OFF		0			/* OFF/ON		*/
#define	ON		(1)

#define	NG		(0)			/* NG/OK		*/
#define	OK		(1)

#define LOW     0
#define	HIGH    (1)

#define  CLR    0

#ifndef SET
#define	SET     (1)
#endif

#define LITTLE_TO_BIG_32(x)	(((((ULONG)(x))>>24)&0x000000FF) |((((ULONG)(x))>>8)&0x0000FF00) |((((ULONG)(x))<<8)&0x00FF0000) |((((ULONG)(x))<<24)&0xFF000000))
#define LITTLE_TO_BIG_16(x)	((((USHORT)(x)>>8)&0x00FF) |(((USHORT)(x)<<8)&0xFF00))
#define	ELEMOF(x)		( ( sizeof( x ) ) / ( sizeof ( *x )))


#define MASK_BIT0		(1<<0)
#define MASK_BIT1		(1<<1)
#define MASK_BIT2		(1<<2)
#define MASK_BIT3		(1<<3)
#define MASK_BIT4		(1<<4)
#define MASK_BIT5		(1<<5)
#define MASK_BIT6		(1<<6)
#define MASK_BIT7		(1<<7)


#define FCC_CALC_ALLWAYS
#define OCV_CALI_AT_TAIL
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


#endif	/* CB_H */

