#include "oled_SSD1306_GT20L16.h"
#include "sysTick.h"
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

/******************************************************
Function:	 void OLED_Init(void)
Description: 驱动端口初始化、OLED与GT20驱动
             B10 (输出) -- CLK	  时钟
			       B11 (输出) -- DIN	  数据	SSD1306输入/GT20L16输入
			       B12 (输出) -- D/S	  命令/数据
			       B13 (输出) -- CS1	  SSD1306片选
			       B14 (输入) -- SO	    数据输入，GT20L16输出
			       B15 (输出) -- CS2	  GT20L16片选
Calls by:	 void OLED_SetInit(void) 
******************************************************/	
/*
int main(void)
{
	LED_SG_Conf();
	OLED_Conf();			 //初始化OLED     
  OLED_SG_Clear();        //显存写0
	
	LED_ALL_ON;
	
	OLED_SG_Clear();
	OLED_Picture( 25, 0, 60, 64, YFpicture, 1);
 	OLED_Show5X7String(72,57,(uint8_t*)"YFROBOT",1);
	OLED_SG_Refresh_Gram();
	SysTick_Delay_Ms(1500);
	OLED_SG_Clear();
	OLED_ShowGB2312String(0,20,(uint8_t*)" 0.96'OLED测试  ",0);
	OLED_SG_Refresh_Gram();
	SysTick_Delay_Ms(1500);
	LED_ALL_OFF;
	while(1)
	{
		OLED_SG_Bright(); //全屏点亮
		SysTick_Delay_Ms(1500);
		LED_ALL_ON;
		OLED_SG_Clear();
		OLED_ShowGB2312String(0,0,(uint8_t*)"故人西辞黄鹤楼，",1);
		OLED_ShowGB2312String(0,16,(uint8_t*)"烟花三月下扬州。",0);
		OLED_ShowGB2312String(0,32,(uint8_t*)"孤帆远影碧空尽，",1);
		OLED_ShowGB2312String(0,48,(uint8_t*)"唯见长江天际流。",0);
		OLED_SG_Refresh_Gram();
		SysTick_Delay_Ms(1500);
		LED_ALL_OFF;
		OLED_SG_Clear();
		OLED_ShowGB2312String(0,0,(uint8_t*)"ＹＦＲＯＢＯＴ",1);//GB2312简体中文中字母，注意它的格式和ASCII
		OLED_ShowGB2312String(0,16,(uint8_t*)"YFROBOT",1);     //在任意点显示8*16点ASCII字符串
		OLED_Show8X16BoldString(0,32,(uint8_t*)"YFROBOT",1);   //在任意点显示8*16点ASCII粗体字符串
		OLED_Show7X8String(0,48,(uint8_t*)"YFROBOT",1);	       //在任意点显示7*8点ASCII字符串
		OLED_Show5X7String(0,57,(uint8_t*)"YFROBOT",1);	       //在任意点显示5*7点ASCII字符串
		OLED_SG_Refresh_Gram();
		SysTick_Delay_Ms(1500);
    LED_ALL_ON;
		OLED_SG_Clear();
		OLED_ShowX16ArialString( 0, 0, (uint8_t*)"ABCDEFGHIJ", 1);//在任意点显示*16点ASCII方头（Arial）字符串
		OLED_ShowX16TimesNewRomanString(0,16,(uint8_t*)"ABCDEFGHIJ",1);//在任意点显示*16点ASCII白正（TimesNewRoman）字符串
		OLED_ShowNumString(0,32,123456789,9,1);
		OLED_ShowFloatNum(0,48, -9876.54321,5,2,1);	 //显示浮点数
		OLED_SG_Refresh_Gram();
		SysTick_Delay_Ms(1500);
		LED_ALL_OFF;
	}	 
}

*/
static void OLED_GPIO_Conf(void)
{ 
	RCC->APB2ENR|=1<<3;    //使能PORTB时钟 
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
 	GPIOB->CRH&=0X000000FF;	  //PB.11~15 
 	GPIOB->CRH|=0X38333300;	  //PB14 输入
	GPIOB->ODR|=0X2F<<11;	   //高电平
	GPIOB->ODR&=~(1<<14);    //
}

//OLED的显存
//存放格式如下.
//PAGE0 [0]0 1 2 3 ... 127		  位高低顺序  D0
//PAGE1 [1]0 1 2 3 ... 127		              D1
//PAGE2 [2]0 1 2 3 ... 127		              D2
//PAGE3 [3]0 1 2 3 ... 127		              D3
//PAGE4 [4]0 1 2 3 ... 127		              D4
//PAGE5 [5]0 1 2 3 ... 127		              D5
//PAGE6 [6]0 1 2 3 ... 127		              D6
//PAGE7 [7]0 1 2 3 ... 127 		              D7 
u8 OLED_SG_GRAM[128][8];	 

