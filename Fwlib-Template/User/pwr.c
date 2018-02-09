#if 0

//用官方库函数即可，因此处只列举了部分常用PWR官方库函数

//先开时钟：RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); 

//(1)配置PVD监测功能阈值等级:void PWR_PVDLevelConfig(uint32_t PWR_PVDLevel)


//(2)进入睡眠模式: （可用任意中断唤醒）
//			__WFI();__WFE();



//(3)进入停止模式:（可用任意中断唤醒，/*从停止模式下被唤醒后使用的是HSI时钟，此处重启HSE时钟*/PLLCLKSYSCLKConfig_STOP();）
//			void PWR_EnterSTOPMode(uint32_t PWR_Regulator, uint8_t PWR_STOPEntry)
//      唤醒后重置时钟
	
//（	
//				可以用下列两个函数获取当前时钟状态
//				//时钟源
//				clock_source_wakeup = RCC_GetSYSCLKSource ();
//				//时钟频率
//				RCC_GetClocksFreq(&clock_status_wakeup);
// ）


//(4)进入待机模式:（可用WKUP引脚A0上升沿唤醒）
//			/*清除WU状态位*/
//			PWR_ClearFlag (PWR_FLAG_WU);
//			
//			/* 使能WKUP引脚的唤醒功能 ，使能PA0*/
//			PWR_WakeUpPinCmd (ENABLE);
//			
//			/* 进入待机模式 */
//			PWR_EnterSTANDBYMode();









#endif

