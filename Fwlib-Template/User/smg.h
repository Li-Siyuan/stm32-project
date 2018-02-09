/*********188开发板专用led函数****************
**********作者：李思远******************
**********参考：YFROBOT*****************
**************************************/

#ifndef __SMG_H
#define __SMG_H

#include "stm32f10x.h"


/*  数码管时钟端口、引脚定义 */
#define SMG_PORT 			GPIOC   
#define SMG_PIN 			(GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7)
//SMG_PORT_RCC		RCC_APB2Periph_GPIOC


			
void SMG_Conf(void);
void SMG_SHOW(uint8_t a);



#endif