/******************************************************
Function:	 void OLED_SG_Refresh_Gram(void)
Description:  更新显存到OLED，将OLED_SG_GRAM中的内容更新到
              SSD1306的Graphic Display Data RAM (GDDRAM)
              每次显示新内容时都需要调用此函数。
******************************************************/		 
void OLED_SG_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_SG_WCMD(0xB0+i);	 //设置页地址（0~7）每页中的GDDRAM列地址是自动增加的。
		OLED_SG_WCMD(0x00);	 //设置显示位置—列低地址
		OLED_SG_WCMD(0x10);	 //设置显示位置—列高地址
		for(n=0;n<128;n++) 
			OLED_SG_WDAT(OLED_SG_GRAM[n][i]); //#define OLED_DATA 1	//写数据
	}   
}
/******************************************************
Function:	 void OLED_SG_WCMD(u8 cmd)
Description: 向SSD1306 写命令
Input:		 cmd：要写入的命令
Others:		 OLED_SG_DC=0：表示写命令；=1写数据
******************************************************/	
void OLED_SG_WCMD(u8 cmd)
{	
	u8 i;			  
	OLED_SG_DC=0; //写命令 
	OLED_SG_CS=0;		  
	for(i=0;i<8;i++)
	{			  
		OLED_SG_CLK=0;
		if(cmd&0x80)OLED_SG_DIN=1;
		else OLED_SG_DIN=0;
		OLED_SG_CLK=1;
		cmd<<=1;   
	}				 
	OLED_SG_CS=1;		  
	OLED_SG_DC=1;   	  
}
/******************************************************
Function:	 void OLED_SG_WDAT(u8 dat)
Description: 向SSD1306 写数据
Input:		 dat：要写入的数据
Others:		 OLED_SG_DC=0：表示写命令；=1写数据
******************************************************/
void OLED_SG_WDAT(u8 dat)
{	
	u8 i;			  
	OLED_SG_DC=1; //写数据 
	OLED_SG_CS=0;		  
	for(i=0;i<8;i++)
	{			  
		OLED_SG_CLK=0;
		if(dat&0x80)OLED_SG_DIN=1;
		else OLED_SG_DIN=0;
		OLED_SG_CLK=1;
		dat<<=1;   
	}				 
	OLED_SG_CS=1;		  
	OLED_SG_DC=1;   	  
}
//开启OLED显示
void OLED_SG_On(void)
{
	OLED_SG_WCMD(0x8D);   //SET DCDC命令
	OLED_SG_WCMD(0x14);   //DCDC ON
	OLED_SG_WCMD(0xAF);   //DISPLAY ON
}
//关闭OLED显示     
void OLED_SG_Off(void)
{
	OLED_SG_WCMD(0x8D);	 //SET DCDC命令
	OLED_SG_WCMD(0x10);	 //DCDC OFF
	OLED_SG_WCMD(0xAE);	 //DISPLAY OFF
}
//OLED_SG_GRAM[128][8]写0
void OLED_SG_Clear(void)  
{  
	u8 i,n;  
	for(i=0;i<8;i++)
	    for(n=0;n<128;n++)
	        OLED_SG_GRAM[n][i]=0X00;  
//	OLED_SG_Refresh_Gram();//更新显示
}
//全屏点亮
void OLED_SG_Bright(void)  
{  
	u8 i,n;  
	for(i=0;i<8;i++)
	    for(n=0;n<128;n++)
	        OLED_SG_GRAM[n][i]=0XFF;  
	OLED_SG_Refresh_Gram();//更新显示
}
/******************************************************
Function:	 void OLED_SG_DrawPoint(u8 x,u8 y,u8 t)
Description: 在屏幕上任一位置画点，填充或清空
Input:	  x：0~127
		  y:0~63
		  t:1 填充 0,清空
******************************************************/
void OLED_SG_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//超出范围了.
	pos=y/8;
	bx=y%8;
	temp=1<<bx;
	if(t)OLED_SG_GRAM[x][pos]|=temp;
	else OLED_SG_GRAM[x][pos]&=~temp;	    
}
//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,清空;1,填充	  
void OLED_SG_Block_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_SG_DrawPoint(x,y,dot);
	}													    
	OLED_SG_Refresh_Gram();//更新显示
}

