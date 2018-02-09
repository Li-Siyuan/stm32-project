/*
 *山外鹰眼摄像头，支持硬件二值化
 *据山外飞思卡尔例程思路改变
 *作者：李思远
 */
 /*
 #include "stm32f10x.h"
#include "include.h"

u8 imgbuff[CAMERA_SIZE];                           
u8 img[CAMERA_H*CAMERA_W];                          

uint8_t flag=0;

int main()
{
	uint8_t i=0;
	NVIC_Conf();
	USART_Conf();
	LED_Conf();
	OV7725_Eagle_Conf(imgbuff); 
	printf("Eagle  Init  Ok  ");
	
	while(1)
	{
		OV7725_Eagle_Get_Img();
		LED1_TOGGLE;
		while(flag==0);
		for(i=0;i<CAMERA_SIZE/8;i++)
			printf("%d",imgbuff[i]);
	  flag = 0;
	}
}

 */

#include "gpio.h"
#include "dma_m2m.h"
#include "OV7725_Eagle.h"
#include "systick.h"
#include "usart.h"
#include "sccb.h"

#define OV7725_Eagle_Delay_ms(time)  delay_ms(time)


u8   *OV7725_Eagle_img_buff;//图像缓存区

volatile IMG_STATUS_e      OV7725_Eagle_img_flag = IMG_FINISH;   //图像状态


static u8 OV7725_Eagle_Reg_Init(void);
static void OV7725_Eagle_GPIO_Init(void);



void OV7725_EAGLE_DMA_Conf(void)
{
	 DMA_InitTypeDef DMA_InitStructure;
	
		// 开启DMA时钟
		RCC_AHBPeriphClockCmd(OV7725_EAGLE_DMA_CLOCK, ENABLE);
		// 地址1
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)OV7725_Eagle_img_buff ;//(uint32_t)aSRC_Const_Buffer;
		// 地址2
		DMA_InitStructure.DMA_MemoryBaseAddr = GPIOF_BASE+0x08;
		// 方向：地址2到地址1
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
		// 传输大小	
		DMA_InitStructure.DMA_BufferSize = OV7725_EAGLE_BUFFER_SIZE;
		// 地址1自增
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
		// 地址2不变
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
    Clear_DMA_FINISH_Flag;
		
		//设置DMA完成中断
   // Set_DMA_FINISH_CH;
	//	Enable_DMA_FINISH_IRQ	;	
	  DMA_ClearFlag(DMA1_FLAG_TC6);
	//	NVIC_EnableIRQ(EAGLE_DMA_IRQ );
		
	//	DMA_Cmd(DMA_CHANNEL,ENABLE);
	//  DMAy_Channelx->CPAR = DMA_InitStruct->DMA_PeripheralBaseAddr+1;
}




//鹰眼初始化函数
u8 OV7725_Eagle_Conf(u8 *imgaddr)
{
    OV7725_Eagle_img_buff = imgaddr; //定义图像缓存
	  OV7725_Eagle_GPIO_Init();//初始化GPIO
	  OV7725_EAGLE_DMA_Conf();//DMA传输，在RCLK中断里打开
    while(OV7725_Eagle_Reg_Init() == 0);//初始化寄存器
    return 0;
}

