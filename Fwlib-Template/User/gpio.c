/*********188开发板专用led函数****************
**********作者：李思远******************
**********参考：秉火*普中*****************
**********新增：GPIO使用模板及位带操作，位带操作见.h文件******************************/

#include "gpio.h"



/***************GPIO初始化模板************/
void GPIO_Conf(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//GPIO_Mode_Out_PP;GPIO_Mode_Out_OD;GPIO_Mode_IPU;GPIO_Mode_IPD;GPIO_Mode_IN_FLOATING;GPIO_Mode_AF_PP;GPIO_Mode_AF_OD;GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}
/**************************************/




/****************GPIO输出高低电平模板*****
//写一位或几位
#define GPIOA0_LOW 			(GPIOA->BRR |= GPIO_Pin_0)
#define GPIOA0_HIGH 		(GPIOA->BSRR |= GPIO_Pin_0)
//写所有位
#define GPIOA_WRITE     (GPIOA->ODR)   //GPIOA_WRITE = 0xff
**************************************/

/****************读取电平状态**************
#define GPIOA0_READ 			(GPIOA->IDR & GPIO_Pin_0)
#define GPIOA_READ 			  (GPIOA->IDR)
**************************************/

