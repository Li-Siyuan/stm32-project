#ifndef _OV7725_EAGLE_H_
#define _OV7725_EAGLE_H_

#include "ov7725_reg.h"
#include "stm32f10x.h"


#define ARR_SIZE( a ) ( sizeof( (a) ) / sizeof( ((a)[0]) ) )
typedef struct
{
    u8 addr;                 /*寄存器地址*/
    u8 val;                   /*寄存器值*/
} reg_s;

//????????
typedef enum
{
    IMG_NOTINIT = 0,
    IMG_FINISH,             //图像采集完成
    IMG_FAIL,               //图像采集失败(采集行数少了)
    IMG_GATHER,             //图像采集中
    IMG_START,              //开始采集图像
    IMG_STOP,               //禁止采集图像
} IMG_STATUS_e;





#define CAMERA_USE_HREF     0          //是否使用行中断：1使用 0不用  
#define CAMERA_COLOR        0          //摄像头图像模式：0二值化 1灰度 2RGB565     
#define CAMERA_POWER        0          //摄像头电源选择：0(3.3V) 1(5V)     

//摄像头API接口
#define camera_init(imgaddr)    ov7725_eagle_init(imgaddr)
#define camera_get_img()        ov7725_eagle_get_img()


#define camera_vsync()          ov7725_eagle_vsync()
#define camera_href()           ov7725_eagle_href()
#define camera_dma()            ov7725_eagle_dma()

//上层封装
#define CAMERA_DMA_CH       OV7725_EAGLE_DMA_CH      
#define CAMERA_W            OV7725_EAGLE_W           
#define CAMERA_H            OV7725_EAGLE_H            

#define CAMERA_SIZE         OV7725_EAGLE_SIZE           
#define CAMERA_DMA_NUM      CAMERA_SIZE       

 

//摄像头参数             
#define OV7725_EAGLE_W            80                       //摄像头图像宽度            
#define OV7725_EAGLE_H            60                         //摄像头图像高度          
#define OV7725_EAGLE_SIZE         (OV7725_EAGLE_W * OV7725_EAGLE_H/8 )   //图像占用空间大小



/************************** OV7725 连接引脚定义********************************/
// RCLK触发DMA开始中断
#define      EAGLE_RCLK_GPIO_CLK                     RCC_APB2Periph_GPIOA
#define      EAGLE_RCLK_GPIO_PORT                    GPIOA
#define      EAGLE_RCLK_GPIO_PIN                     GPIO_Pin_0

#define      EAGLE_RCLK_EXTI_SOURCE_PORT            GPIO_PortSourceGPIOA
#define      EAGLE_RCLK_EXTI_SOURCE_PIN             GPIO_PinSource0
#define      EAGLE_RCLK_EXTI_LINE                   EXTI_Line0
#define      EAGLE_RCLK_EXTI_IRQ                    EXTI0_IRQn
#define      EAGLE_RCLK_EXTI_INT_FUNCTION           EXTI0_IRQHandler

// 8位数据口
#define      EAGLE_DATA_GPIO_CLK                     RCC_APB2Periph_GPIOF
#define      EAGLE_DATA_GPIO_PORT                    GPIOF
#define      EAGLE_DATA_0_GPIO_PIN                   GPIO_Pin_0
#define      EAGLE_DATA_1_GPIO_PIN                   GPIO_Pin_1
#define      EAGLE_DATA_2_GPIO_PIN                   GPIO_Pin_2
#define      EAGLE_DATA_3_GPIO_PIN                   GPIO_Pin_3
#define      EAGLE_DATA_4_GPIO_PIN                   GPIO_Pin_4
#define      EAGLE_DATA_5_GPIO_PIN                   GPIO_Pin_5
#define      EAGLE_DATA_6_GPIO_PIN                   GPIO_Pin_6
#define      EAGLE_DATA_7_GPIO_PIN                   GPIO_Pin_7

// OV7725场中断
#define      EAGLE_VSYNC_GPIO_CLK                    RCC_APB2Periph_GPIOA
#define      EAGLE_VSYNC_GPIO_PORT                   GPIOA
#define      EAGLE_VSYNC_GPIO_PIN                    GPIO_Pin_4

#define      EAGLE_VSYNC_EXTI_SOURCE_PORT            GPIO_PortSourceGPIOA
#define      EAGLE_VSYNC_EXTI_SOURCE_PIN             GPIO_PinSource4
#define      EAGLE_VSYNC_EXTI_LINE                   EXTI_Line4
#define      EAGLE_VSYNC_EXTI_IRQ                    EXTI4_IRQn
#define      EAGLE_VSYNC_EXTI_INT_FUNCTION           EXTI4_IRQHandler







//DMA参数
#define OV7725_EAGLE_DMA_CH                   DMA1_Channel6             //DMA采集通道   
#define OV7725_EAGLE_DMA_Finish               DMA_GetFlagStatus(DMA_FLAG_TC)==RESET
// 当使用存储器到存储器模式时候，通道可以随便选，没有硬性的规定    
#define OV7725_EAGLE_DMA_CLOCK                RCC_AHBPeriph_DMA1
// 传输完成标志
#define OV7725_EAGLE_DMA_FLAG_TC              DMA1_FLAG_TC6
//单次传输总量
#define OV7725_EAGLE_BUFFER_SIZE              1

//DMA传输完成中断配置
//#define Set_DMA_FINISH_Flag                 DMA1 ->IFCR &= 0xf0fffff//开启完成中断通道
#define Enable_DMA_FINISH_IRQ		              OV7725_EAGLE_DMA_CH ->CCR |= 0x0f//开启完成中断
#define Disable_DMA_FINISH_IRQ                OV7725_EAGLE_DMA_CH ->CCR &= 0xfff0//关闭完成中断
#define Clear_DMA_FINISH_Flag                 DMA1->IFCR |= 0xf00000   //清除DMA完成标志位
#define EAGLE_DMA_Handler                     DMA1_Channel6_IRQHandler          //中断服务函数 
#define EAGLE_DMA_IRQ                         DMA1_Channel6_IRQn







void OV7725_EAGLE_DMA_Conf(void);
u8 OV7725_Eagle_Conf(u8 *imgaddr);
void OV7725_Eagle_Get_Img(void);
void img_extract(void *dst, void *src, uint32_t srclen);




#endif 


