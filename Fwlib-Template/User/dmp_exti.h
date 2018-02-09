#ifndef __DMP_EXTI_H
#define __DMP_EXTI_H



#include "stm32f10x.h"






#define DMP_EXTI_GPIO_PORT         GPIOA
#define DMP_EXTI_GPIO_CLK          (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)
#define DMP_EXTI_GPIO_PIN          GPIO_Pin_4
#define DMP_EXTI_PORTSOURCE        GPIO_PortSourceGPIOA
#define DMP_EXTI_PINSOURCE         GPIO_PinSource4
#define DMP_EXTI_LINE              EXTI_Line4
#define DMP_EXTI_IRQ               EXTI4_IRQn

#define DMP_EXTI_IRQHandler            EXTI4_IRQHandler













void DMP_EXTI_CONF(void);





#endif


