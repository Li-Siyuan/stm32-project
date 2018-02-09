#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f10x.h"
#include "gui.h"


#define DHT_DATA_GPIO_PORT    	GPIOA			              /* GPIO端口 */
#define DHT_DATA_GPIO_CLK 	    RCC_APB2Periph_GPIOA		/* GPIO端口时钟 */
#define DHT_DATA_GPIO_PIN		    GPIO_Pin_4			        /* 连接到SCL时钟线的GPIO */



#define DHT_DATA_LOW 			(DHT_DATA_GPIO_PORT->BRR |= DHT_DATA_GPIO_PIN)
#define DHT_DATA_HIGH 			(DHT_DATA_GPIO_PORT->BSRR |= DHT_DATA_GPIO_PIN)
#define DHT_DATA_READ 			(DHT_DATA_GPIO_PORT->IDR  & DHT_DATA_GPIO_PIN)





void DHT_Start(u8 DATA[5]);





#endif


