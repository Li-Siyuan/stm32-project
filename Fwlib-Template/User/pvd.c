/**
  ******************************************************************************
  * @file    bsp_pvd.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   pvd监控配置
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 霸道 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "pvd.h" 



/**
  * @brief  配置PVD.
  * @param  None
  * @retval None
  */
void PVD_Conf(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;

  /*使能 PWR 时钟 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); 
      
  /* 配置 EXTI16线(PVD 输出) 来产生上升下降沿中断*/
  EXTI_ClearITPendingBit(EXTI_Line16);
  EXTI_InitStructure.EXTI_Line = EXTI_Line16;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

	NVIC_EnableIRQ(PVD_IRQn);
	
  /* 配置PVD级别PWR_PVDLevel_2V6 (PVD检测电压的阈值为2.6V，VDD电压低于2.6V时产生PVD中断) */
	/*具体级别根据自己的实际应用要求配置*/
  PWR_PVDLevelConfig(PWR_PVDLevel_2V6);

  /* 使能PVD输出 */
  PWR_PVDCmd(ENABLE);
}

/*********************************************END OF FILE**********************/
