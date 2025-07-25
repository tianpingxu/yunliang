#ifndef __OLED_SPI_H__
#define __OLED_SPI_H__

#include "n32l40x.h"

//PORT: GPIOC
#define OLED_NSS GPIO_PIN_13
#define OLED_RST GPIO_PIN_14
#define OLED_DC  GPIO_PIN_15

//PORT: GPIOA
#define OLED_SCL GPIO_PIN_5
#define OLED_SDA GPIO_PIN_7
#define OLED_EN  GPIO_PIN_15


#define SPI_MASTER                SPI1
#define SPI_MASTER_CLK            RCC_APB2_PERIPH_SPI1
#define SPI_MASTER_GPIO           GPIOA
#define SPI_MASTER_GPIO_CLK       RCC_APB2_PERIPH_GPIOA

#define SPI_MASTER_PIN_NSS        GPIO_PIN_13

#define SPI_MASTER_PIN_SCK        GPIO_PIN_5
#define SPI_MASTER_PIN_MOSI       GPIO_PIN_7

#define SPI_MASTER_Tx_DMA_Channel DMA_CH4
#define SPI_MASTER_Tx_DMA_FLAG    DMA_FLAG_TC4

#define SPI_MASTER_DR_Base        SPI1_BASE+0x0c //0x4001300C

void OLED_SPI_Init(void);
void OLED_DMA_Init(void);
void RCC_Configuration(void);
void GPIO_Configuration(void);

#endif
