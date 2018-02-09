//使用样例
/*********************************************************************************************************************************************
 *#include "stm32f10x.h"
 *#include "include.h"

  //   注：使用时先调用初始化函数初始化IO配置，然后选择波形,选择频率值（无先后顺序）即可输出。
	//   配置ad9834：通过写ad9834的16位控制寄存器来实现所有设置
	//   该程序默认使用FREQ0与PHASE0，未对相位进行设置

	//IOUT和IOUTB产生互补的输出电压，输出正弦波或三角波
	//SIGN BIT OUT输出方波，该方波由正弦信号在芯片内部滤波后输入比较器得到，输入比较器的正弦波频率需不小于3MHz，方波幅值为0-DVDD
	//AVDD与DVDD范围：2.3-5.5V
	//输出频率应小于参考频率的33%
	//为保证达到额定性能，输出电压应小于VDD-1.5V（最大顺从电压）

	int main()
	{	
		//初始化ad9834的四个IO
		Init_AD9834();
		
		//复位ad9834内部寄存器（关掉输出）并选择波形
		//三角波Triangle_Wave，正弦波(不带方波)Sine_Wave，正弦波(带方波)Sine_Cube_Wave
		AD9834_Reset(Sine_Cube_Wave);
		
		//设置频率4MHz，宏定义的波形设置16位默认使用的FREQ0
		AD9834_Set_Freq(FREQ_0,4000000);
		
		//开始输出波形
		AD9834_Select_Wave(Sine_Cube_Wave);
		
		//死循环
		while(1);
	}
***********************************************************************************************************************************************/

#include "stm32f10x.h"
#include "ad9834.h"

/****************************************************************
函数名称: AD9834_Write_16Bits
功    能: 向AD9834写入16为数据
参    数: data --  要写入的16位数据 MSB
返回值  : 无
*****************************************************************/
/*
时序：FSYNC置0，一帧开始，置1，一帧结束
			SCLK置0，发送数据，置1，改变数据
			MSB高位先行，一帧16位
*/
void AD9834_Write_16Bits(unsigned int data)
{
    unsigned char i = 0 ;
	
    AD9834_SCLK_CLR;
    AD9834_SCLK_SET ;
    AD9834_FSYNC_CLR ;
    for(i=0 ;i<16 ;i++) 
    {
        if(data & 0x8000)
          AD9834_SDATA_SET ;
        else
          AD9834_SDATA_CLR ;
        
        AD9834_SCLK_CLR ;
        data <<= 1 ;
				delay_us(5);
        AD9834_SCLK_SET ;
    }
    
    AD9834_FSYNC_SET ;
}



/***********************************************************************************
函数名称：AD9834_Select_Wave
功    能：软件为控制，
    --------------------------------------------------
    IOUT正弦波 ，SIGNBITOUT方波 ，写FREQREG0 ，写PHASE0
    ad9834_write_16bit(0x2028)   一次性写FREQREG0
    ad9834_write_16bit(0x0038)   单独改写FREQREG0的LSB
    ad9834_write_16bit(0x1038)   单独改写FREQREG0的MSB
    --------------------------------------------------
    IOUT三角波 ，写PHASE0
    ad9834_write_16bit(0x2002)   一次性写FREQREG0
    ad9834_write_16bit(0x0002)   单独改写FREQREG0的LSB
    ad9834_write_16bit(0x1008)   单独改写FREQREG0的MSB
参    数：initdata -- 要输入的命令
返回值  ：无
************************************************************************************/
void AD9834_Select_Wave(unsigned int initdata)
{
    AD9834_FSYNC_SET;
    AD9834_SCLK_SET;
    
//    AD9834_RESET_SET;
//    AD9834_RESET_SET;
//    AD9834_RESET_CLR;
     
    AD9834_Write_16Bits(initdata);
}


//复位ad9834内部寄存器
void AD9834_Reset(unsigned int initdata)
{
    AD9834_FSYNC_SET;
    AD9834_SCLK_SET;
    
//    AD9834_RESET_SET;
//    AD9834_RESET_SET;
//    AD9834_RESET_CLR;
     
    AD9834_Write_16Bits(initdata|0x0100);
}

/****************************************************************
函数名称: Init_AD9834
功    能: 初始化AD9834控制引脚
参    数: 无
返回值  : 无
*****************************************************************/
void Init_AD9834(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    GPIO_InitStructure.GPIO_Pin = AD9834_FSYNC | AD9834_SCLK | AD9834_SDATA  ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
    GPIO_Init(AD9834_Control_Port ,&GPIO_InitStructure) ;
}


/****************************************************************
函数名称: AD9834_Set_Freq
功    能: 设置频率值
参    数: freq_number -- 要写入的平率寄存器(FREQ_0或FREQ_1)
          freq -- 频率值 (Freq_value(value)=Freq_data(data)*FCLK/2^28)
返回值  : 无
*****************************************************************/
/*
频率寄存器由两个14位寄存器组成
通过向16位控制器存器两次写操作来写频率寄存器，先低14位，后高14位
DB15=0，DB14=1时，DB0-DB13为FREQ0的14位
DB15=1，DB14=0时，DB0-DB13为FREQ1的14位
*/
 void AD9834_Set_Freq(unsigned char freq_number, unsigned long freq)
{
    unsigned long FREQREG = (unsigned long)(268435456.0/AD9834_SYSTEM_COLCK*freq);  //268435456即2^28  ad9834频率计算公式为f=fMCLK/2^28 * FREQREG
    
    unsigned int FREQREG_LSB_14BIT = (unsigned int)FREQREG;
    unsigned int FREQREG_MSB_14BIT = (unsigned int)(FREQREG>>14);
    
    if(freq_number == FREQ_0)
    {
        FREQREG_LSB_14BIT &= ~(1U<<15);
        FREQREG_LSB_14BIT |= 1<<14;
        FREQREG_MSB_14BIT &= ~(1U<<15);
        FREQREG_MSB_14BIT |= 1<<14;
    }
    else
    {
        FREQREG_LSB_14BIT &= ~(1<<14);
        FREQREG_LSB_14BIT |= 1U<<15;
        FREQREG_MSB_14BIT &= ~(1<<14);
        FREQREG_MSB_14BIT |= 1U<<15;
    }
    
    AD9834_Write_16Bits(FREQREG_LSB_14BIT);
    AD9834_Write_16Bits(FREQREG_MSB_14BIT);
    
}
//============================================================//

