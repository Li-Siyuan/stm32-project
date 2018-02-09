#ifndef __AD5667_H
#define __AD5667_H

#include "stm32f10x.h"


#define  COMMAND                0x1f
//voltage = (dat16 / 65536.0) * 5.0;



//GPIO
#define  AD5667_GPIO_PORT            GPIOB
#define  AD5667_SCL             GPIO_Pin_8
#define  AD5667_SDA             GPIO_Pin_9

#define  AD5667_SDA_SET               GPIO_SetBits(AD5667_GPIO_PORT ,AD5667_SCL)
#define  AD5667_SDA_CLR								GPIO_ResetBits(AD5667_GPIO_PORT ,AD5667_SCL)
#define  AD5667_SCL_SET               GPIO_SetBits(AD5667_GPIO_PORT ,AD5667_SDA)
#define  AD5667_SCL_CLR								GPIO_ResetBits(AD5667_GPIO_PORT ,AD5667_SDA)



//Fun
void AD5667_Init(void);
void AD5667_WRITE_REG(unsigned char com , uint16_t dat16);






#endif

