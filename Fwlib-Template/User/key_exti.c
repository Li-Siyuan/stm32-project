/*********188开发板专用key函数****************
**********作者：李思远******************
**********参考：秉火******************
**********新增：按键中断初始化分开，且屏蔽上下拉按键的硬件差异*****************************/

#include "key_exti.h"

/*
配置嵌套向量中断控制器NVIC，静态函数，不能被外部调用
*/

/*
static void NVIC_Conf(void)
{
	//定义NVIC初始化结构体，选优先级组1,并配置结构体
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //抢占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;   //子优先级为1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //使能中断
	//配置KEYUP的NVIC
	NVIC_InitStructure.NVIC_IRQChannel = KEYUP_INT_EXTI_IRQ;  //选中断通道
	NVIC_Init(&NVIC_InitStructure);
	//配置KEY0的NVIC
	NVIC_InitStructure.NVIC_IRQChannel = KEY0_INT_EXTI_IRQ;  //选中断通道
	NVIC_Init(&NVIC_InitStructure);
	//配置KEY1的NVIC
	NVIC_InitStructure.NVIC_IRQChannel = KEY1_INT_EXTI_IRQ;  //选中断通道
	NVIC_Init(&NVIC_InitStructure);
	//配置KEY2的NVIC
	NVIC_InitStructure.NVIC_IRQChannel = KEY2_INT_EXTI_IRQ;  //选中断通道
	NVIC_Init(&NVIC_InitStructure);
}
*/

void KEYUP_EXTI_Conf(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(KEYUP_INT_GPIO_CLK,ENABLE);
	//NVIC_Conf();  //配置NVIC
	
	 GPIO_InitStructure.GPIO_Pin = KEYUP_INT_GPIO_PIN;
  /* 配置为浮空输入 */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(KEYUP_INT_GPIO_PORT, &GPIO_InitStructure);

	/* 选择EXTI的信号源 */
  GPIO_EXTILineConfig(KEYUP_INT_EXTI_PORTSOURCE, KEYUP_INT_EXTI_PINSOURCE); 
  EXTI_InitStructure.EXTI_Line = KEYUP_INT_EXTI_LINE;
	
	/* EXTI为中断模式 */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* 上升沿中断 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  /* 使能中断 */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	NVIC_EnableIRQ(KEYUP_INT_EXTI_IRQ);
}

void KEY0_EXTI_Conf(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(KEY0_INT_GPIO_CLK,ENABLE);
	//NVIC_Conf();   //配置NVIC
	
	 GPIO_InitStructure.GPIO_Pin = KEY0_INT_GPIO_PIN;
  /* 配置为浮空输入 */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(KEY0_INT_GPIO_PORT, &GPIO_InitStructure);

	/* 选择EXTI的信号源 */
  GPIO_EXTILineConfig(KEY0_INT_EXTI_PORTSOURCE, KEY0_INT_EXTI_PINSOURCE); 
  EXTI_InitStructure.EXTI_Line = KEY0_INT_EXTI_LINE;
	
	/* EXTI为中断模式 */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* 上升沿中断 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  /* 使能中断 */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	 NVIC_EnableIRQ(KEY0_INT_EXTI_IRQ);
}

void KEY1_EXTI_Conf(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(KEY0_INT_GPIO_CLK,ENABLE);
	//NVIC_Conf();      //配置NVIC
	
	 GPIO_InitStructure.GPIO_Pin = KEY1_INT_GPIO_PIN;
  /* 配置为上拉输入 */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitStructure);

	/* 选择EXTI的信号源 */
  GPIO_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE, KEY1_INT_EXTI_PINSOURCE); 
  EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE;
	
	/* EXTI为中断模式 */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* 上升沿中断 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  /* 使能中断 */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	NVIC_EnableIRQ(KEY1_INT_EXTI_IRQ);
}

void KEY2_EXTI_Conf(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(KEY2_INT_GPIO_CLK,ENABLE);
	//NVIC_Conf();   //配置NVIC
	
	 GPIO_InitStructure.GPIO_Pin = KEY2_INT_GPIO_PIN;
  /* 配置为浮空输入 */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(KEY0_INT_GPIO_PORT, &GPIO_InitStructure);

	/* 选择EXTI的信号源 */
  GPIO_EXTILineConfig(KEY2_INT_EXTI_PORTSOURCE, KEY2_INT_EXTI_PINSOURCE); 
  EXTI_InitStructure.EXTI_Line = KEY2_INT_EXTI_LINE;
	
	/* EXTI为中断模式 */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* 上升沿中断 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  /* 使能中断 */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	NVIC_EnableIRQ(KEY2_INT_EXTI_IRQ);
}

