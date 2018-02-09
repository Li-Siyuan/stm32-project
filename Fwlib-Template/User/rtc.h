#ifndef _rtc_H
#define _rtc_H
#include "stm32f10x.h"

//定义全局变量
extern u8 timebz;
extern u8 timeclock;
extern uint8_t Lunar1[200],Lunar2[200],Term24[200];
void RTC_Conf(void);
void RTC_CLOCK_Conf(void);
void Time_Calculate(uint32_t TimeVar);

//北京时间的时区秒数差
#define TIME_ZOOM						(8*60*60)

#endif