static void OV7725_Eagle_GPIO_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
		EXTI_InitTypeDef EXTI_InitStructure;
	
		/*开启时钟*/
	  RCC_APB2PeriphClockCmd (EAGLE_DATA_GPIO_CLK|EAGLE_VSYNC_GPIO_CLK|EAGLE_RCLK_GPIO_CLK, ENABLE );	
	
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//下拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  //V场中断
		GPIO_InitStructure.GPIO_Pin = EAGLE_VSYNC_GPIO_PIN;
		GPIO_Init(EAGLE_VSYNC_GPIO_PORT, &GPIO_InitStructure);
	
	  //SCLK,DMA读数据时钟
	  GPIO_InitStructure.GPIO_Pin = EAGLE_RCLK_GPIO_PIN ;
		GPIO_Init(EAGLE_RCLK_GPIO_PORT, &GPIO_InitStructure);
	
	  //数据GPIO
    GPIO_InitStructure.GPIO_Pin = 	EAGLE_DATA_0_GPIO_PIN | EAGLE_DATA_1_GPIO_PIN |
																			EAGLE_DATA_2_GPIO_PIN | EAGLE_DATA_3_GPIO_PIN |
																			EAGLE_DATA_4_GPIO_PIN | 	EAGLE_DATA_5_GPIO_PIN |
																			EAGLE_DATA_6_GPIO_PIN | 	EAGLE_DATA_7_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
		GPIO_Init(EAGLE_DATA_GPIO_PORT, &GPIO_InitStructure);  
			
			/*配置场中断*/
    GPIO_EXTILineConfig(EAGLE_VSYNC_EXTI_SOURCE_PORT, EAGLE_VSYNC_EXTI_SOURCE_PIN);
    EXTI_InitStructure.EXTI_Line = EAGLE_VSYNC_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising ; 
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    EXTI_GenerateSWInterrupt(EAGLE_VSYNC_EXTI_LINE);
    EXTI_ClearITPendingBit(EAGLE_VSYNC_EXTI_LINE);//清中断标志位		
    //NVIC_EnableIRQ(EAGLE_VSYNC_EXTI_IRQ);		//在开始采集图像函数里使能
		
			/*配置RCLK引起的DMA开始中断*/
		GPIO_EXTILineConfig(EAGLE_RCLK_EXTI_SOURCE_PORT, EAGLE_RCLK_EXTI_SOURCE_PIN);
    EXTI_InitStructure.EXTI_Line = EAGLE_RCLK_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling ; 
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    EXTI_GenerateSWInterrupt(EAGLE_RCLK_EXTI_LINE);
		EXTI_ClearITPendingBit(EAGLE_RCLK_EXTI_LINE);//清中断标志位
		//NVIC_EnableIRQ(EAGLE_RCLK_EXTI_IRQ);		//在场中断里使能
			
}




void OV7725_Eagle_Get_Img(void)
{
    OV7725_Eagle_img_flag = IMG_START;              
   
	//开场中断
	NVIC_EnableIRQ(EAGLE_VSYNC_EXTI_IRQ);
                       
    while(OV7725_Eagle_img_flag != IMG_FINISH)         
    {
      /*  if(OV7725_Eagle_img_flag == IMG_FAIL)            
        {
            OV7725_Eagle_img_flag = IMG_START;   
					
            //清中断标志位
						printf("\nERROR\n");
					  
            //开场中断 
						NVIC_EnableIRQ(EAGLE_VSYNC_EXTI_IRQ);
        }*/
    }
}


