/*********188开发板专用dma函数****************
**********作者：李思远******************
**********参考：秉火******************/

#include "dma_m2m.h"

/*

extern const uint32_t aSRC_Const_Buffer[BUFFER_SIZE];
extern uint32_t aDST_Buffer[BUFFER_SIZE];

int main(void)
{	
	USART_Conf();
	
	DMA_M2M_Conf();
	printf("DMA  Init  OK  \n");
	
	 // 等待DMA传输完成 
  while(DMA_GetFlagStatus(DMA_FLAG_TC)==RESET); 
  
  // 比较源数据与传输后数据 
  if(Buffercmp(aSRC_Const_Buffer, aDST_Buffer, BUFFER_SIZE))
	{
		printf("相等\n");
	}
	else
		printf("不相等\n");
  
	while(1);
}


//GPIO口传输到数组
extern uint8_t aDST_Buffer[BUFFER_SIZE];

int main()
{  	
	LED_Conf();
	
	USART_Conf();
	GPIOA_Conf();

	printf("\n开始传输\n");
		
	LED_ALL_ON;
	
	while(1)
	{

		DMA_M2M_Conf();
//     // while(DMA_GetFlagStatus(DMA_FLAG_TC)==RESET); 
////		Clear_DMA_Flag ;	
	  delay_ms(1000);
		LED_ALL_TOGGLE;
//	
//	  Set_DMA_Flag;
	
	}
}

*/



/* 定义aSRC_Const_Buffer数组作为DMA传输数据源
 * const关键字将aSRC_Const_Buffer数组变量定义为常量类型
 * 表示数据存储在内部的FLASH中
 */
//const uint32_t aSRC_Const_Buffer[BUFFER_SIZE]= {
//                                    0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
//                                    0x11121314,0x15161718,0x191A1B1C,0x1D1E1F20,
//                                    0x21222324,0x25262728,0x292A2B2C,0x2D2E2F30,
//                                    0x31323334,0x35363738,0x393A3B3C,0x3D3E3F40,
//                                    0x41424344,0x45464748,0x494A4B4C,0x4D4E4F50,
//                                    0x51525354,0x55565758,0x595A5B5C,0x5D5E5F60,
//                                    0x61626364,0x65666768,0x696A6B6C,0x6D6E6F70,
//                                    0x71727374,0x75767778,0x797A7B7C,0x7D7E7F80};
/* 定义DMA传输目标存储器
 * 存储在内部的SRAM中																		
 */
uint8_t aDST_Buffer[BUFFER_SIZE]={0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0};
																		
void DMA_M2M_Conf(void)
{
	  DMA_InitTypeDef DMA_InitStructure;
//	  Set_DMA_Flag;
	
		// 开启DMA时钟
		RCC_AHBPeriphClockCmd(DMA_CLOCK, ENABLE);
		// 地址1
    DMA_InitStructure.DMA_PeripheralBaseAddr = addr1 ;//(uint32_t)aSRC_Const_Buffer;
		// 地址2
		DMA_InitStructure.DMA_MemoryBaseAddr = addr2;
		// 方向：SRC：地址1到2；  DST：2到1	
		DMA_InitStructure.DMA_DIR = Derction_2to1;
		// 传输大小	
		DMA_InitStructure.DMA_BufferSize = BUFFER_SIZE;
		// 外设（内部的FLASH）地址不变
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
		// 内存地址递增
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
		// 外设数据单位	
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		// 内存数据单位
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 
		// DMA模式，一次或者循环模式
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
		//DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  
		// 优先级：高	
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;
		// 使能内存到内存的传输
		DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
		// 配置DMA通道		   
		DMA_Init(DMA_CHANNEL, &DMA_InitStructure);
    
		//清除DMA数据流传输完成标志位
    Clear_DMA_Flag ;
	 	Enable_DMA_IRQ	;
		//Set_DMA_CH;
		NVIC_EnableIRQ(DMA_IRQ);
		// 使能DMA
		DMA_Cmd(DMA_CHANNEL,ENABLE);
}

/**
  * 判断指定长度的两个数据源是否完全相等，
  * 如果完全相等返回1，只要其中一对数据不相等返回0
  */
uint8_t Buffercmp(const uint32_t* pBuffer, 
                  uint32_t* pBuffer1, uint16_t BufferLength)
{
  /* 数据长度递减 */
  while(BufferLength--)
  {
    /* 判断两个数据源是否对应相等 */
    if(*pBuffer != *pBuffer1)
    {
      /* 对应数据源不相等马上退出函数，并返回0 */
      return 0;
    }
    /* 递增两个数据源的地址指针 */
    pBuffer++;
    pBuffer1++;
  }
  /* 完成判断并且对应数据相对 */
  return 1;  
}

//typedef struct
//{
//  uint32_t DMA_PeripheralBaseAddr;   // 外设地址
//  uint32_t DMA_MemoryBaseAddr;       // 存储器地址
//  uint32_t DMA_DIR;                  // 传输方向
//  uint32_t DMA_BufferSize;           // 传输数目
//  uint32_t DMA_PeripheralInc;        // 外设地址增量模式
//  uint32_t DMA_MemoryInc;            // 存储器地址增量模式
//  uint32_t DMA_PeripheralDataSize;   // 外设数据宽度
//  uint32_t DMA_MemoryDataSize;       // 存储器数据宽度
//  uint32_t DMA_Mode;                 // 模式选择
//  uint32_t DMA_Priority;             // 通道优先级
//  uint32_t DMA_M2M;                  // 存储器到存储器模式
//}DMA_InitTypeDef;


