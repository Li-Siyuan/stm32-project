/*********188开发板专用led函数****************
**********作者：李思远******************
**********参考：西电小马哥******************
**********新增：用宏定义注释及声明相应中断服务函数******************************/

#include "nvic.h"

#define NVIC_PRIORITY_GROUP_0   3    //16    0
#define NVIC_PRIORITY_GROUP_1   4    //8     2
#define NVIC_PRIORITY_GROUP_2   5    //4     4
#define NVIC_PRIORITY_GROUP_3   6    //2     9
#define NVIC_PRIORITY_GROUP_4   7    //0     16



void NVIC_Conf(void)
{
	NVIC_SetPriority(SysTick_IRQn,NVIC_EncodePriority( NVIC_PRIORITY_GROUP_1,0,0));
	
//	NVIC_SetPriority(RTC_IRQn,NVIC_EncodePriority( NVIC_PRIORITY_GROUP_1,0,0));
//	NVIC_SetPriority( PVD_IRQn,NVIC_EncodePriority( NVIC_PRIORITY_GROUP_1,0,0));
	
//	NVIC_SetPriority(WWDG_IRQn,NVIC_EncodePriority( NVIC_PRIORITY_GROUP_1,0,0));
//NVIC_SetPriority(ADC_IRQ,NVIC_EncodePriority( NVIC_PRIORITY_GROUP_1,2,0));
//	NVIC_SetPriority(NRF_EXTI_IRQ,NVIC_EncodePriority( NVIC_PRIORITY_GROUP_1,0,0));
	
//	NVIC_SetPriority(EAGLE_RCLK_EXTI_IRQ,NVIC_EncodePriority( NVIC_PRIORITY_GROUP_1,0,2));
//	NVIC_SetPriority(EAGLE_VSYNC_EXTI_IRQ,NVIC_EncodePriority( NVIC_PRIORITY_GROUP_1,0,3));
//	NVIC_SetPriority(EAGLE_DMA_IRQ,NVIC_EncodePriority( NVIC_PRIORITY_GROUP_1,0,1));
//	
	//NVIC_SetPriority(USART_IRQ,NVIC_EncodePriority( NVIC_PRIORITY_GROUP_1,0,2));
  //NVIC_SetPriority(OV7725_VSYNC_EXTI_IRQ,NVIC_EncodePriority( NVIC_PRIORITY_GROUP_1,0,1));
//		
//	NVIC_SetPriority(DMP_EXTI_IRQ,NVIC_EncodePriority(NVIC_PRIORITY_GROUP_2,1,0));
//		
//	NVIC_SetPriority(KEYUP_INT_EXTI_IRQ,NVIC_EncodePriority(NVIC_PRIORITY_GROUP_2,1,0));

//	NVIC_SetPriority(KEY0_INT_EXTI_IRQ,NVIC_EncodePriority(NVIC_PRIORITY_GROUP_2,1,0));
// 
//	NVIC_SetPriority(KEY1_INT_EXTI_IRQ,NVIC_EncodePriority(NVIC_PRIORITY_GROUP_2,1,0));
//	NVIC_SetPriority(KEY2_INT_EXTI_IRQ,NVIC_EncodePriority(NVIC_PRIORITY_GROUP_2,1,0));	
	

//  NVIC_SetPriority(USART1_IRQn,NVIC_EncodePriority(NVIC_PRIORITY_GROUP_2,1,1));
//	NVIC_SetPriority(USART2_IRQn,NVIC_EncodePriority(NVIC_PRIORITY_GROUP_2,1,1));
//	NVIC_SetPriority(USART3_IRQn,NVIC_EncodePriority(NVIC_PRIORITY_GROUP_2,1,1));
//  NVIC_SetPriority(UART4_IRQn,NVIC_EncodePriority(NVIC_PRIORITY_GROUP_2,1,1));
//		  
//	//Base_TIM
//	NVIC_SetPriority(TIM6_IRQn,NVIC_EncodePriority(NVIC_PRIORITY_GROUP_2,1,1));
//	NVIC_SetPriority(TIM7_IRQn,NVIC_EncodePriority(NVIC_PRIORITY_GROUP_2,1,1));
//  //ADVANCE_TIM
//	NVIC_SetPriority(TIM1_UP_IRQn,NVIC_EncodePriority(NVIC_PRIORITY_GROUP_2,1,0));
//	NVIC_SetPriority(TIM1_CC_IRQn,NVIC_EncodePriority(NVIC_PRIORITY_GROUP_2,1,1));
//	NVIC_SetPriority(TIM8_UP_IRQn,NVIC_EncodePriority(NVIC_PRIORITY_GROUP_2,1,1));
//	NVIC_SetPriority(TIM8_CC_IRQn,NVIC_EncodePriority(NVIC_PRIORITY_GROUP_2,1,1));	
//	//GENERAL_TIM
//	NVIC_SetPriority(TIM2_IRQn,NVIC_EncodePriority(NVIC_PRIORITY_GROUP_2,1,1));
//	NVIC_SetPriority(TIM3_IRQn,NVIC_EncodePriority(NVIC_PRIORITY_GROUP_2,1,1));	
//	NVIC_SetPriority(TIM4_IRQn,NVIC_EncodePriority(NVIC_PRIORITY_GROUP_2,1,1));
//	NVIC_SetPriority(TIM5_IRQn,NVIC_EncodePriority(NVIC_PRIORITY_GROUP_2,1,1));	
//	
	
}


