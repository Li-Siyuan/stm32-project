#ifndef __DMA_M2M_H
#define __DMA_M2M_H

// DMA-存储器到存储器模式 ，即内部的FLASH到内部的SRAM
	
#include "stm32f10x.h"
#include "led.h"






//DMA地址
#define     addr1            (uint32_t)aDST_Buffer
#define     addr2            GPIOC_BASE+0x0C

//传输方向
#define     Derction_1to2    DMA_DIR_PeripheralSRC
#define     Derction_2to1    DMA_DIR_PeripheralDST

// 要发送的数据大小
#define BUFFER_SIZE     8




// 当使用存储器到存储器模式时候，通道可以随便选，没有硬性的规定
#define DMA_CHANNEL     DMA1_Channel6
#define DMA_CLOCK       RCC_AHBPeriph_DMA1

// 传输完成标志
#define DMA_FLAG_TC     DMA1_FLAG_TC6



void DMA_M2M_Conf(void);		
uint8_t Buffercmp(const uint32_t* pBuffer, 
                  uint32_t* pBuffer1, uint16_t BufferLength);

//#define Set_DMA_Flag                    DMA1 ->IFCR &= 0xf0fffff//开启完成中断通道
#define Enable_DMA_IRQ		              DMA_CHANNEL ->CCR |= 0x0f//开启完成中断
#define Disable_DMA_IRQ                 DMA_CHANNEL ->CCR &= 0xfff0//关闭完成中断
#define Clear_DMA_Flag                  DMA1->IFCR |= 0xf00000 //清除DMA完成标志位
#define DMA_Handler                     DMA1_Channel6_IRQHandler          //中断服务函数 
#define DMA_IRQ                         DMA1_Channel6_IRQn
#define DMA_Finish       DMA_GetFlagStatus(DMA_FLAG_TC)==RESET



#endif


