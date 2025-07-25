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
#include "n32l40x.h"
#include "sys_type.h"
#include "bsp_usart.h"
#include "bsp_io.h"
#include "bsp_wdg.h"
#include "bsp_timer.h"
#include "protocol.h"


/****************************************************************************
 define macro
*****************************************************************************/
/****************************************************************************
 define type (Enumerated type ENUM)
*****************************************************************************/
/****************************************************************************
 Variable declaration
*****************************************************************************/
/* hal lib used */

/* recv buffer */


UCHAR u8_gUartRevPool1[RECV_POOL_SIZE];
UCHAR u8_gUartRevPool2[RECV_POOL_SIZE];
UCHAR u8_gUartRevPool3[RECV_POOL_SIZE];
UCHAR u8_gUartRevPool4[RECV_POOL_SIZE];
ST_UART st_gUsartBLE = {0};
ST_UART st_gUsartLCD1 = {0};
ST_UART st_gUsartLCD2 = {0};
ST_UART st_gUsartDebug = {0};
//ST_UART st_gUsartLCD = {0};




/****************************************************************************
 Prototype declaration
*****************************************************************************/
void HAL_UART_MspInit(USART_Module* huart);
void HAL_UART_MspDeInit(USART_Module* huart);



/******************************************************************************
#ifdef DOC
FUNCTION		: 
DESCRIPTION		: 
INPUT			: none
OUTPUT			: none
UPDATE			: -
DATE			: 2019/5/17
******************************************************************************/

