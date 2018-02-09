/*********188开发板专用led函数****************
**********作者：李思远******************
**********参考：YFROBOT*****************
**********新增：封装7个音节******************************/

#ifndef __BEEP_H
#define __BEEP_H	 
#include "stm32f10x.h"
#include "gpio.h"



//蜂鸣器端口定义
#define BEEP PBout(5)	// BEEP,蜂鸣器接口		   
//音符 半周期
#define DO  1911
#define RE  1702
#define MI  1516
#define FA  1431
#define SO  1275
#define LA  1136
#define SI  1012

void BEEP_Conf(void);	    //初始化		
void BEEP_ON(u32 times,u16 note); //蜂鸣器发出响声
#endif


