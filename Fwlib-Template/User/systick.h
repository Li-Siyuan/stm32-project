/*********188开发板专用led函数****************
**********作者：李思远******************
**********参考：秉火*普中*****************
**********新增：三种延时函数*并兼容各种编程风格****************************/

#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
#define _ms       1000
#define _us       1000000


//兼容各种编程风格
#define delay_ms     					Delay_Ms
#define delay_us     					Delay_Us
#define SYSTICK_Delay1ms      delay_ms
#define DELAY_MS              Delay_Ms
#define DELAY_US     					Delay_Us
#define Delay_us              Delay_Us
#define Delay_ms              Delay_Ms

//需初始化及中断服务函数
void SysTick_Conf(uint32_t time);
void Delay(__IO u32 nTime);

//不需初始化及中断服务函数
void SysTick_Delay_Us( __IO uint32_t us);
void SysTick_Delay_Ms( __IO uint32_t ms);

//直接操作寄存器
void Delay_Us(u32 i);
void Delay_Ms(u32 i);

//兼容普中例程
void delay(u32 i);

#endif /* __SYSTICK_H */
