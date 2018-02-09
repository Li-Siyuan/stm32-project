 #if 0
 static void Delay(__IO u32 nCount); 

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */ 
int main(void)
{	
		NVIC_Conf();
	// 配置LED GPIO，并关闭LED
	LED_GPIO_Config();	

	Delay(0X8FFFFF);
	/*------------------------------------------------------------*/
		/* 检查是否为独立看门狗复位 */
  if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
  {
    /* 独立看门狗复位 */
    /*  亮红灯 */
    LED_RED;

    /* 清除标志 */
    RCC_ClearFlag();
		
		/*如果一直不喂狗，会一直复位，加上前面的延时，会看到红灯闪烁
		在1s 时间内喂狗的话，则会持续亮绿灯*/
  }
  else
  {
    /*不是独立看门狗复位(可能为上电复位或者手动按键复位之类的) */
    /* 亮蓝灯 */
    LED_BLUE;
  }
	/*--------------------------------------------------------------*/
	
	// 配置按键GPIO
	Key_GPIO_Config();
	// IWDG 1s 超时溢出
	IWDG_Config(IWDG_Prescaler_64 ,625);
	
	//while部分是我们在项目中具体需要写的代码，这部分的程序可以用独立看门狗来监控
    //如果我们知道这部分代码的执行时间，比如是500ms，那么我们可以设置独立看门狗的
	//溢出时间是600ms，比500ms多一点，如果要被监控的程序没有跑飞正常执行的话，那么
	//执行完毕之后就会执行喂狗的程序，如果程序跑飞了那程序就会超时，到达不了喂狗
	//的程序,此时就会产生系统复位。但是也不排除程序跑飞了又跑回来了，刚好喂狗了，
	//歪打正着。所以要想更精确的监控程序，可以使用窗口看门狗，窗口看门狗规定必须在
	//规定的窗口时间内喂狗。
	while(1)                        
	{	
// 这里添加需要被监控的代码，如果有就去掉按键模拟喂狗，把按键扫描程序去掉
//--------------------------------------------------------------------------
		if( Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON  )
		{
			// 喂狗，如果不喂狗，系统则会复位，LED1则会灭一次，如果在1s
			// 时间内准时喂狗的话，则绿会常亮
			IWDG_Feed();
			//喂狗后亮绿灯
			LED_GREEN;
		}   
	}
//---------------------------------------------------------------------------
}

static void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}


/*********************************************END OF FILE**********************/
 #endif
 
 #include "iwdg.h"   

/*
 * 设置 IWDG 的超时时间
 * Tout = prv/40 * rlv (s)
 *      prv可以是[4,8,16,32,64,128,256]
 * prv:预分频器值，取值如下：
 *     @arg IWDG_Prescaler_4: IWDG prescaler set to 4
 *     @arg IWDG_Prescaler_8: IWDG prescaler set to 8
 *     @arg IWDG_Prescaler_16: IWDG prescaler set to 16
 *     @arg IWDG_Prescaler_32: IWDG prescaler set to 32
 *     @arg IWDG_Prescaler_64: IWDG prescaler set to 64
 *     @arg IWDG_Prescaler_128: IWDG prescaler set to 128
 *     @arg IWDG_Prescaler_256: IWDG prescaler set to 256
 *
 * rlv:预分频器值，取值范围为：0-0XFFF
 * 函数调用举例：
 * IWDG_Config(IWDG_Prescaler_64 ,625);  // IWDG 1s 超时溢出
 */

void IWDG_Conf(uint8_t prv ,uint16_t rlv)
{	
	// 使能 预分频寄存器PR和重装载寄存器RLR可写
	IWDG_WriteAccessCmd( IWDG_WriteAccess_Enable );
	
	// 设置预分频器值
	IWDG_SetPrescaler( prv );
	
	// 设置重装载寄存器值
	IWDG_SetReload( rlv );
	
	// 把重装载寄存器的值放到计数器中
	IWDG_ReloadCounter();
	
	// 使能 IWDG
	IWDG_Enable();	
}

// 喂狗
void IWDG_Feed(void)
{
	// 把重装载寄存器的值放到计数器中，喂狗，防止IWDG复位
	// 当计数器的值减到0的时候会产生系统复位
	IWDG_ReloadCounter();
}


/*********************************************END OF FILE**********************/