void vBSP_USARTInit(USART_Module* USARTx)
{   
	USART_InitType USART_InitStructure;
    /*##-1- Configure the UART peripheral ######################################*/
    /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
    /* UART configured as follows:
        - Word Length = 8 Bits
        - Stop Bit = One Stop bit
        - Parity = None
        - BaudRate = 9600 baud
        - Hardware flow control disabled (RTS and CTS signals) */
    /* Enable GPIO clock */

	vBSP_USARTDeInit(USARTx);

	if(USART1 == USARTx)
	{
		HAL_UART_MspInit(USART1);
		USART_StructInit(&USART_InitStructure);
		USART_InitStructure.BaudRate			= 115200;
		USART_InitStructure.WordLength			= USART_WL_8B;
		USART_InitStructure.StopBits			= USART_STPB_1;
		USART_InitStructure.Parity				= USART_PE_NO;
		USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
		USART_InitStructure.Mode				= USART_MODE_RX | USART_MODE_TX;
		USART_Init(USART1, &USART_InitStructure);
		
		USART_ConfigInt(USART1, USART_INT_RXDNE, DISABLE);
		USART_ConfigInt(USART1, USART_INT_TXDE, DISABLE);
		USART_ConfigInt(USART1, USART_INT_ERRF, DISABLE);
		USART_ConfigInt(USART1, USART_INT_IDLEF, DISABLE);
		USART_Enable(USART1, DISABLE);
		
		 USART_ConfigInt(USART1, USART_INT_RXDNE, ENABLE);
		// USART_ConfigInt(USART1, USART_INT_TXDE, ENABLE);
		 USART_ConfigInt(USART1, USART_INT_ERRF, ENABLE);
		 USART_ConfigInt(USART1, USART_INT_IDLEF, ENABLE);
		 USART_Enable(USART1, ENABLE);
	}
	else if(USART2 == USARTx)
	{
		HAL_UART_MspInit(USART2);
		USART_StructInit(&USART_InitStructure);
		USART_InitStructure.BaudRate			= 115200;
		USART_InitStructure.WordLength			= USART_WL_8B;
		USART_InitStructure.StopBits			= USART_STPB_1;
		USART_InitStructure.Parity				= USART_PE_NO;
		USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
		USART_InitStructure.Mode				= USART_MODE_RX | USART_MODE_TX;
		USART_Init(USART2, &USART_InitStructure);
		
		USART_ConfigInt(USART2, USART_INT_RXDNE, DISABLE);
		USART_ConfigInt(USART2, USART_INT_TXDE, DISABLE);
		USART_ConfigInt(USART2, USART_INT_ERRF, DISABLE);
		USART_ConfigInt(USART2, USART_INT_IDLEF, DISABLE);
		USART_Enable(USART2, DISABLE);
		
		 USART_ConfigInt(USART2, USART_INT_RXDNE, ENABLE);
		// USART_ConfigInt(USART2, USART_INT_TXDE, ENABLE);
		 USART_ConfigInt(USART2, USART_INT_ERRF, ENABLE);
		 USART_ConfigInt(USART2, USART_INT_IDLEF, ENABLE);
		 USART_Enable(USART2, ENABLE);
	}
	else if(USART3 == USARTx)
	{
		HAL_UART_MspInit(USART3);
		USART_StructInit(&USART_InitStructure);
		USART_InitStructure.BaudRate			= 38400;
		USART_InitStructure.WordLength			= USART_WL_8B;
		USART_InitStructure.StopBits			= USART_STPB_1;
		USART_InitStructure.Parity				= USART_PE_NO;
		USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
		USART_InitStructure.Mode				= USART_MODE_RX | USART_MODE_TX;
		USART_Init(USART3, &USART_InitStructure);
		
		USART_ConfigInt(USART3, USART_INT_RXDNE, DISABLE);
		USART_ConfigInt(USART3, USART_INT_TXDE, DISABLE);
		USART_ConfigInt(USART3, USART_INT_ERRF, DISABLE);
		USART_ConfigInt(USART3, USART_INT_IDLEF, DISABLE);
		USART_Enable(USART3, DISABLE);
		
		 USART_ConfigInt(USART3, USART_INT_RXDNE, ENABLE);
		// USART_ConfigInt(USART3, USART_INT_TXDE, ENABLE);
		 USART_ConfigInt(USART3, USART_INT_ERRF, ENABLE);
		 USART_ConfigInt(USART3, USART_INT_IDLEF, ENABLE);
		 USART_Enable(USART3, ENABLE);
	}
	else if(UART5 == USARTx)
	{
		HAL_UART_MspInit(UART5);
		USART_StructInit(&USART_InitStructure);
		USART_InitStructure.BaudRate			= 115200;
		USART_InitStructure.WordLength			= USART_WL_8B;
		USART_InitStructure.StopBits			= USART_STPB_1;
		USART_InitStructure.Parity				= USART_PE_NO;
		USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
		USART_InitStructure.Mode				= USART_MODE_RX | USART_MODE_TX;
		USART_Init(UART5, &USART_InitStructure);
		
		USART_ConfigInt(UART5, USART_INT_RXDNE, DISABLE);
		USART_ConfigInt(UART5, USART_INT_TXDE, DISABLE);
		USART_ConfigInt(UART5, USART_INT_ERRF, DISABLE);
		USART_ConfigInt(UART5, USART_INT_IDLEF, DISABLE);
		USART_Enable(UART5, DISABLE);
		
		 USART_ConfigInt(UART5, USART_INT_RXDNE, ENABLE);
		// USART_ConfigInt(USART3, USART_INT_TXDE, ENABLE);
		 USART_ConfigInt(UART5, USART_INT_ERRF, ENABLE);
		 USART_ConfigInt(UART5, USART_INT_IDLEF, ENABLE);
		 USART_Enable(UART5, ENABLE);
	}

	st_gUsartBLE.pRecvDataPool = u8_gUartRevPool1;
	st_gUsartBLE.usRecvIdx = 0;
	st_gUsartBLE.RevComplete = FALSE;

	st_gUsartLCD1.pRecvDataPool = u8_gUartRevPool2;
	st_gUsartLCD1.usRecvIdx = 0;
	st_gUsartLCD1.RevComplete = FALSE;
	
	st_gUsartLCD2.pRecvDataPool = u8_gUartRevPool3;
	st_gUsartLCD2.usRecvIdx = 0;
	st_gUsartLCD2.RevComplete = FALSE;

	st_gUsartDebug.pRecvDataPool = u8_gUartRevPool4;
	st_gUsartDebug.usRecvIdx = 0;
	st_gUsartDebug.RevComplete = FALSE;
}



void vBSP_USARTDeInit(USART_Module* USARTx)
{
	HAL_UART_MspDeInit(USARTx);
    USART_DeInit(USARTx);
}

USHORT u16_gUartTxCnt;
UCHAR *u8_pUartTxData;
UCHAR ucBSP_UartSendDataIT(UCHAR *pTxData, USHORT Size)
{
	u16_gUartTxCnt = Size;
	u8_pUartTxData = pTxData;
	
	USART_SendData(USART1, (USHORT)*pTxData);
    //USART_ConfigInt(USART1, USART_INT_TXDE, ENABLE);
    USART_ConfigInt(USART1, USART_INT_TXC, ENABLE);
    return TRUE;
}

