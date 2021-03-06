#ifndef __OLED_SSD1306_GT20L16_H
#define __OLED_SSD1306_GT20L16_H

#include "stm32f10x.h"
#include "gpio.h"
#include "stdlib.h"
#include "picture.h"
/*********************************************************
Copyright (C), 2016-2026, YFRobot.
www.yfrobot.com
File：OLED驱动代码
Author：pinggai    Version:1.0     Data:2016/09/08
Description:  Balance-STM32 V2.0 oled驱动代码,带字库芯片， 4线串口（SPI）
Others:使用端口：PB10(I2C2_SCL/U3_TX)、
                 PB11(I2C2_SDA/U3_RX)、
								 PB12(SPI2_NSS/I2C2_SMBAI/TIM1_BKIN)、
								 PB13(SPI2_SCK/TIM1_CH1N)、
								 PB14(SPI2_MISO/TIM1_CH2N)、
								 PB15(SPI2_MOSI/TIM1_CH1N)。
       使用硬件资源：PB10、PB11、PB12、PB13、PB14、PB15。
**********************************************************/
	    						  
//-----------------OLED端口定义----------------  					   
#define OLED_SG_CLK PBout(10)
#define OLED_SG_DIN PBout(11)
#define OLED_SG_DC  PBout(12)
#define OLED_SG_CS  PBout(13)

#define GT20_CLK PBout(10)
#define GT20_SI  PBout(11)
#define GT20_SO  PBin(14)
#define GT20_CS  PBout(15)		     

//OLED控制用函数   
void OLED_SG_WCMD(u8 cmd);	  //向SSD1306写命令
void OLED_SG_WDAT(u8 dat);	  //向SSD1306写数据
void OLED_SG_On(void);
void OLED_SG_Off(void);
void OLED_SG_Refresh_Gram(void);//将OLED_GRAM中的内容更新到SSD1306的Graphic Display Data RAM (GDDRAM)		   
void OLED_SG_Block_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot) ;	//填充一块区域
					
//void OLED_Init(void);	  //驱动端口初始化
void OLED_SG_Conf(void);  //OLED设置初始化
void OLED_SG_Clear(void);    //显存写0
void OLED_SG_Bright(void); //全屏点亮
void OLED_SG_DrawPoint(u8 x,u8 y,u8 t);

void GT20_WCMD(u8 cmd);  //向GT20L16写命令
static u8 GT20_RDAT(void);  //读GT20L16中字库的汉字或字符数据
void GT20_GetBytes(u8 addrH, u8 addrM, u8 addrL,u8 *fontBuf,u8 dataLen); //读取点阵字模
void OLED_GT20Photo( u8 x, u8 y, u8 len, u8 wide, u8 *pic, u8 mode);     //从任意点开始画指点大小的图片

void OLED_ShowGB2312String(u8 x,u8 y,u8 *p,u8 mode);     //在任意点显示GB2312字符串
void OLED_Show8X16String(u8 x,u8 y,u8 *text,u8 mode);    //在任意点显示8*16点ASCII字符串
void OLED_Show8X16BoldString(u8 x,u8 y,u8 *text,u8 mode);//在任意点显示8*16点ASCII粗体字符串
void OLED_ShowX16ArialString(u8 x,u8 y,u8 *text,u8 mode);//在任意点显示*16点ASCII方头（Arial）字符串
void OLED_ShowX16TimesNewRomanString(u8 x,u8 y,u8 *text,u8 mode);//在任意点显示*16点ASCII白正（TimesNewRoman）字符串
void OLED_Show7X8String(u8 x,u8 y,u8 *text,u8 mode);	   //在任意点显示7*8点ASCII字符串
void OLED_Show5X7String(u8 x,u8 y,u8 *text,u8 mode);	   //在任意点显示5*7点ASCII字符串
void OLED_ShowNumString(u8 x,u8 y,u32 num,u8 len,u8 mode);	//显示数字0~4294967295
void OLED_ShowFloatNum(u8 x,u8 y,float num,u8 lenInt,u8 lenFlo,u8 mode);	 //显示浮点数
void OLED_Picture( u8 x, u8 y, u8 len, u8 wide, const u8 *pic, u8 mode);	 //从任意点开始画指点大小的图片
void OLED_Show8X16Num(u8 x,u8 y,u8 num,u8 mode);		//显示单个数字




/*
在.c文件中定义
//自定义字符或图片

//YF 99*105
 unsigned char YFpicture[]={
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,
0xE0,0xF0,0x38,0x38,0xEC,0xC4,0x82,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x80,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE,0xFF,0x3F,0x1F,0x07,0x03,0x01,0x00,0x00,0x00,
0x01,0x3F,0xFF,0xFF,0xFE,0xFE,0xF8,0xE0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xE0,0xF8,0xFC,0xFE,0xFF,0xFF,0xFF,0x3F,
0x1F,0x07,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x7F,
0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0xE0,0xF8,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x0F,0x01,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,
0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0xFF,0xFF,0xFF,0xFF,
0xFF,0xFF,0xFF,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,
0xFE,0xFC,0x80,0x00,0x00,0x00,0x00,0x00,0xC0,0xFF,0xDF,0xC1,0xE0,0x70,0x30,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x03,0xC0,
0xC0,0xC0,0xC0,0xC0,0xE0,0xE0,0xE0,0xE0,0x70,0x9C,0x9F,0x8F,0xCF,0x7F,0x7D,0x3C,
0x3E,0x1E,0x1F,0x0F,0x07,0x07,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x80,0x87,0x9F,0xBF,0xFF,0xFF,0xFF,0x80,0x80,0x80,0xE0,0xE1,0xF1,
0xF9,0xDD,0xDF,0xDF,0xCF,0xC7,0xC7,0xC3,0xC0,0xC0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,
0xE0,0xE0,0xE0,0xE0,0xF0,0xF0,0xF0,0x70,0x70,0x70,0x78,0x38,0x38,0x18,0x18,0x18,
0x18,0x0C,0x0C,0x0C,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x03,0x03,0x03,0x03,0x03,
0x07,0x07,0x07,0x0F,0x0F,0x1F,0x3F,0x7F,0x7F,0x7F,0x7F,0x3F,0x3F,0x3F,0x1F,0x1F,
0x1F,0x1F,0x0F,0x0F,0x0F,0x0F,0x07,0x07,0x07,0x03,0x03,0x03,0x03,0x01,0x01,0x01,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};


*/








#endif


