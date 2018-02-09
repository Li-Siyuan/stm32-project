 #if 0
 /**
  * @brief  主函数
  * @param  无
  * @retval 无
  */ 
int main(void)
{	
	uint8_t wwdg_tr, wwdg_wr; 
	
	NVIC_Conf();
	
	// 配置LED GPIO，并关闭LED
	LED_GPIO_Config();

	LED1(ON) ;
	SOFT_Delay(0X00FFFFFF);	
	
	// 初始化WWDG：配置计数器初始值，配置上窗口值，启动WWDG，使能提前唤醒中断
	WWDG_Config(0X7F, 0X5F, WWDG_Prescaler_8);
	
	// 窗口值我们在初始化的时候设置成0X5F，这个值不会改变
	wwdg_wr = WWDG->CFR & 0X7F;

	while(1)
	{	
		LED1(OFF);
		//-----------------------------------------------------
		// 这部分应该写需要被WWDG监控的程序，这段程序运行的时间
		// 决定了窗口值应该设置成多大。
		//-----------------------------------------------------
		
		// 计时器值，初始化成最大0X7F，当开启WWDG时候，这个值会不断减小
		// 当计数器的值大于窗口值时喂狗的话，会复位，当计数器减少到0X40
		// 还没有喂狗的话就非常非常危险了，计数器再减一次到了0X3F时就复位
		// 所以要当计数器的值在窗口值和0X40之间的时候喂狗，其中0X40是固定的。
		wwdg_tr = WWDG->CR & 0X7F;
		
		if( wwdg_tr < wwdg_wr )
		{
			// 喂狗，重新设置计数器的值为最大0X7F
			// 
			WWDG_Feed();
		}
	}
}
 #endif



#include "wwdg.h"   



/* WWDG 配置函数
 * tr ：递减计时器的值， 取值范围为：0x7f~0x40
 * wr ：窗口值，取值范围为：0x7f~0x40
 * prv：预分频器值，取值可以是
 *      @arg WWDG_Prescaler_1: WWDG counter clock = (PCLK1(36MHZ)/4096)/1
 *      @arg WWDG_Prescaler_2: WWDG counter clock = (PCLK1(36mhz)/4096)/2
 *      @arg WWDG_Prescaler_4: WWDG counter clock = (PCLK1(36mhz)/4096)/4
 *      @arg WWDG_Prescaler_8: WWDG counter clock = (PCLK1(36mhz)/4096)/8
 */
void WWDG_Conf(uint8_t tr, uint8_t wr, uint32_t prv)
{	
	// 开启 WWDG 时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	
	// 设置递减计数器的值
	WWDG_SetCounter( tr );
	
	// 设置预分频器的值
	WWDG_SetPrescaler( prv );
	
	// 设置上窗口值
	WWDG_SetWindowValue( wr );
	
	// 设置计数器的值，使能WWDG
	WWDG_Enable(WWDG_CNT);	
	
	// 清除提前唤醒中断标志位
	WWDG_ClearFlag();	
	// 使能WWDG中断
	NVIC_EnableIRQ(WWDG_IRQn);
	// 开WWDG 中断
	WWDG_EnableIT();
}

// 喂狗
void WWDG_Feed(void)
{
	// 喂狗，刷新递减计数器的值，设置成最大WDG_CNT=0X7F
	WWDG_SetCounter( WWDG_CNT );
}


/*********************************************END OF FILE**********************/
