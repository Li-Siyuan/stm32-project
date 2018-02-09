/*********188开发板专用led函数****************
**********作者：李思远******************
**********参考：YFROBOT*****************
**********新增：封装7个音节******************************/
#include "beep.h"
#include "sysTick.h"

	    

/******************************************************
Function:   void BEEP_Init(void)
Description:初始化PA8为输出口,蜂鸣器初始化
******************************************************/
void BEEP_Conf(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}
/******************************************************
Function:   void beepBeep(void)
Description:蜂鸣器发出响声
Input: times 发声时间
       note  音符
#define DO  1911
#define RE  1702
#define MI  1516
#define FA  1431
#define SO  1275
#define LA  1136
#define SI  1012
******************************************************/
void BEEP_ON(u32 times,u16 note)
{
	u32 i;
	for(i = times; i>0; i--)
	{
	  BEEP=0;
		SysTick_Delay_Us(note);
		BEEP=1;
    SysTick_Delay_Us(note);
	}	
}