/******************************************************
Function:	 void OLED_Init(void)
Description: 驱动端口初始化、SSD1306初始化。
Calls:		 void OLED_Init(void);	  //驱动端口初始化
			 void OLED_SG_WCMD(u8 cmd)； //向SSD1306写命令
******************************************************/				    
void OLED_SG_Conf(void)
{ 	 				 	 					    
	OLED_GPIO_Conf();		//端口初始化				  

	OLED_SG_WCMD(0xAE);    //关闭显示
	OLED_SG_WCMD(0xD5);    //设置时钟分频因子,震荡频率
	OLED_SG_WCMD(0x50);	    //[3:0],分频因子;[7:4],震荡频率
	OLED_SG_WCMD(0xA8);    //设置驱动路数
	OLED_SG_WCMD(0x3F);        //默认0X3F(1/64)
	
	OLED_SG_WCMD(0xD3);    //设置显示偏移
	OLED_SG_WCMD(0x00);       //默认为0
	OLED_SG_WCMD(0x40);    //设置显示开始行 [5:0],行数.
	OLED_SG_WCMD(0x8D);    //电荷泵设置
	OLED_SG_WCMD(0x14);    //bit2，开启/关闭
	
	OLED_SG_WCMD(0x20);	//设置内存寻址模式
	OLED_SG_WCMD(0x02);	//[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;	
	OLED_SG_WCMD(0xA1);	//段重定义设置,bit0: 0,column 0->SEG 0;  1,column 0->SEG 127;
	OLED_SG_WCMD(0xC8);	//设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
	OLED_SG_WCMD(0xDA);	//设置COM硬件引脚配置  [5:4]配置  配置COM（SSD1306）与ROW（液晶）的连接关系
	OLED_SG_WCMD(0x12);        //[5:4]配置  配置COM（SSD1306）与ROW（液晶）的连接关系
	
	OLED_SG_WCMD(0x81);	//对比度设置
	OLED_SG_WCMD(0xEF);	//1~255;默认0X7F (亮度设置,越大越亮)
	OLED_SG_WCMD(0xD9);	//设置预充电周期
	OLED_SG_WCMD(0xF1);	//[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_SG_WCMD(0xDB);	//设置VCOMH 电压倍率
	OLED_SG_WCMD(0x30);	//[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
	OLED_SG_WCMD(0xA4);	//全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	OLED_SG_WCMD(0xA6);	//设置显示方式;bit0:1,反相显示;0,正常显示
	OLED_SG_WCMD(0xAF);	//开启显示

	OLED_SG_Clear();		//清屏
}



/******************************************************
Function:	  void GT20_WCMD(u8 cmd)
Description:  向GT20L16写命令  
Input:		  u8 cmd 命令符
Others:		  时钟上升沿，数据输入GT20
******************************************************/
void GT20_WCMD(u8 cmd)
{
	u8 i;			  		  
	for(i=0;i<8;i++)
	{	
		GT20_CLK=0;	  		
		if(cmd&0x80) GT20_SI=1;
		else GT20_SI=0;		
		GT20_CLK=1;
		cmd<<=1; 
	}				 
}

/******************************************************
Function:	  static u8 GT20_RDAT( )
Description:  读GT20L16中字库的汉字或字符数据
Output:		  data 一个字节，用于显示汉字或字符
Others:		  时钟下降沿，读GT20数据
******************************************************/
static u8 GT20_RDAT(void)
{
	u16 i;
	u8 data=0x00;
	GT20_CLK = 1;	
	for(i=0;i<8;i++)
	{
		GT20_CLK = 1; 
		data = data<<1;
		GT20_CLK = 0;					
		if(GT20_SO)
			data = data+1;			
	}
	return data;
}
/******************************************************
Function:	 void GT20_GetBytes(u8 addrH, u8 addrM, u8 addrL,u8 *fontBuf,u8 dataLen)
Description: 读取点阵字模
Calls:		 void GT20_WCMD(u8 cmd)		//向GT20L16写命令 
         	 static u8 GT20_RDAT(void)	//读GT20L16中字库的汉字或字符数据
Input:	 u8 addrH  高地址
		 u8 addrM  中地址
		 u8 addrL  低地址
		 u8 *fontBuf  数组存储指针
		 u8 dataLen	  汉字或字符 点阵字模字节个数
Others:	汉字和字符存储的字节不同，根据实际情况进行修改
******************************************************/	
void GT20_GetBytes(u8 addrH, u8 addrM, u8 addrL,u8 *fontBuf,u8 dataLen)
{
	u8 i;
	GT20_CS = 1;
	OLED_SG_CS = 1;
	GT20_CS = 0;   //片选 低电平为有效电平		
	GT20_CLK = 0;
	GT20_WCMD(0x03);  //通讯起始命令字（03H）
	GT20_WCMD(addrH);
	GT20_WCMD(addrM);
	GT20_WCMD(addrL);
	for(i=0;i<dataLen; i++)
	{
		*(fontBuf+i) = GT20_RDAT( );
	}
	SysTick_Delay_Us(2);	
	GT20_CS = 1;	 
}

