/*********188开发板专用led函数****************
**********作者：李思远******************
**********参考：秉火******************
**********新增：led单个初始化函数******************************/

#include "led.h"

/***
初始化LED的IO2_GPIO
**/
void LED_Conf(void)
{
	/***初始化结构体，时钟，配置模式，速度***/
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(LED0_GPIO_CLK|LED1_GPIO_CLK|LED2_GPIO_CLK|LED3_GPIO_CLK|LED4_GPIO_CLK|LED5_GPIO_CLK|LED6_GPIO_CLK|LED7_GPIO_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/*****初始化LED0并关灯**************/
	GPIO_InitStructure.GPIO_Pin = LED0_GPIO_PIN;
	GPIO_Init(LED0_GPIO_PORT,&GPIO_InitStructure);
	LED0(OFF);
	/*****初始化LED1并关灯***************/
	GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;
	GPIO_Init(LED1_GPIO_PORT,&GPIO_InitStructure);
	LED1(OFF);
	/*****初始化LED2并关灯***************/
	GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN;
	GPIO_Init(LED2_GPIO_PORT,&GPIO_InitStructure);
	LED2(OFF);
	/*****初始化LED3并关灯***************/
	GPIO_InitStructure.GPIO_Pin = LED3_GPIO_PIN;
	GPIO_Init(LED3_GPIO_PORT,&GPIO_InitStructure);
	LED3(OFF);
	/*****初始化LED4并关灯***************/
	GPIO_InitStructure.GPIO_Pin = LED4_GPIO_PIN;
	GPIO_Init(LED4_GPIO_PORT,&GPIO_InitStructure);
	LED4(OFF);
	/*****初始化LED5并关灯***************/
	GPIO_InitStructure.GPIO_Pin = LED5_GPIO_PIN;
	GPIO_Init(LED5_GPIO_PORT,&GPIO_InitStructure);
	LED5(OFF);
	/*****初始化LED6并关灯***************/
	GPIO_InitStructure.GPIO_Pin = LED6_GPIO_PIN;
	GPIO_Init(LED6_GPIO_PORT,&GPIO_InitStructure);
	LED6(OFF);
	/*****初始化LED7并关灯***************/
	GPIO_InitStructure.GPIO_Pin = LED7_GPIO_PIN;
	GPIO_Init(LED7_GPIO_PORT,&GPIO_InitStructure);
	LED7(OFF);
}

/*
初始化单个LED
x为0，1，2，3，4，5，6，7
*/
void LEDx_Conf(uint8_t LEDx)
{
	switch(LEDx)
	{
		case 0:
		{
			GPIO_InitTypeDef GPIO_InitStructure;    //定义GPIO初始化结构体
			RCC_APB2PeriphClockCmd(LED0_GPIO_CLK,ENABLE);    //使能LEDx对应GPIO的时钟
			//配置初始化结构体
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
			GPIO_InitStructure.GPIO_Pin = LED0_GPIO_PIN;  
			//调用固件库初始化GPIO
			GPIO_Init(LED0_GPIO_PORT,&GPIO_InitStructure);
			//关LEDx
			GPIO_SetBits(LED0_GPIO_PORT, LED0_GPIO_PIN);	
			break;
		}
		case 1:
		{
			GPIO_InitTypeDef GPIO_InitStructure;    //定义GPIO初始化结构体
			RCC_APB2PeriphClockCmd(LED1_GPIO_CLK,ENABLE);    //使能LEDx对应GPIO的时钟
			//配置初始化结构体
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
			GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;  
			//调用固件库初始化GPIO
			GPIO_Init(LED1_GPIO_PORT,&GPIO_InitStructure);
			//关LEDx
			GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);	
			break;
		}
		case 2:
		{
			GPIO_InitTypeDef GPIO_InitStructure;    //定义GPIO初始化结构体
			RCC_APB2PeriphClockCmd(LED2_GPIO_CLK,ENABLE);    //使能LEDx对应GPIO的时钟
			//配置初始化结构体
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
			GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN;  
			//调用固件库初始化GPIO
			GPIO_Init(LED2_GPIO_PORT,&GPIO_InitStructure);
			//关LEDx
			GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);	
			break;
		}
		case 3:
		{
			GPIO_InitTypeDef GPIO_InitStructure;    //定义GPIO初始化结构体
			RCC_APB2PeriphClockCmd(LED3_GPIO_CLK,ENABLE);    //使能LEDx对应GPIO的时钟
			//配置初始化结构体
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
			GPIO_InitStructure.GPIO_Pin = LED3_GPIO_PIN;  
			//调用固件库初始化GPIO
			GPIO_Init(LED3_GPIO_PORT,&GPIO_InitStructure);
			//关LEDx
			GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);	
			break;
		}
		case 4:
		{
			GPIO_InitTypeDef GPIO_InitStructure;    //定义GPIO初始化结构体
			RCC_APB2PeriphClockCmd(LED4_GPIO_CLK,ENABLE);    //使能LEDx对应GPIO的时钟
			//配置初始化结构体
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
			GPIO_InitStructure.GPIO_Pin = LED4_GPIO_PIN;  
			//调用固件库初始化GPIO
			GPIO_Init(LED4_GPIO_PORT,&GPIO_InitStructure);
			//关LEDx
			GPIO_SetBits(LED4_GPIO_PORT, LED4_GPIO_PIN);	
			break;
		}
		case 5:
		{
			GPIO_InitTypeDef GPIO_InitStructure;    //定义GPIO初始化结构体
			RCC_APB2PeriphClockCmd(LED5_GPIO_CLK,ENABLE);    //使能LEDx对应GPIO的时钟
			//配置初始化结构体
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
			GPIO_InitStructure.GPIO_Pin = LED5_GPIO_PIN;  
			//调用固件库初始化GPIO
			GPIO_Init(LED5_GPIO_PORT,&GPIO_InitStructure);
			//关LEDx
			GPIO_SetBits(LED5_GPIO_PORT, LED5_GPIO_PIN);	
			break;
		}
		case 6:
		{
			GPIO_InitTypeDef GPIO_InitStructure;    //定义GPIO初始化结构体
			RCC_APB2PeriphClockCmd(LED6_GPIO_CLK,ENABLE);    //使能LEDx对应GPIO的时钟
			//配置初始化结构体
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
			GPIO_InitStructure.GPIO_Pin = LED6_GPIO_PIN;  
			//调用固件库初始化GPIO
			GPIO_Init(LED6_GPIO_PORT,&GPIO_InitStructure);
			//关LEDx
			GPIO_SetBits(LED6_GPIO_PORT, LED6_GPIO_PIN);	
			break;
		}
		case 7:
		{
			GPIO_InitTypeDef GPIO_InitStructure;    //定义GPIO初始化结构体
			RCC_APB2PeriphClockCmd(LED7_GPIO_CLK,ENABLE);    //使能LEDx对应GPIO的时钟
			//配置初始化结构体
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
			GPIO_InitStructure.GPIO_Pin = LED7_GPIO_PIN;  
			//调用固件库初始化GPIO
			GPIO_Init(LED7_GPIO_PORT,&GPIO_InitStructure);
			//关LEDx
			GPIO_SetBits(LED7_GPIO_PORT, LED7_GPIO_PIN);	
			break;
		}
	}
}

/****************************************************************************
* Function Name  : LED_SetState
* Description    : 设置LED的状态，0代表相应的LED点亮，1表示相应的LED熄灭
* Input          : stateValue：LED的状态
* Output         : None
* Return         : None
****************************************************************************/

void LED_ALL_SET(uint8_t stateValue)
{    
    /* 设置LED灯的状态, GPIO一次设置16位，将其值强制转换位16位 */
    GPIO_SetBits(GPIOC, (uint16_t)stateValue & 0x00FF);         //将要熄灭的LED熄灭
    GPIO_ResetBits(GPIOC, (~(uint16_t)stateValue) & 0x00FF);    //将要点亮的LED点亮                  
}


	// 断言错误时执行的代码
void assert_failed(uint8_t* file, uint32_t line)
{
	LED1_ON;
}

