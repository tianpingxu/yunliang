#include "485.h"

void rs485_init(void){
    GPIO_InitType GPIO_InitStructure;
    NVIC_InitType NVIC_InitStructure;
    USART_InitType USART_InitStructure;

    RCC_EnableAPB2PeriphClk(RCC_APB2_PERIPH_GPIOB | RCC_APB2_PERIPH_GPIOD | RCC_APB2_PERIPH_AFIO, ENABLE);
    RCC_EnableAPB1PeriphClk(RS485_USART_CLK, ENABLE);
    
    /* UART TX GPIO pin configuration  */
    GPIO_InitStructure.Pin			  = RS485_USART_TX_PIN;
    GPIO_InitStructure.GPIO_Mode	  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pull      = GPIO_Pull_Up;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF4_USART2;
    GPIO_InitPeripheral(RS485_USART_RX_GPIO_PORT, &GPIO_InitStructure);

    /* UART RX GPIO pin configuration  */
    GPIO_InitStructure.Pin			  = RS485_USART_RX_PIN;
    GPIO_InitStructure.GPIO_Mode	  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pull      = GPIO_Pull_Up;
    GPIO_InitStructure.GPIO_Alternate = GPIO_AF4_USART2;
    GPIO_InitPeripheral(RS485_USART_TX_GPIO_PORT, &GPIO_InitStructure);

      /* 485收发控制管脚 */
	GPIO_InitStructure.Pin = RS485_RE_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Slew_Rate = GPIO_Slew_Rate_High;
    GPIO_InitPeripheral(RS485_RE_GPIO_PORT, &GPIO_InitStructure);

    USART_InitStructure.BaudRate			= 115200;
    USART_InitStructure.WordLength			= USART_WL_8B;
    USART_InitStructure.StopBits			= USART_STPB_1;
    USART_InitStructure.Parity				= USART_PE_NO;
    USART_InitStructure.HardwareFlowControl = USART_HFCTRL_NONE;
    USART_InitStructure.Mode				= USART_MODE_RX | USART_MODE_TX;
    USART_Init(RS485_USART, &USART_InitStructure);
    
    USART_Enable(RS485_USART, ENABLE);

    /* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel			  = RS485_INT_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd		  = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable USART interrupts */
    USART_ConfigInt(RS485_USART, USART_INT_RXDNE, ENABLE);
    USART_ConfigInt(USART3, USART_INT_TXDE, ENABLE);

	/*控制485芯片进入接收模式*/
	RS485_RX_EN();
}

 
/***************** 发送一个字符  **********************/
//使用单字节数据发送前要使能发送引脚，发送后要使能接收引脚。
void RS485_SendByte(uint8_t ch)
{
	/* 发送一个字节数据到RS485_USART */
	USART_SendData(RS485_USART,ch);
	/* 等待发送完毕 */
	while (USART_GetFlagStatus(RS485_USART, USART_FLAG_TXDE) == RESET);	
}
/*****************  发送指定长度的字符串 **********************/
void RS485_SendStr_length( uint8_t *str,uint32_t strlen )
{
	unsigned int k=0;
	RS485_TX_EN();//	使能发送数据	
    do {
        RS485_SendByte( *(str + k) );
        k++;
    } while(k < strlen);
	/*加短暂延时，保证485发送数据完毕*/
	delay_us(0xFFF);
	RS485_RX_EN();//	使能接收数据
}
/*****************  发送字符串 **********************/
void RS485_SendString(uint8_t *str)
{
	unsigned int k=0;
	RS485_TX_EN();//	使能发送数据
    do {
        RS485_SendByte(  *(str + k) );
        k++;
    } while(*(str + k)!='\0');
	/*加短暂延时，保证485发送数据完毕*/
	delay_us(0xFFF);
	RS485_RX_EN();//	使能接收数据
}

void *xtp_memset(void *src, unsigned char ch, unsigned short count) {
    unsigned char *tmp = (unsigned char *) src;
    if (src == NULL) {
        return NULL;
    }
    while (count--) {
        *tmp++ = ch;
    }
    return src;
}

//中断缓存串口数据
#define UART_BUFF_SIZE      1024
uint16_t uart_p = 0;
uint8_t  uart_buff[UART_BUFF_SIZE];

void RS485_IRQHandler(void)
{
    uint8_t data = 0;
    if(USART_GetIntStatus(RS485_USART, USART_INT_RXDNE) != RESET) {
        USART_ClrIntPendingBit(RS485_USART, USART_INT_RXDNE);
        data = USART_ReceiveData(RS485_USART);
        uart_buff[uart_p++] = data;
    }
}

//获取接收到的数据和长度
char *get_rebuff(uint16_t *len)
{
    *len = uart_p;
    return (char *)&uart_buff;
}

//清空缓冲区
void clean_rebuff(void)
{
    uart_p = 0;
    xtp_memset(uart_buff, 0x00, UART_BUFF_SIZE);
}