UCHAR ucBSP_UartSendDataPoll(USART_Module* USARTx,UCHAR *pTxData, USHORT Size)
{
	for(USHORT idx = 0;idx<Size;idx++)
	{
      	vBSP_WDG_Refresh();
      	USART_SendData(USARTx, (USHORT)(*(pTxData+idx)));
		while (RESET == USART_GetFlagStatus(USARTx, USART_FLAG_TXDE));  /* Wait Tx data register empty */
	}
	vBSP_WDG_Refresh();
	while (RESET == USART_GetFlagStatus(USARTx, USART_FLAG_TXC));	/* Wait Tx data register empty */
    return TRUE;
}



/******************************************************************************
#ifdef DOC
FUNCTION		: ucBSP_Uart_SendData
DESCRIPTION		: Sends an amount of data in non blocking mode.
INPUT			: 
                 pData: Pointer to data buffer
                 Size: Amount of data to be sent
OUTPUT			: none
UPDATE			: -
DATE			: 2019/5/27
******************************************************************************/ 
UCHAR ucBSP_UartSendData(UCHAR *pTxData, USHORT Size)
{
    UCHAR ucRet;
	//ucRet = ucBSP_UartSendDataPoll(pTxData, Size);
	//ucRet = ucBSP_UartSendDataIT(pTxData, Size);
    	
    return ucRet;
}

/******************************************************************************
#ifdef DOC
FUNCTION		: Uart1_receive
DESCRIPTION		: Receive an amount of data .
INPUT			: 
                 pRxData: Pointer to save data buffer
                 RxSize: Amount of data to be save
OUTPUT			: none
UPDATE			: -
DATE			: 2019/5/27
******************************************************************************/ 
UCHAR ucBSP_UartReceive(USART_Module *huart,UCHAR *pRxData, USHORT *RxSize)
{
    UCHAR ucRet = FALSE;
	USHORT wk_rx_size;
	USHORT i;
	ULONG wk_uart = (ULONG)huart;
 	ST_UART *pUart = &st_gUsartBLE;

	switch(wk_uart)
	{
		case (ULONG)USART_BLE:
			pUart = &st_gUsartBLE;			
 			break;
		case (ULONG)USART_LCD1:
			pUart = &st_gUsartLCD1;			
 			break;
		case (ULONG)USART_LCD2:
			pUart = &st_gUsartLCD2;			
 			break;
		case (ULONG)USART_DEBUG:
			pUart = &st_gUsartDebug;			
 			break;
		default:
			break;
	}
	
	if(pUart->RevComplete)
	{
		pUart->RevComplete = FALSE;
		wk_rx_size = pUart->usRecvIdx;
 		for (i = 0; i < wk_rx_size; i++ )
		{
			pRxData[i] =pUart->pRecvDataPool[i];
		}
		*RxSize = wk_rx_size;
		pUart->usRecvIdx = 0;
		ucRet = TRUE;
	}


	
//	if(USART_LCD == huart)
//	{
//		if ( st_gUsartLCD.ucPacketCnt > 0)
//		{
//			wk_rx_size = st_gUsartLCD.usRecvPacketSize[st_gUsartLCD.ucPacketRdIdx];
//			for ( i = 0; i < wk_rx_size; i++ )
//			{
//				pRxData[i] = st_gUsartLCD.ucRecvPacketPool[st_gUsartLCD.ucPacketRdIdx][i];
//			}
//			*RxSize = wk_rx_size
//			st_gUsartLCD.ucPacketRdIdx++;
//			if ( st_gUsartLCD.ucPacketRdIdx >= PACKET_POOL_NUM )
//			{
//				st_gUsartLCD.ucPacketRdIdx = 0;
//			}
//			__disable_irq();
//			st_gUsartLCD.ucPacketCnt--;
//			__enable_irq();
//			ucRet = TRUE;
//		}
//	}
//	else if(USART_BLE == huart)
//	{

//	}
    return ucRet;
}

