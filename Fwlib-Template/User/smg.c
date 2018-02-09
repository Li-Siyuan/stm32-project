/*********188开发板专用led函数****************
**********作者：李思远******************
**********参考：YFROBOT*****************
**************************************/

#include "smg.h"

u8 smgduan[16]={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
             0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};//0~F 数码管段选数据

/*******************************************************************************
* 函 数 名         : SMG_Init
* 函数功能		   : 数码管初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void SMG_Conf(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;  //声明一个结构体变量，用来初始化GPIO
	/* 开启GPIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

	
	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=SMG_PIN;	  //选择你要设置的IO口
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(SMG_PORT,&GPIO_InitStructure);		/* 初始化GPIO */
}


//显示数码管段选数据 a[15:0]						 
void SMG_SHOW(uint8_t a)
{
  (GPIOC->ODR = ~smgduan[a]); 
}


