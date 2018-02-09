#ifndef _ST7735S_LCD_H
#define _ST7735S_LCD_H

#include "gpio.h"
#include "stm32f10x.h"
#include "systick.h"
#include "st7735s_gui.h"

#ifndef __SITE__
#define __SITE__
/*
 * 定义坐标结构体
 */
typedef struct
{
    u16 x;
    u16 y;
} Site_t;

/*
 * 定义矩阵大小结构体
 */
typedef struct
{
    u16 W;       
    u16 H;       
} Size_t;
#endif



#define ST7735S_DIR_DEFAULT     0       //定义默认方向 (0~3)



/*********************** 上层封装******************************/

#define LCD_H                   ST7735S_get_h()                 
#define LCD_W                   ST7735S_get_w()   

#define LCD_INIT()              LCD_ST7735S_init()              
#define LCD_PTLON(site,size)    LCD_ST7735S_ptlon(site,size)    
#define LCD_RAMWR()             LCD_WR_CMD(0x2C)                
#define LCD_WR_DATA(data)       do{LCD_ST7735S_WR_8DATA((u8)((data)>>8) );LCD_ST7735S_WR_8DATA( (u8)(data) );}while(0)       //???
#define LCD_WR_CMD(cmd)         LCD_ST7735S_WR_8CMD(cmd)        
#define LCD_SET_DIR(opt)        LCD_ST7735S_dir(opt)            

#define LCD_DIR                 ST7735S_get_dir()               


//函数调用，速度太慢，改为宏定义
//#define LCD_WR_8DATA(data)      LCD_ST7735S_wr_data(data)       
//#define LCD_WR_8CMD(cmd)        LCD_ST7735S_wr_cmd(cmd)         
#define LCD_RST    PTC13                //??

#define ST7735S_H       128
#define ST7735S_W       128


/********************* ST7735S内部实现 ***********************/
//LCD 的GPIO
#define      ST7735S_WR_GPIO_CLK                     RCC_APB2Periph_GPIOF
#define      ST7735S_WR_GPIO_PORT                    GPIOF
#define      ST7735S_WR_GPIO_PIN                     GPIO_Pin_3

#define      ST7735S_RD_GPIO_CLK                     RCC_APB2Periph_GPIOA
#define      ST7735S_RD_GPIO_PORT                    GPIOA
#define      ST7735S_RD_GPIO_PIN                     GPIO_Pin_0

#define      ST7735S_CS_GPIO_CLK                     RCC_APB2Periph_GPIOF
#define      ST7735S_CS_GPIO_PORT                    GPIOF
#define      ST7735S_CS_GPIO_PIN                     GPIO_Pin_1

#define      ST7735S_RS_GPIO_CLK                     RCC_APB2Periph_GPIOF
#define      ST7735S_RS_GPIO_PORT                    GPIOF
#define      ST7735S_RS_GPIO_PIN                     GPIO_Pin_5

#define      ST7735S_RST_GPIO_CLK                     RCC_APB2Periph_GPIOF
#define      ST7735S_RST_GPIO_PORT                    GPIOF
#define      ST7735S_RST_GPIO_PIN                     GPIO_Pin_2

#define      ST7735S_DATA_GPIO_CLK                    RCC_APB2Periph_GPIOE
#define      ST7735S_DATA_GPIO_PORT                   GPIOE
#define      ST7735S_DATA_GPIO_PIN                    (GPIO_Pin_0| GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7)

//此部分需要修改初始化里的代码   
#define     LCD_ST7735S_P0(a)              {GPIOE->BSRR = (u8)a;GPIOE->BRR = ((~(u8)a)&0x00ff);}
//#define     LCD_ST7735S_PIN             PTC_B0_IN
//#define     LCD_ST7735S_PDDR_OUT()      (DDRC_B0 = 0xff)
//#define     LCD_ST7735S_PDDR_IN()       (DDRC_B0 = 0)


#define ST7735S_DELAY()                   do{delay_us(10);}while(0)
#define ST7735S_DELAY_MS(ms)    delay_ms(ms)


void     LCD_ST7735S_init(void);
void     LCD_ST7735S_dir(u8 option);
void     LCD_ST7735S_ptlon(Site_t site, Size_t size);
u16   ST7735S_get_h(void);
u16   ST7735S_get_w(void);
u8    ST7735S_get_dir(void);


void LCD_ST7735S_wr_data(u8 data);
void LCD_ST7735S_wr_cmd(u8 cmd);

#define     LCD_ST7735S_WR_OUT      PFout(3)
#define     LCD_ST7735S_RD_OUT      PAout(0)
#define     LCD_ST7735S_CS_OUT      PFout(1)
#define     LCD_ST7735S_RS_OUT      PFout(5)
#define     LCD_ST7735S_RST_OUT     PFout(2)

#define LCD_ST7735S_WR_8CMD(cmd)    do\
                            {\
                                LCD_ST7735S_RD_OUT=1;\
                                LCD_ST7735S_RS_OUT=0;\
                                LCD_ST7735S_CS_OUT=0;\
                                LCD_ST7735S_P0((u8)(cmd));\
                                LCD_ST7735S_WR_OUT=0;\
                                LCD_ST7735S_WR_OUT=1;\
                                LCD_ST7735S_CS_OUT=1;\
                            }while(0)   //LCD_WR=0;LCD_WR=1;产生一个上升沿

#define LCD_ST7735S_WR_8DATA(data)  do\
                            {\
                                LCD_ST7735S_RD_OUT=1;\
                                LCD_ST7735S_RS_OUT=1;\
                                LCD_ST7735S_CS_OUT=0;\
                                LCD_ST7735S_P0((u8)(data));\
                                LCD_ST7735S_WR_OUT=0;\
                                LCD_ST7735S_WR_OUT=1;\
                                LCD_ST7735S_CS_OUT=1;\
                            }while(0)   //LCD_WR=0;从这里写入数据到RAM


#define LCD_ST7735S_WR_DATA(data)       do{LCD_ST7735S_WR_8DATA((u8)((data)>>8) );LCD_ST7735S_WR_8DATA( (u8)(data) );}while(0)       //写数据



#endif 