/******************************************************************************
#ifdef DOC
FUNCTION		: UsartRx1MsCallback
DESCRIPTION		: this function should be call every millisecond
INPUT			:
OUTPUT			: none
UPDATE			: -
DATE			: 2019/5/27
******************************************************************************/ 
void HAL_UART_RxCpltCallback(USART_Module *huart)
{
	USHORT *pRxLen = NULL;
	UCHAR *pRxBuf = NULL;
	UCHAR *pRxComplete = NULL;
	USHORT wk_buf_len;
	UCHAR wk_rev_dat;

	ULONG wk_uart = (ULONG)huart;
	
	switch(wk_uart)
	{
		case (ULONG)USART_BLE:
			pRxLen = &st_gUsartBLE.usRecvIdx;
			pRxBuf = st_gUsartBLE.pRecvDataPool;
			pRxComplete = &st_gUsartBLE.RevComplete;
			wk_buf_len = sizeof(u8_gUartRevPool1);
			break;
		case (ULONG)USART_LCD1:
			pRxLen = &st_gUsartLCD1.usRecvIdx;
			pRxBuf = st_gUsartLCD1.pRecvDataPool;
			pRxComplete = &st_gUsartLCD1.RevComplete;
			wk_buf_len = sizeof(u8_gUartRevPool2);
			st_gUsartLCD1.ucRecvTimeout = 0;
			break;
		case (ULONG)USART_LCD2:
			pRxLen = &st_gUsartLCD2.usRecvIdx;
			pRxBuf = st_gUsartLCD2.pRecvDataPool;
			pRxComplete = &st_gUsartLCD2.RevComplete;
			wk_buf_len = sizeof(u8_gUartRevPool3);
			st_gUsartLCD2.ucRecvTimeout = 0;
			break;
		case (ULONG)USART_DEBUG:
			pRxLen = &st_gUsartDebug.usRecvIdx;
			pRxBuf = st_gUsartDebug.pRecvDataPool;
			pRxComplete = &st_gUsartDebug.RevComplete;
			wk_buf_len = sizeof(u8_gUartRevPool4);
			break;
		default:
			break;
 	}

	// if((pRxLen != NULL) &&(*pRxLen < wk_buf_len))
	// {
	// 	*pRxComplete = FALSE;
		// wk_rev_dat = USART_ReceiveData(USART2);
	// 	pRxBuf[(*pRxLen)++] = wk_rev_dat;
	// }
		// GPIO_SetBits(GPIOB, GPIO_PIN_9); 
		
		// USART_SendData(USART2, wk_rev_dat);
		// vBSP_DelayMs(1);

		// GPIO_ResetBits(GPIOB, GPIO_PIN_9); 
	
//    if(huart == USART_LCD)
//    {
//        st_gUsartLCD.ucRecvTimeout = 0;
//        /* save recv byte */
//        if(st_gUsartLCD.usRecvIdx < RECV_POOL_SIZE)
//        {
//       		wk_rev_dat = USART_ReceiveData(huart) & 0xFF;
//            st_gUsartLCD.ucRecvDataPool[st_gUsartLCD.usRecvIdx++] = wk_rev_dat;
//        }
//        else
//        {
//            USHORT i, usRecvNum;
//            usRecvNum = st_gUsartLCD.usRecvIdx;
//            st_gUsartLCD.usRecvIdx = 0;
//            if ( st_gUsartLCD.ucPacketCnt < PACKET_POOL_NUM )
//            {
//                st_gUsartLCD.usRecvPacketSize[st_gUsartLCD.ucPacketWrIdx] = usRecvNum;              
//                for( i = 0; i < usRecvNum; i++)
//                {
//                    st_gUsartLCD.ucRecvPacketPool[st_gUsartLCD.ucPacketWrIdx][i] = st_gUsartLCD.ucRecvDataPool[i];
//                }
//                st_gUsartLCD.ucPacketWrIdx++;
//                if ( st_gUsartLCD.ucPacketWrIdx >= PACKET_POOL_NUM )
//                {
//                    st_gUsartLCD.ucPacketWrIdx = 0;
//                }
//                st_gUsartLCD.ucPacketCnt++;
//            }
//        }
//    }
}

