/****************************************************************************
#ifdef DOC

File Name		:app_balance.h

Description		:

Remark			:

Date			:19/05/17

Copyright		:Techsum Power Technology Co., Ltd.

#endif
*****************************************************************************/
#ifndef		BSP_USART_H
#define		BSP_USART_H


#define USART_BLE	(USART1)
#define USART_LCD1	(USART2)
#define USART_LCD2	(USART3)
#define USART_DEBUG	(UART5)

/****************************************************************************
 define macro
*****************************************************************************/

/****************************************************************************
 define type (STRUCTURE, UNION)
*****************************************************************************/
 
#define  UART_RX_OVER_TIME    5 
#define  RECV_POOL_SIZE         100
#define PACKET_POOL_NUM         3

typedef struct
{
	UCHAR *pRecvDataPool;
	//UCHAR *pRecvPacketPool;
	USHORT usRecvIdx;
	UCHAR ucRecvTimeout;
	UCHAR RevComplete;
	//HORT usRecvPacketSize[PACKET_POOL_NUM];
	//HAR ucPacketWrIdx;
	//HAR ucPacketRdIdx;
	//HAR ucPacketCnt;
	//HAR ucUartErrorFlag;
}ST_UART;

/****************************************************************************
 define type (Enumerated type ENUM)
*****************************************************************************/
extern UCHAR u8_gUartRevPool1[RECV_POOL_SIZE];
extern UCHAR u8_gUartRevPool2[RECV_POOL_SIZE];
extern UCHAR u8_gUartRevPool3[RECV_POOL_SIZE];
extern ST_UART st_gUsartBLE;
extern ST_UART st_gUsartLCD1;
extern ST_UART st_gUsartLCD2;

/****************************************************************************
 Variable declaration
*****************************************************************************/
/****************************************************************************
 Prototype declaration
*****************************************************************************/
void vBSP_USARTInit(USART_Module* USARTx);
void vBSP_USARTDeInit(USART_Module* USARTx);

UCHAR ucBSP_UartSendData(UCHAR *pTxData, USHORT Size);
UCHAR ucBSP_UartRecvData(void);
UCHAR ucBSP_UartReceive(USART_Module *huart,UCHAR *pRxData, USHORT *RxSize);
UCHAR ucBSP_UartSendDataPoll(USART_Module* USARTx,UCHAR *pTxData, USHORT Size);

void PRINTF(const char *format, ...);



#endif	/* CB_H */