/******************************************************
Function:	void OLED_GT20Photo( u8 x, u8 y, u8 len, u8 wide, u8 *pic, u8 mode)
Description:从任意点开始画指点大小的图片
Calls： void OLED_SG_DrawPoint(u8 x,u8 y,u8 t) //任意坐标画点
Input:	u8 x，u8 y：起始点。
        u8 len：长，x轴方向
		u8 wide：宽，y轴方向 
		u8 *pic：图片数组
		u8 mode：字符显示区域为0或1
******************************************************/
void OLED_GT20Photo( u8 x, u8 y, u8 len, u8 wide, u8 *pic, u8 mode)
{
	u8 x_pos,y_pos,temp;
	u8 i,j;
	x_pos = x;
	y_pos = y;
	for(j=0; j<len; j++)
	{
		for(i=0;i<wide;i++)
		{
		    if(i%8==0)
			{
				temp = *(pic+(i/8)*len+j);
			}
			if(temp&0x01)	OLED_SG_DrawPoint( x_pos+j,y_pos+i,mode);
			else	OLED_SG_DrawPoint( x_pos+j,y_pos+i,!mode);		  //某“点”写0，或写1
			temp >>=1;			
		}
	}
}
u32	 FONTADDR=0; //GT20中的存储地址
/******************************************************
Function:	void OLED_ShowGB2312String(u8 x,u8 y,u8 *p,u8 mode)
Description: 在任意点显示GB2312字符串
Calls： void GT20_GetBytes(u8 addrH, u8 addrM, u8 addrL,u8 *fontBuf,u8 dataLen); //读取点阵字模
        void OLED_GT20Photo( u8 x, u8 y, u8 len, u8 wide, u8 *pic, u8 mode);     //从任意点开始画指点大小的图片
Input:	u8 x，u8 y：起始点。 
		u8 *text：图片数组
		u8 mode：字符显示区域为0或1
******************************************************/
void OLED_ShowGB2312String(u8 x,u8 y,u8 *text,u8 mode)
{
	u8 i = 0;
//	u8 K; 
	u8 addrH, addrM, addrL;
	u8 FONTBUF[32];
	while(text[i]>0x00)
	{
//		printf("%X,%X\r\n",text[i],text[i+1]);	//串口输出字符内码
		//A9区
		if(((text[i]==0xA9))&&(text[i+1]>=0xA1))
		{						
			/*国标简体（GB2312）汉字在GT20字库IC中的地址由以下公式来计算：*/
			/*Address = (282 + (LSB - 0xA1 ))*32+BaseAdd;BaseAdd=0*/
			/*由于担心8位单片机有乘法溢出问题，所以分三部取地址*/
			FONTADDR = (text[i+1]-0xA1)+282;
			FONTADDR = FONTADDR*32;			
			
			addrH = (FONTADDR&0xff0000)>>16;  //地址的高8位,共24位
			addrM = (FONTADDR&0xff00)>>8;     //地址的中8位,共24位
			addrL = FONTADDR&0xff;	         //地址的低8位,共24位
			GT20_GetBytes(addrH,addrM,addrL,FONTBUF,32 );//取32个字节的数据，存到"FONTBUF[32]"
//			printf("%X,%X\r\n",text[i],text[i+1]);	//串口输出字符内码
//			for(K=0;K<32;K++)				        //串口输出字符点阵信息数组
//				printf("%x，",FONTBUF[K]);
			OLED_GT20Photo( x, y, 16, 16, FONTBUF, mode);	 //更新GRAM, 显示为填充
			i+=2;
			x+=16;
		}
		//A1~A3区
		else if(((text[i]>=0xA1) &&(text[i]<=0xA3))&&(text[i+1]>=0xA1))
		{						
			/*国标简体（GB2312）汉字在GT20字库IC中的地址由以下公式来计算：*/
			/*Address =( (MSB - 0xA1) * 94 + (LSB - 0xA1))*32+ BaseAdd;BaseAdd=0*/
			/*由于担心8位单片机有乘法溢出问题，所以分三部取地址*/
			FONTADDR = (text[i]- 0xA1)*94; 
			FONTADDR += (text[i+1]-0xA1);
			FONTADDR = FONTADDR*32;			
			
			addrH = (FONTADDR&0xff0000)>>16;  //地址的高8位,共24位
			addrM = (FONTADDR&0xff00)>>8;     //地址的中8位,共24位
			addrL = FONTADDR&0xff;	         //地址的低8位,共24位
			GT20_GetBytes(addrH,addrM,addrL,FONTBUF,32 );//取32个字节的数据，存到"FONTBUF[32]"
			OLED_GT20Photo( x, y, 16, 16, FONTBUF, mode);	 //更新GRAM, 显示为填充
			i+=2;
			x+=16;
		}
		//B0~F7区
		else if(((text[i]>=0xB0) &&(text[i]<=0xF7))&&(text[i+1]>=0xA1))
		{						
			/*国标简体（GB2312）汉字在GT20字库IC中的地址由以下公式来计算：*/
			/*Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0*/
			/*由于担心8位单片机有乘法溢出问题，所以分三部取地址*/
			FONTADDR = (text[i]- 0xB0)*94; 
			FONTADDR += (text[i+1]-0xA1)+846;
			FONTADDR = FONTADDR*32;			
			
			addrH = (FONTADDR&0xff0000)>>16;  //地址的高8位,共24位
			addrM = (FONTADDR&0xff00)>>8;     //地址的中8位,共24位
			addrL = FONTADDR&0xff;	         //地址的低8位,共24位
			GT20_GetBytes(addrH,addrM,addrL,FONTBUF,32 );//取32个字节的数据，存到"FONTBUF[32]"
			OLED_GT20Photo( x, y, 16, 16, FONTBUF, mode);	 //更新GRAM, 显示为填充
			i+=2;
			x+=16;
		}
		//8*16点ASSII字符
		else if((text[i]>=0x20)&&(text[i]<=0x7E))
		{						
			/*8*16点ASCII在GT20字库IC中的地址由以下公式来计算：*/
			/*Address = (ASCIICode –0x20 ) * 16+BaseAdd;BaseAdd=0x3B7C0*/
			/*由于担心8位单片机有乘法溢出问题，所以分三部取地址*/
			FONTADDR = (text[i]- 0x20)*16+0x3B7C0; 		
			
			addrH = (FONTADDR&0xff0000)>>16;  //地址的高8位,共24位
			addrM = (FONTADDR&0xff00)>>8;     //地址的中8位,共24位
			addrL = FONTADDR&0xff;	         //地址的低8位,共24位
			GT20_GetBytes(addrH,addrM,addrL,FONTBUF,16 );//取32个字节的数据，存到"FONTBUF[32]"
			OLED_GT20Photo( x, y, 8, 16, FONTBUF, mode);	 //更新GRAM, 显示为填充
			i++;
			x+=8;
		}
		else
		i++;
	}			
}
/******************************************************
Function:	 void OLED_Show8*16String(u8 x,u8 y,u8 *p,u8 mode)
Description: 在任意点显示8*16点ASCII字符串
Calls： void GT20_GetBytes(u8 addrH, u8 addrM, u8 addrL,u8 *fontBuf,u8 dataLen); //读取点阵字模
        void OLED_GT20Photo( u8 x, u8 y, u8 len, u8 wide, u8 *pic, u8 mode);     //从任意点开始画指点大小的图片
Input:	u8 x，u8 y：起始点。 
		u8 *text：字符串
		u8 mode：字符显示区域显示模式，为0或1
******************************************************/
void OLED_Show8X16String(u8 x,u8 y,u8 *text,u8 mode)
{
	u8 i = 0;
	u8 addrH, addrM, addrL;
	u8 FONTBUF[16];
	while(text[i]>0x00)
	{
		if((text[i]>=0x20)&&(text[i]<=0x7E))
		{						
			/*8*16点ASCII粗体在字库IC中的地址由以下公式来计算：*/
			/*Address = (ASCIICode –0x20 ) * 16+BaseAdd;BaseAdd=0x3B7C0*/
			/*由于担心8位单片机有乘法溢出问题，所以分三部取地址*/
			FONTADDR = (text[i]- 0x20)*16+0x3B7C0; 		
			
			addrH = (FONTADDR&0xff0000)>>16;  //地址的高8位,共24位
			addrM = (FONTADDR&0xff00)>>8;     //地址的中8位,共24位
			addrL = FONTADDR&0xff;	         //地址的低8位,共24位
			GT20_GetBytes(addrH,addrM,addrL,FONTBUF,16 );//取16个字节的数据，存到"FONTBUF[16]"
			OLED_GT20Photo( x, y, 8, 16, FONTBUF, mode);	 //更新GRAM, 显示为填充
			i++;
			x+=8;
		}
		else 
			i++;
	}
}
/******************************************************
Function:	 void OLED_Show8*16BoldString(u8 x,u8 y,u8 *p,u8 mode)
Description: 在任意点显示8*16点ASCII粗体字符串
Calls： void GT20_GetBytes(u8 addrH, u8 addrM, u8 addrL,u8 *fontBuf,u8 dataLen); //读取点阵字模
        void OLED_GT20Photo( u8 x, u8 y, u8 len, u8 wide, u8 *pic, u8 mode);     //从任意点开始画指点大小的图片
Input:	u8 x，u8 y：起始点。 
		u8 *text：图片数组
		u8 mode：字符显示区域为0或1
******************************************************/
void OLED_Show8X16BoldString(u8 x,u8 y,u8 *text,u8 mode)
{
	u8 i = 0;
	u8 addrH, addrM, addrL;
	u8 FONTBUF[16];
	while(text[i]>0x00)
	{
		if((text[i]>=0x20)&&(text[i]<=0x7E))
		{						
			/*8*16点ASCII粗体在字库IC中的地址由以下公式来计算：*/
			/*Address = (ASCIICode –0x20 ) * 16+BaseAdd;BaseAdd=0x3CF80*/
			/*由于担心8位单片机有乘法溢出问题，所以分三部取地址*/
			FONTADDR = (text[i]- 0x20)*16+0x3CF80; 		
			
			addrH = (FONTADDR&0xff0000)>>16;  //地址的高8位,共24位
			addrM = (FONTADDR&0xff00)>>8;     //地址的中8位,共24位
			addrL = FONTADDR&0xff;	         //地址的低8位,共24位
			GT20_GetBytes(addrH,addrM,addrL,FONTBUF,16 );//取16个字节的数据，存到"FONTBUF[16]"
			OLED_GT20Photo( x, y, 8, 16, FONTBUF, mode);	 //更新GRAM, 显示为填充
			i++;
			x+=8;
		}
		else 
			i++;
	}
}
/******************************************************
Function:	 OLED_ShowX16ArialString(u8 x,u8 y,u8 *text,u8 mode)
Description: 在任意点显示*16点ASCII方头（Arial）字符串
Calls： void GT20_GetBytes(u8 addrH, u8 addrM, u8 addrL,u8 *fontBuf,u8 dataLen); //读取点阵字模
        void OLED_GT20Photo( u8 x, u8 y, u8 len, u8 wide, u8 *pic, u8 mode);     //从任意点开始画指点大小的图片
Input:	u8 x，u8 y：起始点。 
		u8 *text：字符串
		u8 mode：字符显示区域显示模式，为0或1
******************************************************/
void OLED_ShowX16ArialString(u8 x,u8 y,u8 *text,u8 mode)
{
	u8 i = 0;
	u8 bitWide = 0;      //字符宽度
	u8 addrH, addrM, addrL;
	u8 FONTBUF[34];
	while(text[i]>0x00)
	{
		if((text[i]>=0x20)&&(text[i]<=0x7E))
		{						
			/*8*16点ASCII粗体在字库IC中的地址由以下公式来计算：*/
			/*Address = (ASCIICode –0x20 ) * 16+BaseAdd;BaseAdd=0x3C2C0*/
			/*由于担心8位单片机有乘法溢出问题，所以分三部取地址*/
			FONTADDR = (text[i]- 0x20)*34+0x3C2C0; 		
			
			addrH = (FONTADDR&0xff0000)>>16;  //地址的高8位,共24位
			addrM = (FONTADDR&0xff00)>>8;     //地址的中8位,共24位
			addrL = FONTADDR&0xff;	         //地址的低8位,共24位
			GT20_GetBytes(addrH,addrM,addrL,FONTBUF,34 );//取34个字节的数据，存到"FONTBUF[34]"
			bitWide = FONTBUF[1];
			OLED_GT20Photo( x, y, 16, 16, FONTBUF+2, mode);	 //更新GRAM, 显示为填充
			i++;
			x+=bitWide;
		}
		else 
			i++;
	}
}
/******************************************************
Function:	 OLED_ShowX16TimesNewRomanString(u8 x,u8 y,u8 *text,u8 mode)
Description: 在任意点显示*16点ASCII白正（TimesNewRoman）字符串
Calls： void GT20_GetBytes(u8 addrH, u8 addrM, u8 addrL,u8 *fontBuf,u8 dataLen); //读取点阵字模
        void OLED_GT20Photo( u8 x, u8 y, u8 len, u8 wide, u8 *pic, u8 mode);     //从任意点开始画指点大小的图片
Input:	u8 x，u8 y：起始点。 
		u8 *text：字符串
		u8 mode：字符显示区域显示模式，为0或1
******************************************************/
void OLED_ShowX16TimesNewRomanString(u8 x,u8 y,u8 *text,u8 mode)
{
	u8 i = 0;
	u8 bitWide = 0;      //字符宽度
	u8 addrH, addrM, addrL;
	u8 FONTBUF[34];
	while(text[i]>0x00)
	{
		if((text[i]>=0x20)&&(text[i]<=0x7E))
		{						
			/*8*16点ASCII粗体在字库IC中的地址由以下公式来计算：*/
			/*Address = (ASCIICode –0x20 ) * 34+BaseAdd;BaseAdd=0x3D580*/
			/*由于担心8位单片机有乘法溢出问题，所以分三部取地址*/
			FONTADDR = (text[i]- 0x20)*34+0x3D580; 		
			
			addrH = (FONTADDR&0xff0000)>>16;  //地址的高8位,共24位
			addrM = (FONTADDR&0xff00)>>8;     //地址的中8位,共24位
			addrL = FONTADDR&0xff;	         //地址的低8位,共24位
			GT20_GetBytes(addrH,addrM,addrL,FONTBUF,34 );//取34个字节的数据，存到"FONTBUF[34]"
			bitWide = FONTBUF[1];
			OLED_GT20Photo( x, y, 16, 16, FONTBUF+2, mode);	 //更新GRAM, 显示为填充
			i++;
			x += bitWide  ;
		}
		else 
			i++;
	}
}
/******************************************************
Function:	 void OLED_Show7*8String(u8 x,u8 y,u8 *p,u8 mode)
Description: 在任意点显示7*8点ASCII字符串
Calls： void GT20_GetBytes(u8 addrH, u8 addrM, u8 addrL,u8 *fontBuf,u8 dataLen); //读取点阵字模
        void OLED_GT20Photo( u8 x, u8 y, u8 len, u8 wide, u8 *pic, u8 mode);     //从任意点开始画指点大小的图片
Input:	u8 x，u8 y：起始点。 
		u8 *text：图片数组
		u8 mode：字符显示区域为0或1
******************************************************/
void OLED_Show7X8String(u8 x,u8 y,u8 *text,u8 mode)
{
	u8 i = 0;
	u8 addrH, addrM, addrL;
	u8 FONTBUF[7];
	while(text[i]>0x00)
	{
		if((text[i]>=0x20)&&(text[i]<=0x7E))
		{						
			/*7*8点ASCII在字库IC中的地址由以下公式来计算：*/
			/*Address = (ASCIICode –0x20 ) * 8+BaseAdd;  BaseAdd=0x66C0*/
			/*由于担心8位单片机有乘法溢出问题，所以分三部取地址*/
			FONTADDR = (text[i]- 0x20)*8+0x66C0; 		
			
			addrH = (FONTADDR&0xff0000)>>16;  //地址的高8位,共24位
			addrM = (FONTADDR&0xff00)>>8;     //地址的中8位,共24位
			addrL = FONTADDR&0xff;	         //地址的低8位,共24位
			GT20_GetBytes(addrH,addrM,addrL,FONTBUF,7);//取7个字节的数据，存到"FONTBUF[7]"
			OLED_GT20Photo( x, y, 7, 8, FONTBUF, mode);	 //更新GRAM, 显示为填充
			i++;
			x+=7;
		}
		else 
			i++;
	}
}