void HAL_UART_IdleCallback(USART_Module *huart)
{
	ULONG wk_uart = (ULONG)huart;

	switch(wk_uart)
	{
		case (ULONG)USART_BLE:
			st_gUsartBLE.RevComplete = TRUE;
			break;
		case (ULONG)USART_LCD1:
			//st_gUsartLCD1.RevComplete = TRUE;
			break;
		case (ULONG)USART_LCD2:
			//st_gUsartLCD2.RevComplete = TRUE;
			break;
		case (ULONG)USART_DEBUG:
			st_gUsartDebug.RevComplete = TRUE;
			break;
		default:
			break;
	}

//USHORT i, usRecvNum;

//	if(USART_LCD == huart)
//	{
//		NVIC_DisableIRQ(USART1_IRQn);
//		usRecvNum = st_gUsartLCD.usRecvIdx;
//		st_gUsartLCD.usRecvIdx = 0;
//		NVIC_EnableIRQ(USART1_IRQn);
//		if ( st_gUsartLCD.ucPacketCnt < PACKET_POOL_NUM )
//		{
//			st_gUsartLCD.usRecvPacketSize[st_gUsartLCD.ucPacketWrIdx] = usRecvNum;
//			for( i = 0; i < usRecvNum; i++)
//			{
//				st_gUsartLCD.ucRecvPacketPool[st_gUsartLCD.ucPacketWrIdx][i] = st_gUsartLCD.ucRecvDataPool[i];
//			}
//			st_gUsartLCD.ucPacketWrIdx++;
//			if ( st_gUsartLCD.ucPacketWrIdx >= PACKET_POOL_NUM )
//			{
//				st_gUsartLCD.ucPacketWrIdx = 0;
//			}
//			st_gUsartLCD.ucPacketCnt++;
//		}
//	}
}

void vUartRevOtCallback(void)
{
	if((st_gUsartLCD1.usRecvIdx > 0) && (st_gUsartLCD1.RevComplete == FALSE))
	{
		if(++st_gUsartLCD1.ucRecvTimeout >= 35)
		{
			st_gUsartLCD1.ucRecvTimeout = 0;
			st_gUsartLCD1.RevComplete = TRUE;
		}
	}
	if((st_gUsartLCD2.usRecvIdx > 0) && (st_gUsartLCD2.RevComplete == FALSE))
	{
		if(++st_gUsartLCD2.ucRecvTimeout >= 35)
		{
			st_gUsartLCD2.ucRecvTimeout = 0;
			st_gUsartLCD2.RevComplete = TRUE;
		}
	}
}


void UART_TxCallback(USART_Module* USARTx)
{    
    if((u16_gUartTxCnt == 0) || (u8_pUartTxData == NULL))
    {
		return;
    }
    
    if(USARTx == USART1)
    {
        u8_pUartTxData++;
        u16_gUartTxCnt--;
		if(u16_gUartTxCnt == 0)
		{
			//st_gUsartLCD.usRecvIdx = 0;
            return;
		}
		USART_SendData(USARTx, (USHORT)*u8_pUartTxData);
		//USART_ConfigInt(USART1, USART_INT_TXDE, ENABLE);
		USART_ConfigInt(USART1, USART_INT_TXC, ENABLE);
    }
}


/******************************************************************************
#ifdef DOC
FUNCTION		: UsartRx1MsCallback
DESCRIPTION		: this function should be call every millisecond
INPUT			:
OUTPUT			: none
UPDATE			: -
DATE			: 2019/6/4
******************************************************************************/ 
void HAL_UART_TxCpltCallback(USART_Module *huart)
{
#if 0
    if(huart == &hUartRs485)
    {
        /* send complete , convert to receive mode */   
//        HAL_HalfDuplex_EnableReceiver(huart);
        RS_485_RX_EN();
        st_gUsartLCD.usRecvIdx = 0;
        HAL_UART_Receive_IT(&hUartRs485, &ucRecvDat, 1);
    }
#endif
}
/******************************************************************************
#ifdef DOC
FUNCTION		: UsartRx1MsCallback
DESCRIPTION		: this function should be call every millisecond
INPUT			:
OUTPUT			: none
UPDATE			: -
DATE			: 2019/6/4
******************************************************************************/ 
void HAL_UART_ErrorCallback(USART_Module *huart)
{
    if(huart == USART1)
    {
        /* error */
//        HAL_HalfDuplex_EnableReceiver(huart);
//        RS_485_RX_EN();
//        st_gUsartLCD.usRecvIdx = 0;
//        HAL_UART_Receive_IT(&hUartRs485, &ucRecvDat, 1);
		//st_gUsartLCD1.ucUartErrorFlag = TRUE;
    }

}
/******************************************************************************
#ifdef DOC
FUNCTION		: HAL_UART_MspInit
DESCRIPTION		: 
INPUT			:
OUTPUT			: none
UPDATE			: -
DATE			: 2019/6/4
******************************************************************************/ 
void HAL_UART_MspInit(USART_Module* huart)
{    
    GPIO_InitType GPIO_InitStructure;
    NVIC_InitType NVIC_InitStructure;
    
	GPIO_InitStruct(&GPIO_InitStructure);
    if(huart == USART1)
    {
		RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOA | RCC_APB2_PERIPH_AFIO, ENABLE);
		RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_USART1, ENABLE);

		
        /* UART TX GPIO pin configuration  */
		GPIO_InitStructure.Pin			  = GPIO_PIN_9;	 
		GPIO_InitStructure.GPIO_Mode	  = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Pull      = GPIO_Pull_Up;
		GPIO_InitStructure.GPIO_Alternate = GPIO_AF4_USART1;
		GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);

        /* UART RX GPIO pin configuration  */
