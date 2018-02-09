#if 0
//万年历
#include "stm32f10x.h"
#include "include.h"


extern struct rtc_time systmtime;
extern struct rtc_time clocktime;

int main()
{	
	u32 timedata=0;
	Site_t site     = {0, 0};                          
	LCD_ST7735S_Conf();
  LCD_ST7735S_str(site,(u8*)"Calendar",FCOLOUR,BCOLOUR);
//  RTC_Conf();	   //RTC配置初始化	  如果想断电后继续走时，把此行注释掉如果想更改起始元年，加上这一句
	RTC_CLOCK_Conf();  //时间初始化
	
//	/*设置闹钟寄存器*/
//	RTC_SetAlarm(Struct_To_UNIX(&clocktime)-TIME_ZOOM);
	
	while(1)
	{
		if(timebz==1)	   //时钟秒中断
		{
			timebz=0;	  //清零
			timedata=RTC_GetCounter();//获取RTC计数器的值
			Time_Calculate(timedata);//时间转换计算

			site.x=0;site.y=20;
			LCD_ST7735S_str(site,(u8*)"year:",FCOLOUR,BCOLOUR);
			site.x=40;site.y=20;
      LCD_num_BC(site,systmtime.tm_year,4,FCOLOUR,BCOLOUR);
			site.x=0;site.y=40;
			LCD_ST7735S_str(site,(u8*)"month:",FCOLOUR,BCOLOUR);
			site.x=48;site.y=40;
      LCD_num_BC(site,systmtime.tm_mon,4,FCOLOUR,BCOLOUR);
			site.x=0;site.y=60;
			LCD_ST7735S_str(site,(u8*)"day:",FCOLOUR,BCOLOUR);
			site.x=32;site.y=60;
      LCD_num_BC(site,systmtime.tm_mday,4,FCOLOUR,BCOLOUR);
			site.x=0;site.y=80;
      LCD_num_BC(site,systmtime.tm_hour,2,FCOLOUR,BCOLOUR);
			site.x=16;site.y=80;
			LCD_ST7735S_str(site,(u8*)":",FCOLOUR,BCOLOUR);
			site.x=24;site.y=80;
			LCD_num_BC(site,systmtime.tm_min,2,FCOLOUR,BCOLOUR);
			site.x=40;site.y=80;
			LCD_ST7735S_str(site,(u8*)":",FCOLOUR,BCOLOUR);
			site.x=48;site.y=80;
			LCD_num_BC(site,systmtime.tm_sec,2,FCOLOUR,BCOLOUR);
		}	
		if(timeclock==1)
		{
			//闹钟时间到
		}
	}			
}
#endif


#include "rtc.h"
#include "date.h"
#include "calendar.h"


/*时间结构体，默认时间2000-01-01 00:00:00,用于作为开始元年*/
struct rtc_time systmtime=
{
0,8,20,3,9,2017,0
};

/*时间结构体，闹钟时间2000-01-01 00:00:08*/
struct rtc_time clocktime=
{
0,8,20,3,9,2017,0
};

//存储时间的字符串
uint8_t Lunar1[200],Lunar2[200],Term24[200];
// tm->tm_year, zodiac_sign[(tm->tm_year-3)%12], tm->tm_mon, tm->tm_mday, WEEK_STR[tm->tm_wday], tm->tm_hour, tm->tm_min, tm->tm_sec;
//UNIX时间戳 = %d 当前时间为: %d年(%s年) %d月 %d日 (星期%s)  %0.2d:%0.2d:%0.2d\r


/* 秒中断标志，进入秒中断时置1，当时间被刷新之后清0 */
u8 timebz = 0;

/*闹钟响铃标志，在中断中闹钟事件致1*/
u8 timeclock = 0;

/*星期，生肖用文字ASCII码*/
char const *WEEK_STR[] = {"日", "一", "二", "三", "四", "五", "六"};
char const *zodiac_sign[] = {"猪", "鼠", "牛", "虎", "兔", "龙", "蛇", "马", "羊", "猴", "鸡", "狗"};

