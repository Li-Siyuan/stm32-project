#ifndef __EXTI_H
#define	__EXTI_H


#include "stm32f10x.h"

//Òý½Å¶¨Òå
#define KEYUP_INT_GPIO_PORT         GPIOA
#define KEYUP_INT_GPIO_CLK          (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)
#define KEYUP_INT_GPIO_PIN          GPIO_Pin_0
#define KEYUP_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA
#define KEYUP_INT_EXTI_PINSOURCE    GPIO_PinSource0
#define KEYUP_INT_EXTI_LINE         EXTI_Line0
#define KEYUP_INT_EXTI_IRQ          EXTI0_IRQn

#define KEYUP_IRQHandler            EXTI0_IRQHandler


#define KEY0_INT_GPIO_PORT         GPIOE
#define KEY0_INT_GPIO_CLK          (RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO)
#define KEY0_INT_GPIO_PIN          GPIO_Pin_2
#define KEY0_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOE
#define KEY0_INT_EXTI_PINSOURCE    GPIO_PinSource2
#define KEY0_INT_EXTI_LINE         EXTI_Line2
#define KEY0_INT_EXTI_IRQ          EXTI2_IRQn

#define KEY0_IRQHandler            EXTI2_IRQHandler



#define KEY1_INT_GPIO_PORT         GPIOE
#define KEY1_INT_GPIO_CLK          (RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO)
#define KEY1_INT_GPIO_PIN          GPIO_Pin_3
#define KEY1_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOE
#define KEY1_INT_EXTI_PINSOURCE    GPIO_PinSource3
#define KEY1_INT_EXTI_LINE         EXTI_Line3
#define KEY1_INT_EXTI_IRQ          EXTI3_IRQn

#define KEY1_IRQHandler            EXTI3_IRQHandler



#define KEY2_INT_GPIO_PORT         GPIOE
#define KEY2_INT_GPIO_CLK          (RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO)
#define KEY2_INT_GPIO_PIN          GPIO_Pin_4
#define KEY2_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOE
#define KEY2_INT_EXTI_PINSOURCE    GPIO_PinSource4
#define KEY2_INT_EXTI_LINE         EXTI_Line4
#define KEY2_INT_EXTI_IRQ          EXTI4_IRQn

#define KEY2_IRQHandler            EXTI4_IRQHandler

void KEY_EXTI_Conf(void);
void KEYUP_EXTI_Conf(void);
void KEY0_EXTI_Conf(void);
void KEY1_EXTI_Conf(void);
void KEY2_EXTI_Conf(void);


#endif /* __EXTI_H */