/******************************************************
Function:	 void OLED_Show5*7String(u8 x,u8 y,u8 *p,u8 mode)
Description: 在任意点显示5*7点ASCII字符串
Calls： void GT20_GetBytes(u8 addrH, u8 addrM, u8 addrL,u8 *fontBuf,u8 dataLen); //读取点阵字模
        void OLED_GT20Photo( u8 x, u8 y, u8 len, u8 wide, u8 *pic, u8 mode);     //从任意点开始画指点大小的图片
Input:	u8 x，u8 y：起始点。 
		u8 *text：图片数组
		u8 mode：字符显示区域为0或1
******************************************************/
void OLED_Show5X7String(u8 x,u8 y,u8 *text,u8 mode)
{
	u8 i = 0;
	u8 addrH, addrM, addrL;
	u8 FONTBUF[5];
	while(text[i]>0x00)
	{
		if((text[i]>=0x20)&&(text[i]<=0x7E))
		{						
			/*5*7点ASCII在字库IC中的地址由以下公式来计算：*/
			/*Address = (ASCIICode –0x20 ) * 8+BaseAdd;  BaseAdd=0x3BFC0*/
			/*由于担心8位单片机有乘法溢出问题，所以分三部取地址*/
			FONTADDR = (text[i]- 0x20)*8+0x3BFC0; 		
			
			addrH = (FONTADDR&0xff0000)>>16;  //地址的高8位,共24位
			addrM = (FONTADDR&0xff00)>>8;     //地址的中8位,共24位
			addrL = FONTADDR&0xff;	         //地址的低8位,共24位
			GT20_GetBytes(addrH,addrM,addrL,FONTBUF,5);//取5个字节的数据，存到"FONTBUF[32]"
			OLED_GT20Photo( x, y, 5, 7, FONTBUF, mode);	 //更新GRAM, 显示为填充
			i++;
			x+=5;
		}
		else 
			i++;
	}
}

