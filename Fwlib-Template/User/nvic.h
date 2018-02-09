/*********188开发板专用led函数****************
**********作者：李思远******************
**********参考：西电小马哥******************
**********新增：用宏定义注释及声明相应中断服务函数******************************/

#ifndef __NVIC_H
#define __NVIC_H

#include "stm32f10x.h"
#include "include.h"

//中断源
//#define DMP_EXTI_IRQ               EXTI4_IRQn

//#define KEYUP_INT_EXTI_IRQ         EXTI0_IRQn
//#define KEY0_INT_EXTI_IRQ          EXTI2_IRQn
//#define KEY1_INT_EXTI_IRQ          EXTI3_IRQn
//#define KEY2_INT_EXTI_IRQ          EXTI4_IRQn







//#define __OV7725_VSYNC_EXTI_INT_FUNCTION
//  #define __DMP_EXTI_IRQHandler 

//  #define __KEYUP_IRQHandler
//  #define __KEY0_IRQHandler
//	#define __KEY1_IRQHandler
//	#define __KEY2_IRQHandler
		
//#define __USART1_IRQHandler
//#define __USART2_IRQHandler
//#define __USART3_IRQHandler
//#define __USART_IRQHandler
//#define __UART5_IRQHandler

//BASE_TIM
//#define __TIM6_IRQHandler
//#define __TIM7_IRQHandler

//ADVANCE_TIM
//#define __TIM1_UP_IRQHandler
//#define __TIM1_CC_IRQHandler
//#define __TIM8_UP_IRQHandler
//#define __TIM8_CC_IRQHandler

//GENERAL_TIM
//#define __TIM2_IRQHandler
//#define __TIM3_IRQHandler
//#define __TIM4_IRQHandler
//#define __TIM5_IRQHandler


//#define __EAGLE_VSYNC_EXTI_INT_FUNCTION
//#define __EAGLE_RCLK_EXTI_INT_FUNCTION
//#define __EAGLE_DMA_Handler 

//#define __DMA_Handler

//#define __NRF_EXTI_IRQHANDLER
//#define   __ADC_IRQHANDLER
//#define __WWDG_IRQHANDLER
//#define __PVD_IRQHANDLER
//#define __RTC_IRQHANDLER


void NVIC_Conf(void);











#endif