//		GPIO_InitStructure.Pin			  = GPIO_PIN_10;	 
//		GPIO_InitStructure.GPIO_Mode	  = GPIO_Mode_AF_PP;
//      GPIO_InitStructure.GPIO_Pull      = GPIO_Pull_Up;
//		GPIO_InitStructure.GPIO_Alternate = GPIO_AF4_USART1;
//		GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);

		/* Enable the USARTy Interrupt */
		// NVIC_InitStructure.NVIC_IRQChannel			  = USART1_IRQn;
		// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
		// NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		// NVIC_InitStructure.NVIC_IRQChannelCmd		  = ENABLE;
		// NVIC_Init(&NVIC_InitStructure);
    }
    else if(huart == USART2)
    {
		RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOD | RCC_APB2_PERIPH_AFIO, ENABLE);
		RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_USART2, ENABLE);

		
        /* UART TX GPIO pin configuration  */
		GPIO_InitStructure.Pin			  = GPIO_PIN_14;	 
		GPIO_InitStructure.GPIO_Mode	  = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Pull      = GPIO_Pull_Up;
		GPIO_InitStructure.GPIO_Alternate = GPIO_AF4_USART2;
		GPIO_InitPeripheral(GPIOD, &GPIO_InitStructure);

        /* UART RX GPIO pin configuration  */
		GPIO_InitStructure.Pin			  = GPIO_PIN_15;	 
		GPIO_InitStructure.GPIO_Mode	  = GPIO_Mode_AF_OD;
        GPIO_InitStructure.GPIO_Pull      = GPIO_Pull_Up;
		GPIO_InitStructure.GPIO_Alternate = GPIO_AF4_USART2;
		GPIO_InitPeripheral(GPIOD, &GPIO_InitStructure);

		GPIO_InitStructure.Pin = GPIO_PIN_9;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Slew_Rate = GPIO_Slew_Rate_High;
		GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);

		/* Enable the USARTy Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel			  = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd		  = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		GPIO_ResetBits(GPIOB,GPIO_PIN_9); 
    }
    else if(huart == USART3)
    {
		RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB | RCC_APB2_PERIPH_AFIO, ENABLE);
		RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_USART3, ENABLE);

		
        /* UART TX GPIO pin configuration  */
		GPIO_InitStructure.Pin			  = GPIO_PIN_10;	 
		GPIO_InitStructure.GPIO_Mode	  = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Pull      = GPIO_Pull_Up;
		GPIO_InitStructure.GPIO_Alternate = GPIO_AF0_USART3;
		GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);

        /* UART RX GPIO pin configuration  */
		GPIO_InitStructure.Pin			  = GPIO_PIN_11;	 
		GPIO_InitStructure.GPIO_Mode	  = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Pull      = GPIO_Pull_Up;
		GPIO_InitStructure.GPIO_Alternate = GPIO_AF5_USART3;
		GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);

		/* Enable the USARTy Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel			  = USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd		  = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
    }
    else if(huart == UART5)
    {
		RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB | RCC_APB2_PERIPH_AFIO, ENABLE);
		RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_UART5, ENABLE);

		
        /* UART TX GPIO pin configuration  */
		GPIO_InitStructure.Pin			  = GPIO_PIN_8;	 
		GPIO_InitStructure.GPIO_Mode	  = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Pull      = GPIO_Pull_Up;
		GPIO_InitStructure.GPIO_Alternate = GPIO_AF6_UART5;
		GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);

        /* UART RX GPIO pin configuration  */