/******************************************************
Function:	void OLED_Photo( u8 x, u8 y, u8 len, u8 wide, const u8 *pic, u8 mode)
Description:从任意点开始画指点大小的图片
Calls： void OLED_SG_DrawPoint(u8 x,u8 y,u8 t) //任意坐标画点
Input:	u8 x，u8 y：起始点。
        u8 len：长，x轴方向
		u8 wide：宽，y轴方向 
		u8 *pic：图片数组
		u8 mode：字符显示区域为0或1
******************************************************/
void OLED_Picture( u8 x, u8 y, u8 len, u8 wide, const u8 *pic, u8 mode)
{
	u8 x_pos,y_pos,temp;
	u8 i,j;
	x_pos = x;
	y_pos = y;
	for(j=0; j<len; j++)
	{
		for(i=0;i<wide;i++)
		{
		    if(i%8==0)
			{
				temp = *(pic+(i/8)*len+j);
			}
			if(temp&0x01)	OLED_SG_DrawPoint( x_pos+j,y_pos+i,mode);
			else	OLED_SG_DrawPoint( x_pos+j,y_pos+i,!mode);		  //某“点”写0，或写1
			temp >>=1;			
		}
	}
}

//m^n函数
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}
//显示单个数字
/******************************************************
Function:	 void OLED_Show8X16Num(u8 x,u8 y,u8 num,u8 mode)
Description: 在任意位置显示单个数字
Calls：   void GT20_GetBytes(u8 addrH, u8 addrM, u8 addrL,u8 *fontBuf,u8 dataLen); //读取点阵字模
          void OLED_GT20Photo( u8 x, u8 y, u8 len, u8 wide, u8 *pic, u8 mode);     //从任意点开始画指点大小的图片
Calls by：void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 mode);  //在任意位置显示数字
Input:	u8 x，u8 y：起始点。 
		u8 num：数字 0、1、2、3、4、5、6、7、8、9
		u8 mode：字符显示区域为0或1
******************************************************/
void OLED_Show8X16Num(u8 x,u8 y,u8 num,u8 mode)
{
	u8 addrH, addrM, addrL,temp;
	u8 FONTBUF[16];
	temp = num+'0';//	
	if((temp>=0x20)&&(temp<=0x7E))
	{						
		/*8*16点ASCII在GT20字库IC中的地址由以下公式来计算：*/
		/*Address = (ASCIICode –0x20 ) * 16+BaseAdd;BaseAdd=0x3B7C0*/
		/*由于担心8位单片机有乘法溢出问题，所以分三部取地址*/
		FONTADDR = (temp - 0x20)*16+0x3B7C0; 					
		addrH = (FONTADDR&0xff0000)>>16;  //地址的高8位,共24位
		addrM = (FONTADDR&0xff00)>>8;     //地址的中8位,共24位
		addrL = FONTADDR&0xff;	          //地址的低8位,共24位
		GT20_GetBytes(addrH,addrM,addrL,FONTBUF,16 );//取16个字节的数据，存到"FONTBUF[32]"
		OLED_GT20Photo( x, y, 8, 16, FONTBUF, mode);	 //更新GRAM, 显示为填充	
   } 
}
/******************************************************
Function:	  void OLED_ShowGB2312Num(u8 x,u8 y,u32 num,u8 len,u8 size)
Description:  在任意位置显示数字
Calls:		  void OLED_ShowGB2312String(u8 x,u8 y,u8 *p,u8 mode);     //在任意点显示GB2312字符串
Input:		  x,y :起点坐标	 
              len :数字的位数 不能大于10
			  num:数值(0~4294967295);
              mode:模式0,不填充模式;1,填充模式 
******************************************************/	 		  
void OLED_ShowNumString(u8 x,u8 y,u32 num,u8 len,u8 mode)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp = (num/mypow(10,len-t-1))%10;	
		if(enshow==0 && t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowGB2312String(x+8*t,y,(uint8_t*)" ",mode);
				continue;
			}
			else enshow=1; 		 	 
		}
		OLED_Show8X16Num(x+8*t,y,temp,mode); 
	}
}
/******************************************************
Function:	  void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
Description:  在任意位置显示数字
Calls:		  void OLED_ShowGB2312String(u8 x,u8 y,u8 *p,u8 mode);     //在任意点显示GB2312字符串
Input:		  x,y :起点坐标	 
       		  num:有效显示长度为8位，包含符号，不包含小数点，
            lenInt:数字的位数 包括符号 正负号  >=1
            lenFlo：小数的位数， 不包括小数点  >=0
            mode:模式0,不填充背景填充：模式1,填充模式 
******************************************************/	 		  
void OLED_ShowFloatNum(u8 x,u8 y,float num,u8 lenInt,u8 lenFlo,u8 mode)
{         	
	u8 t,temp = 0;
	u8 enshow=0;     //显示使能，显示区域多出实际显示区域不显示
	u8 sign = 0;     //正负号，0：正号； 1：负号
	u32 pre = 0, i = 0;
	s32 numNew;      //整数
	u32 numInt;      //无符整数
	u32 numFlo;      //得到小数位
	i = pre = mypow(10,lenFlo);   //得到小数精度
	i *=10;
	numNew = (s32)(num*i); //得到有符型整数
	i = numNew%10;
	if((i <= 9)&&(i >= 5)) numNew += 10; //四舍五入
	numNew /=10;
	
  if (numNew < 0)     //判断正负数 并赋值给numInt
	{
		numInt = -numNew;
		sign = 1;
	}	
  else numInt = numNew;
	numFlo = numInt%pre; //得到小数位
	numInt = numInt/pre; //得到整数位
	
//显示整数部分
	if(numInt)//不为0
	{
		for(t=0;t<lenInt;t++)
		{
		
			temp = (numInt/mypow(10,lenInt-t-1))%10;	
			if(enshow==0)
			{
				if(temp==0)
				{
					OLED_ShowGB2312String(x+8*t,y,(uint8_t*)" ",mode);
					continue;
				}
				else 
				{	
					if(sign) 
					{
						if(t) OLED_ShowGB2312String(x+8*(t-1),y,(uint8_t*)"-",mode);
					}
					enshow=1; 
				}				
			}
			OLED_Show8X16Num(x+8*t,y,temp,mode); 
		} 
	}
	else //0
	{	
		if(sign)
		{
			if(lenInt>=2) OLED_ShowGB2312String(x+8*(lenInt-2),y,(uint8_t*)"-",mode);
		}
		if(lenInt>=1)
		OLED_Show8X16Num(x+8*(lenInt-1),y,temp,mode);
	}
//显示小数部分	
	if(lenFlo)           
	{
	  OLED_ShowGB2312String(x+8*lenInt,y,(uint8_t*)".",mode);
		for(t=0; t<lenFlo; t++)
		{
		  temp = (numFlo/mypow(10,lenFlo-t-1))%10;
			OLED_Show8X16Num(x + 8*(lenInt + t +1), y, temp, mode);
		} 
	}
  		
  		
}