/*英文，星期，生肖用文字ASCII码*/
char const *en_WEEK_STR[] = { "Sunday","Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char const *en_zodiac_sign[] = {"Pig", "Rat", "Ox", "Tiger", "Rabbit", "Dragon", "Snake", "Horse", "Goat", "Monkey", "Rooster", "Dog"};



/*******************************************************************************
* 函 数 名         : rtc_init
* 函数功能		   : RTC时钟、中断初始化	//若想更新元年时间，则用此函数后再用RTC_CLOCK_Conf。若想掉电继续计时，则不用此函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void RTC_Conf()
{
   
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);//打开电源时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);//打开存储器时钟

	PWR_BackupAccessCmd(ENABLE);//使能或者失能RTC和后备寄存器访问
	BKP_DeInit();//将外设BKP的全部寄存器重设为缺省值
	RCC_LSEConfig(RCC_LSE_ON);//设置外部低速晶振（LSE）
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET);//检查指定的RCC标志位设置与否
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//设置RTC时钟（RTCCLK）
	RCC_RTCCLKCmd(ENABLE);//使能或者失能RTC时钟
	RTC_WaitForSynchro(); //等待RTC寄存器同步
	RTC_WaitForLastTask();//等待最近一次对RTC寄存器的写操作完成
	RTC_ITConfig(RTC_IT_SEC,ENABLE);//使能或者失能指定的RTC中断
	RTC_WaitForLastTask();//等待最近一次对RTC寄存器的写操作完成
	RTC_SetPrescaler(32767);//设置预分频 使用外部晶振为32.768K，要想1s中断则预分频数设置为32767，系统会在此数字基础上加1
	RTC_WaitForLastTask();//等待最近一次对RTC寄存器的写操作完成	
		
}

void RTC_CLOCK_Conf()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP|RCC_APB1Periph_PWR,ENABLE);//打开后备备份区域时钟和电源时钟
	PWR_BackupAccessCmd(ENABLE); //使能RTC和后备区域寄存器访问
	if(BKP_ReadBackupRegister(BKP_DR1)!=0XA5A5)//从指定的后备寄存器中读出数据
	{
		//第一次运行  初始化设置
		//RTC初始化
		RTC_Conf();
		RTC_WaitForLastTask();//等待最近一次对RTC寄存器的写操作完成
		RTC_SetCounter(Struct_To_UNIX(&systmtime)-TIME_ZOOM);//设置RTC计数器的值 11:20：50
		RTC_WaitForLastTask();
		BKP_WriteBackupRegister(BKP_DR1,0xA5A5);
	}
	else
	{	
		RTC_WaitForSynchro();//等待RTC寄存器同步	
		RTC_WaitForLastTask();//等待写RTC寄存器完成		
	  RTC_ITConfig(RTC_IT_SEC,ENABLE);//使能RTC秒中断    
		RTC_WaitForLastTask();//等待写RTC寄存器完成	
	}
	NVIC_EnableIRQ(RTC_IRQn);
	RCC_ClearFlag();//清除复位标志；		
}

/*
 * 函数名：Time_Display
 * 描述  ：显示当前时间值
 * 输入  ：-TimeVar RTC计数值，单位为 s
 * 输出  ：无
 * 调用  ：内部调用
 */	
void Time_Calculate(uint32_t TimeVar)
{
	   static uint32_t FirstCalculate = 1;
	   uint32_t BJ_TimeVar;	

	   /*  把标准时间转换为北京时间*/
	   BJ_TimeVar =TimeVar + TIME_ZOOM;

	   UNIX_To_Struct(BJ_TimeVar, &systmtime);/*把定时器的值转换为北京时间*/	
	
	  if((!systmtime.tm_hour && !systmtime.tm_min && !systmtime.tm_sec)  || (FirstCalculate))
	  {
	      
	      GetChinaCalendar((u16)systmtime.tm_year, (u8)systmtime.tm_mon, (u8)systmtime.tm_mday, Lunar1);	//公农历转换(只允许1901-2099年)
//					printf("\r\n 今天新历：%0.2d%0.2d,%0.2d,%0.2d", Lunar1[0], Lunar1[1], Lunar1[2],  Lunar1[3]);
	
	      GetChinaCalendarStr((u16)systmtime.tm_year,(u8)systmtime.tm_mon,(u8)systmtime.tm_mday,Lunar2);//输入公历日期得到农历字符串, 如:GetChinaCalendarStr(2007,02,06,str) 返回str="丙戌年腊月十九"
//					printf("\r\n 今天农历：%s\r\n", Lunar2);
	
	     if(GetJieQiStr((u16)systmtime.tm_year, (u8)systmtime.tm_mon, (u8)systmtime.tm_mday, Term24))//输入公历日期得到本月24节气日期
//					printf("\r\n 今天农历：%s\r\n", Term24);
	
	      FirstCalculate = 0;
	  }	 	  	

	  /* 输出时间戳，公历时间 */
//	  printf(" UNIX时间戳 = %d 当前时间为: %d年(%s年) %d月 %d日 (星期%s)  %0.2d:%0.2d:%0.2d\r",TimeVar,
//	                    tm->tm_year, zodiac_sign[(tm->tm_year-3)%12], tm->tm_mon, tm->tm_mday, 
//	                    WEEK_STR[tm->tm_wday], tm->tm_hour, 
//	                    tm->tm_min, tm->tm_sec);
		
}

