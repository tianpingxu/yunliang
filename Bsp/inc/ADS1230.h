#ifndef ADS1230_H
#define ADS1230_H

#define POWER_DOWN_1(value)	GPIO_WriteBit(GPIOA, GPIO_PIN_1,(value == LOW) ? Bit_RESET : Bit_SET)
#define POWER_DOWN_2(value)	GPIO_WriteBit(GPIOB, GPIO_PIN_14,(value == LOW) ? Bit_RESET : Bit_SET)

#define ADS1230_SCLK1(value)		GPIO_WriteBit(GPIOD, GPIO_PIN_15,(value == LOW) ? Bit_RESET : Bit_SET)
#define ADS1230_SCLK2(value)		GPIO_WriteBit(GPIOB, GPIO_PIN_13,(value == LOW) ? Bit_RESET : Bit_SET)

#define ADS1230_DOUT1_LOW()	((GPIO_ReadInputDataBit(GPIOD, GPIO_PIN_14) == Bit_RESET) ? TRUE : FALSE)
#define ADS1230_DOUT2_LOW()	((GPIO_ReadInputDataBit(GPIOB, GPIO_PIN_12) == Bit_RESET) ? TRUE : FALSE)

#define ADS1230_DOUT1_STATUS()	((GPIO_ReadInputDataBit(GPIOD, GPIO_PIN_14) == Bit_RESET) ? 0 : 1)
#define ADS1230_DOUT2_STATUS()	((GPIO_ReadInputDataBit(GPIOB, GPIO_PIN_12) == Bit_RESET) ? 0 : 1)


void vADS1230_Init(void);
void vADS1230_PowerDown(void);
void vADS1230_Offset1(void);
void vADS1230_Offset2(void);

LONG vADS1230_Read1(void);
LONG vADS1230_Read2(void);
#endif
