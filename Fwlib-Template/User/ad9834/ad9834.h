#ifndef __AD9834_H
#define __AD9834_H

#include "stm32f10x.h"
#include "include.h"


#define Triangle_Wave    0x2002
#define Sine_Wave        0x2028
#define Sine_Cube_Wave   0x2038

/* AD9834晶振频率 */
#define AD9834_SYSTEM_COLCK     10000000UL  //10MHz(unsigned long)

/* AD9834 控制引脚 */
#define AD9834_Control_Port     GPIOA
#define AD9834_FSYNC            GPIO_Pin_4
#define AD9834_SCLK             GPIO_Pin_5
#define AD9834_SDATA            GPIO_Pin_7
//#define AD9834_RESET            GPIO_Pin_9
#define AD9834_FSYNC_SET        GPIO_SetBits(AD9834_Control_Port ,AD9834_FSYNC)
#define AD9834_FSYNC_CLR   			GPIO_ResetBits(AD9834_Control_Port ,AD9834_FSYNC)
#define AD9834_SCLK_SET   			GPIO_SetBits(AD9834_Control_Port ,AD9834_SCLK)
#define AD9834_SCLK_CLR   			GPIO_ResetBits(AD9834_Control_Port ,AD9834_SCLK)
#define AD9834_SDATA_SET   			GPIO_SetBits(AD9834_Control_Port ,AD9834_SDATA)
#define AD9834_SDATA_CLR   			GPIO_ResetBits(AD9834_Control_Port ,AD9834_SDATA)
//#define AD9834_RESET_SET   			GPIO_SetBits(AD9834_Control_Port ,AD9834_RESET)
//#define AD9834_RESET_CLR   			GPIO_ResetBits(AD9834_Control_Port ,AD9834_RESET)

#define FREQ_0      0
#define FREQ_1      1


//																					0x2002				0x2028								0x2038       0x0100
//控制寄存器位定义                          三角波      正弦波(不带方波)    正弦波(带方波)   复位
#define DB15        0//                        0            0                     0           0
#define DB14        0//                        0            0                     0           0           
#define DB13        B28//(28位连续写两次)      1            1                     1           0          
#define DB12        HLB//(28位分开写)          0            0                     0           0           
#define DB11        FSEL//                     0            0                     0           0          
#define DB10        PSEL//                     0            0                     0           0           
#define DB9         PIN_SW//                   0            0                     0           0           //屏蔽引脚选择寄存器，使用寄存器位选择寄存器
#define DB8         RESET//                    0            0                     0           1           
#define DB7         SLEEP1//                   0            0                     0           0           
#define DB6         SLEEP12//                  0            0                     0           0           
#define DB5         OPBITEN//                  0            1                     1           0           //1则使能SIGN_BIT_OUT输出(此时MODE必须为0输出正弦波)
#define DB4         SIGN_PIB//                 0            0                     1           0           //1则输出方波，0则输出NCO的MSB
#define DB3         DIV2//                     0            1                     1           0           //0：输出MSB二分频，1：输出MSB或方波
#define DB2         0//(保留)                  0            0                     0           0
#define DB1         MODE//                     1            0                     0           0           //1输出三角波，0输出正弦波
#define DB0         0//(保留)                  0            0                     0           0


/* AD9834函数声明 */
extern void AD9834_Write_16Bits(unsigned int data) ;  //写一个字到AD9834
extern void AD9834_Select_Wave(unsigned int initdata) ; //选择输出波形
extern void Init_AD9834(void) ;//初始化配置
extern void AD9834_Set_Freq(unsigned char freq_number, unsigned long freq) ;//选择输出寄存器和输出频率值
void AD9834_Reset(unsigned int initdata);  //复位ad9834内部寄存器

#endif /* AD9834_H */