//		GPIO_InitStructure.Pin			  = GPIO_PIN_11;	 
//		GPIO_InitStructure.GPIO_Mode	  = GPIO_Mode_AF_PP;
//        GPIO_InitStructure.GPIO_Pull      = GPIO_Pull_Up;
//		GPIO_InitStructure.GPIO_Alternate = GPIO_AF5_USART3;
//		GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);

		/* Enable the USARTy Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel			  = UART5_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd		  = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
    }
}
/******************************************************************************
#ifdef DOC
FUNCTION		: HAL_UART_MspDeInit
DESCRIPTION		: 
INPUT			:
OUTPUT			: none
UPDATE			: -
DATE			: 2019/6/4
******************************************************************************/ 
void HAL_UART_MspDeInit(USART_Module* huart)
{
      GPIO_InitType GPIO_InitStructure;    
    if(huart == USART1)
    {
        /*##-1- Reset peripherals ##################################################*/
		vBSP_GpioDeinit(GPIOA,GPIO_PIN_9);
		vBSP_GpioDeinit(GPIOA,GPIO_PIN_10);

        /*##-3- Disable the NVIC for UART ##########################################*/
        NVIC_DisableIRQ(USART1_IRQn);
        GPIO_InitStruct(&GPIO_InitStructure);
        GPIO_InitStructure.Pin		  	= GPIO_PIN_9 |GPIO_PIN_10;
        GPIO_InitStructure.GPIO_Pull	= GPIO_No_Pull;
        GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Analog;
        GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
    }
    else if(huart == USART2)
    {
        /*##-1- Reset peripherals ##################################################*/
		vBSP_GpioDeinit(GPIOA,GPIO_PIN_2);
		vBSP_GpioDeinit(GPIOA,GPIO_PIN_3);

        /*##-3- Disable the NVIC for UART ##########################################*/
        NVIC_DisableIRQ(USART2_IRQn);
        GPIO_InitStruct(&GPIO_InitStructure);
        GPIO_InitStructure.Pin		  	= GPIO_PIN_2 |GPIO_PIN_3;
        GPIO_InitStructure.GPIO_Pull	= GPIO_No_Pull;
        GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Analog;
        GPIO_InitPeripheral(GPIOA, &GPIO_InitStructure);
    }
    else if(huart == USART3)
    {
        /*##-1- Reset peripherals ##################################################*/
		vBSP_GpioDeinit(GPIOB,GPIO_PIN_10);
		vBSP_GpioDeinit(GPIOB,GPIO_PIN_11);

        /*##-3- Disable the NVIC for UART ##########################################*/
        NVIC_DisableIRQ(USART3_IRQn);
        GPIO_InitStruct(&GPIO_InitStructure);
        GPIO_InitStructure.Pin		  	= GPIO_PIN_10 |GPIO_PIN_11;
        GPIO_InitStructure.GPIO_Pull	= GPIO_No_Pull;
        GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Analog;
        GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
    }
    else if(huart == UART5)
    {
        /*##-1- Reset peripherals ##################################################*/
		vBSP_GpioDeinit(GPIOB,GPIO_PIN_8);
		//(GPIOB,GPIO_PIN_11);

        /*##-3- Disable the NVIC for UART ##########################################*/
        NVIC_DisableIRQ(UART5_IRQn);
        GPIO_InitStruct(&GPIO_InitStructure);
        GPIO_InitStructure.Pin		  	= GPIO_PIN_8;
        GPIO_InitStructure.GPIO_Pull	= GPIO_No_Pull;
        GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Analog;
        GPIO_InitPeripheral(GPIOB, &GPIO_InitStructure);
    }
} 

#if 1
//#pragma import(__use_no_semihosting)
//struct __FILE       //��׼����Ҫ��֧�ֺ���
//{
//    int handle;
//};
//FILE __stdout;
void _sys_exit(int x) //����_sys_exit()�Ա���ʹ�ð�����ģʽ
{
    x = x;
}
int fputc(int ch, FILE *f)      //�ض���fputc����
{
	UCHAR wk_data = ch;
	ucBSP_UartSendDataPoll(USART1,&wk_data,1);
    return ch;
}

void printbuffer(u8* buffer,u16 len)
{
    u16 i;
    for(i=0; i<len; i++)
    {
        printf("%02X",buffer[i]);
    }
}

//void debugprint(u8 *data)
void PRINTF(const char *format, ...)
{
	printf(format);
}
#endif