/*OV7725初始化配置表*/
reg_s OV7725_Eagle_reg[] =
{
    //寄存器，寄存器地址
    {OV7725_COM4         , 0xC1},
    {OV7725_CLKRC        , 0x00},
    {OV7725_COM2         , 0x03},
    {OV7725_COM3         , 0xD0},
    {OV7725_COM7         , 0x40},
    {OV7725_COM10        , 0x20},
    {OV7725_HSTART       , 0x3F},
    {OV7725_HSIZE        , 0x50},
    {OV7725_VSTRT        , 0x03},
    {OV7725_VSIZE        , 0x78},
    {OV7725_HREF         , 0x00},
    {OV7725_SCAL0        , 0x0A},
    {OV7725_AWB_Ctrl0    , 0xE0},
    {OV7725_DSPAuto      , 0xff},
    {OV7725_DSP_Ctrl2    , 0x0C},
    {OV7725_DSP_Ctrl3    , 0x00},
    {OV7725_DSP_Ctrl4    , 0x00},

#if (CAMERA_W == 80)
    {OV7725_HOutSize     , 0x14},
#elif (CAMERA_W == 160)
    {OV7725_HOutSize     , 0x28},
#elif (CAMERA_W == 240)
    {OV7725_HOutSize     , 0x3c},
#elif (CAMERA_W == 320)
    {OV7725_HOutSize     , 0x50},
#else

#endif

#if (CAMERA_H == 60 )
    {OV7725_VOutSize     , 0x1E},
#elif (CAMERA_H == 120 )
    {OV7725_VOutSize     , 0x3c},
#elif (CAMERA_H == 180 )
    {OV7725_VOutSize     , 0x5a},
#elif (CAMERA_H == 240 )
    {OV7725_VOutSize     , 0x78},
#else

#endif

    {OV7725_EXHCH        , 0x00},
    {OV7725_GAM1         , 0x0c},
    {OV7725_GAM2         , 0x16},
    {OV7725_GAM3         , 0x2a},
    {OV7725_GAM4         , 0x4e},
    {OV7725_GAM5         , 0x61},
    {OV7725_GAM6         , 0x6f},
    {OV7725_GAM7         , 0x7b},
    {OV7725_GAM8         , 0x86},
    {OV7725_GAM9         , 0x8e},
    {OV7725_GAM10        , 0x97},
    {OV7725_GAM11        , 0xa4},
    {OV7725_GAM12        , 0xaf},
    {OV7725_GAM13        , 0xc5},
    {OV7725_GAM14        , 0xd7},
    {OV7725_GAM15        , 0xe8},
    {OV7725_SLOP         , 0x20},
    {OV7725_LC_RADI      , 0x00},
    {OV7725_LC_COEF      , 0x13},
    {OV7725_LC_XC        , 0x08},
    {OV7725_LC_COEFB     , 0x14},
    {OV7725_LC_COEFR     , 0x17},
    {OV7725_LC_CTR       , 0x05},
    {OV7725_BDBase       , 0x99},
    {OV7725_BDMStep      , 0x03},
    {OV7725_SDE          , 0x04},
    {OV7725_BRIGHT       , 0x00},
    {OV7725_CNST         , 0xFF},
    {OV7725_SIGN         , 0x06},
    {OV7725_UVADJ0       , 0x11},
    {OV7725_UVADJ1       , 0x02},

};

u8 OV7725_Eagle_cfgnum = ARR_SIZE( OV7725_Eagle_reg ) ; /*结构体数组成员数目*/


static u8 OV7725_Eagle_Reg_Init(void)
{
    u16 i = 0;
    u8 Sensor_IDCode = 0;
    SCCB_GPIO_Config();

    //OV7725_Delay_ms(50);
    if( 0 == SCCB_WriteByte ( 0x12, 0x80 ) ) /*复位sensor */
    {
        printf("\n警告:SCCB写数据错误");
        return 0 ;
    }

    OV7725_Eagle_Delay_ms(50);

    if( 0 == SCCB_ReadByte( &Sensor_IDCode, 1, 0x0B ) )    /* ??sensor ID?*/
    {
        printf("\n读取ID失败");
        return 0;
    }
    printf("\nGet ID success,SENSOR ID is 0x%x", Sensor_IDCode);
    printf("\nConfig Register Number is %d ", OV7725_Eagle_cfgnum);
    if(Sensor_IDCode == OV7725_ID)
    {
        for( i = 0 ; i < OV7725_Eagle_cfgnum ; i++ )
        {
            if( 0 == SCCB_WriteByte(OV7725_Eagle_reg[i].addr, OV7725_Eagle_reg[i].val) )
            {
                printf("\n警告：写寄存器0x%x失败", OV7725_Eagle_reg[i].addr);
                return 0;
            }
        }
    }
    else
    {
        return 0;
    }
    printf("\nOV7725 Register Config Success!");
    return 1;
}

//鹰眼采集二值化图像数琚为一个字节八个像素，需要解压为八个字节进行显示
//0为白，255为黑
void img_extract(void *dst, void *src, uint32_t srclen)
{
    uint8_t colour[2] = {255, 0}; 
    uint8_t * mdst = dst;
    uint8_t * msrc = src;

    uint8_t tmpsrc;
    while(srclen --)
    {
        tmpsrc = *msrc++;
        *mdst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
    }
}



