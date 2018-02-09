/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "include.h"
uint16_t DMA_TIME = 0;
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */



extern void Delay_Handler(void);
extern void UartMoni_Handler(void); 
void SysTick_Handler(void)
{
	Delay_Handler();
}

#ifdef __KEYUP_IRQHandler
//#include "key_exti.h"
//#include "led.h"
//#include "sysTick.h"
void KEYUP_IRQHandler()
{
	  //È·±£ÊÇ·ñ²úÉúÁËEXTI LineÖÐ¶Ï
	if(EXTI_GetITStatus(KEYUP_INT_EXTI_LINE) != RESET) 
	{

    //Çå³ýÖÐ¶Ï±êÖ¾Î»
		EXTI_ClearITPendingBit(KEYUP_INT_EXTI_LINE);     
	}  
}
#endif


#ifdef __KEY0_IRQHandler
//#include "key_exti.h"
//#include "led.h"
//#include "sysTick.h"
void KEY0_IRQHandler()
{
	  //È·±£ÊÇ·ñ²úÉúÁËEXTI LineÖÐ¶Ï
	if(EXTI_GetITStatus(KEY0_INT_EXTI_LINE) != RESET) 
	{

    //Çå³ýÖÐ¶Ï±êÖ¾Î»
		EXTI_ClearITPendingBit(KEY0_INT_EXTI_LINE);     
	}  
}
#endif


#ifdef   __KEY1_IRQHandler
//#include "key_exti.h"
void KEY1_IRQHandler()
{
	  //È·±£ÊÇ·ñ²úÉúÁËEXTI LineÖÐ¶Ï
	if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET) 
	{
		
		
		
    //Çå³ýÖÐ¶Ï±êÖ¾Î»
		EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);     
	}  
}
#endif


#ifdef   __KEY2_IRQHandler
//#include "key_exti.h"
void KEY2_IRQHandler()
{
	  //È·±£ÊÇ·ñ²úÉúÁËEXTI LineÖÐ¶Ï
	if(EXTI_GetITStatus(KEY2_INT_EXTI_LINE) != RESET) 
	{
		
		
    //Çå³ýÖÐ¶Ï±êÖ¾Î»
		EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);     
	}  
}
#endif



// ´®¿ÚÖÐ¶Ï·þÎñº¯Êý
#ifdef   __USART_IRQHandler
//#include "usart.h"
uint8_t ucTemp;
void USART_IRQHandler(void)
{
	if(USART_GetITStatus(USARTx,USART_IT_RXNE)!=RESET)
	{		
		ucTemp = USART_ReceiveData(USARTx);//¶ÁÈ¡DR¼Ä´æÆ÷ÀïµÄÊý¾ÝÊ±£¬RXNEÓ²¼þ×Ô¶¯ÇåÁã
    USART_SendData(USARTx,ucTemp);    
	}	 
}
#endif



#ifdef __DMP_EXTI_IRQHandler 
//extern void gyro_data_ready_cb(void);
//extern u8 dmp_getdata(void);
extern float pit,rol,yaw;						
//#include "usart.h"
void DMP_EXTI_IRQHandler (void)
{
	if(EXTI_GetITStatus(EXTI_Line4))
	{
		LED1_TOGGLE;
		gyro_data_ready_cb();
		dmp_getdata();
		printf("pit = %f\nrol = %f\nyaw = %f\n",pit,rol,yaw);
		
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}
#endif


#ifdef  __TIM6_IRQHandler
//#include "led.h"
volatile uint16_t i = 50;
uint16_t Time = 800;
uint16_t ZKB = 50;
uint8_t Direction = 1;
void TIM6_IRQHandler(void)
{
	if ( TIM_GetITStatus( TIM6, TIM_IT_Update) != RESET ) 
	{	
		i++;
		if(i==ZKB)
		{
			LED_ALL_ON;
		}
		if(i==Time)
		{
			LED_ALL_OFF;
			i=50;
			if(Direction==1)
			{
				ZKB++;
				if(ZKB==800)
					Direction = 2;
			}
			if(Direction==2)
			{
				ZKB--;
				if(ZKB==50)
					Direction = 1;
			}
		}
		TIM_ClearITPendingBit(TIM6 , TIM_FLAG_Update);  		 
	}		 	

}
#endif




#ifdef __TIM1_CC_IRQHandler
__IO uint16_t IC2Value = 0;
__IO uint16_t IC1Value = 0;
__IO float DutyCycle = 0;
__IO float Frequency = 0;
//#include "advance_tim.h"
//#include "usart.h"
//#include "led.h"
/*
 * Èç¹ûÊÇµÚÒ»¸öÉÏÉýÑØÖÐ¶Ï£¬¼ÆÊýÆ÷»á±»¸´Î»£¬Ëø´æµ½CCR1¼Ä´æÆ÷µÄÖµÊÇ0£¬CCR2¼Ä´æÆ÷µÄÖµÒ²ÊÇ0
 * ÎÞ·¨¼ÆËãÆµÂÊºÍÕ¼¿Õ±È¡£µ±µÚ¶þ´ÎÉÏÉýÑØµ½À´µÄÊ±ºò£¬CCR1ºÍCCR2²¶»ñµ½µÄ²ÅÊÇÓÐÐ§µÄÖµ¡£ÆäÖÐ
 * CCR1¶ÔÓ¦µÄÊÇÖÜÆÚ£¬CCR2¶ÔÓ¦µÄÊÇÕ¼¿Õ±È¡£
 */
void ADVANCE_TIM_IRQHandler(void)
{
  /* Çå³ýÖÐ¶Ï±êÖ¾Î» */
  TIM_ClearITPendingBit(ADVANCE_TIM, TIM_IT_CC1);

  /* »ñÈ¡ÊäÈë²¶»ñÖµ */
  IC1Value = TIM_GetCapture1(ADVANCE_TIM);
  IC2Value = TIM_GetCapture2(ADVANCE_TIM);
	
  // ×¢Òâ£º²¶»ñ¼Ä´æÆ÷CCR1ºÍCCR2µÄÖµÔÚ¼ÆËãÕ¼¿Õ±ÈºÍÆµÂÊµÄÊ±ºò±ØÐë¼Ó1
	if (IC1Value != 0)
  {
    /* Õ¼¿Õ±È¼ÆËã */
    DutyCycle = (float)((IC2Value+1) * 100) / (IC1Value+1);

    /* ÆµÂÊ¼ÆËã */
    Frequency = (72000000/(ADVANCE_TIM_PSC+1))/(float)(IC1Value+1);
		printf("Õ¼¿Õ±È£º%0.2f%%   ÆµÂÊ£º%0.2fHz\n",DutyCycle,Frequency);
		LED1_ON;
		LED2_OFF;
  }
  else
  {
    DutyCycle = 0;
    Frequency = 0;
  }
}
#endif

#ifdef   __TIM1_UP_IRQHandler
volatile uint32_t time=0;
/**
  * @brief  This function handles TIM2 interrupt request.
  * @param  None
  * @retval None
  */
void  ADVANCE_TIM_IRQHandler (void)
{
	if ( TIM_GetITStatus( ADVANCE_TIM, TIM_IT_Update) != RESET ) 
	{	
		time++;
		TIM_ClearITPendingBit(ADVANCE_TIM , TIM_FLAG_Update);  		 
	}		 	
}
	
#endif



#ifdef __TIM2_IRQHandler
#ifdef GENERAL_TIM_Time
volatile uint32_t time=0;
/**
  * @brief  This function handles TIM2 interrupt request.
  * @param  None
  * @retval None
  */
void  TIM2_IRQHandler (void)
{
	if ( TIM_GetITStatus( GENERAL_TIM, TIM_IT_Update) != RESET ) 
	{	
		time++;
		TIM_ClearITPendingBit(GENERAL_TIM , TIM_FLAG_Update);  		 
	}		 	
}

#elif defined GENERAL_TIM_ICPULSE

void TIM5_IRQHandler(void)
{
	// µ±Òª±»²¶»ñµÄÐÅºÅµÄÖÜÆÚ´óÓÚ¶¨Ê±Æ÷µÄ×î³¤¶¨Ê±Ê±£¬¶¨Ê±Æ÷¾Í»áÒç³ö£¬²úÉú¸üÐÂÖÐ¶Ï
	// Õâ¸öÊ±ºòÎÒÃÇÐèÒª°ÑÕâ¸ö×î³¤µÄ¶¨Ê±ÖÜÆÚ¼Óµ½²¶»ñÐÅºÅµÄÊ±¼äÀïÃæÈ¥
	if ( TIM_GetITStatus ( GENERAL_TIM, TIM_IT_Update) != RESET )               
	{	
		TIM_ICUserValueStructure.Capture_Period ++;		
		TIM_ClearITPendingBit ( GENERAL_TIM, TIM_FLAG_Update ); 		
	}

	// ÉÏÉýÑØ²¶»ñÖÐ¶Ï
	if ( TIM_GetITStatus (GENERAL_TIM, GENERAL_TIM_IT_CCx ) != RESET)
	{
		// µÚÒ»´Î²¶»ñ
		if ( TIM_ICUserValueStructure.Capture_StartFlag == 0 )
		{
			// ¼ÆÊýÆ÷Çå0
			TIM_SetCounter ( GENERAL_TIM, 0 );
			// ×Ô¶¯ÖØ×°ÔØ¼Ä´æÆ÷¸üÐÂ±êÖ¾Çå0
			TIM_ICUserValueStructure.Capture_Period = 0;
      // ´æ²¶»ñ±È½Ï¼Ä´æÆ÷µÄÖµµÄ±äÁ¿µÄÖµÇå0			
			TIM_ICUserValueStructure.Capture_CcrValue = 0;

			// µ±µÚÒ»´Î²¶»ñµ½ÉÏÉýÑØÖ®ºó£¬¾Í°Ñ²¶»ñ±ßÑØÅäÖÃÎªÏÂ½µÑØ
			GENERAL_TIM_OCxPolarityConfig_FUN(GENERAL_TIM, TIM_ICPolarity_Falling);
      // ¿ªÊ¼²¶»ñ±ê×¼ÖÃ1			
			TIM_ICUserValueStructure.Capture_StartFlag = 1;			
		}
		// ÏÂ½µÑØ²¶»ñÖÐ¶Ï
		else // µÚ¶þ´Î²¶»ñ
		{
			// »ñÈ¡²¶»ñ±È½Ï¼Ä´æÆ÷µÄÖµ£¬Õâ¸öÖµ¾ÍÊÇ²¶»ñµ½µÄ¸ßµçÆ½µÄÊ±¼äµÄÖµ
			TIM_ICUserValueStructure.Capture_CcrValue = 
			GENERAL_TIM_GetCapturex_FUN (GENERAL_TIM);

			// µ±µÚ¶þ´Î²¶»ñµ½ÏÂ½µÑØÖ®ºó£¬¾Í°Ñ²¶»ñ±ßÑØÅäÖÃÎªÉÏÉýÑØ£¬ºÃ¿ªÆôÐÂµÄÒ»ÂÖ²¶»ñ
			GENERAL_TIM_OCxPolarityConfig_FUN(GENERAL_TIM, TIM_ICPolarity_Rising);
      // ¿ªÊ¼²¶»ñ±êÖ¾Çå0		
			TIM_ICUserValueStructure.Capture_StartFlag = 0;
      // ²¶»ñÍê³É±êÖ¾ÖÃ1			
			TIM_ICUserValueStructure.Capture_FinishFlag = 1;		
		}

		TIM_ClearITPendingBit (GENERAL_TIM,GENERAL_TIM_IT_CCx);	    
	}		
}
#endif
#endif

#ifdef __OV7725_VSYNC_EXTI_INT_FUNCTION 
extern uint8_t Ov7725_vsync;
void OV7725_VSYNC_EXTI_INT_FUNCTION ( void )
{
    if ( EXTI_GetITStatus(OV7725_VSYNC_EXTI_LINE) != RESET ) 	//¼ì²éEXTI_Line4ÏßÂ·ÉÏµÄÖÐ¶ÏÇëÇóÊÇ·ñ·¢ËÍµ½ÁËNVIC 
    {
        if( Ov7725_vsync == 0 )
        {
            FIFO_WRST_L(); 	                      //À­µÍÊ¹FIFOÐ´(Êý¾ÝfromÉãÏñÍ·)Ö¸Õë¸´Î
					  FIFO_WRST_H();                        //ÔÊÐíÊ¹FIFOÐ´(Êý¾ÝfromÉãÏñÍ·)Ö¸ÕëÔË¶¯»
            FIFO_WE_H();	                        //À­¸ßÊ¹FIFOÐ´ÔÊÐí
            
            Ov7725_vsync = 1;	   	
            FIFO_WE_H();                          //Ê¹FIFOÐ´ÔÊÐí
            
        }
        else if( Ov7725_vsync == 1 )
        {
            FIFO_WE_L();                          //À­µÍÊ¹FIFOÐ´ÔÝÍ£
						FIFO_WE_L();                          //À­µÍÊ¹FIFOÐ´ÔÝÍ£
						FIFO_WRST_L(); 	                      //À­µÍÊ¹FIFOÐ´(Êý¾ÝfromÉãÏñÍ·)Ö¸Õë¸´Î»
            Ov7725_vsync = 2;
        }        
        EXTI_ClearITPendingBit(OV7725_VSYNC_EXTI_LINE);		    //Çå³ýEXTI_Line4ÏßÂ·¹ÒÆð±êÖ¾Î»        
    }    
}
#endif


//Ó¥ÑÛ³¡ÖÐ¶Ï
#ifdef __EAGLE_VSYNC_EXTI_INT_FUNCTION
extern volatile IMG_STATUS_e      OV7725_Eagle_img_flag;   
void EAGLE_VSYNC_EXTI_INT_FUNCTION(void)
{
	 if ( EXTI_GetITStatus(EAGLE_VSYNC_EXTI_LINE) != RESET )
	 {
		// printf("³¡ÖÐ¶Ï");
		 
		  if(OV7725_Eagle_img_flag == IMG_START)                 
			{
				LED0_ON;
					OV7725_Eagle_img_flag = IMG_GATHER;             
					NVIC_DisableIRQ(EAGLE_VSYNC_EXTI_IRQ);
				
					NVIC_EnableIRQ(EAGLE_RCLK_EXTI_IRQ);		//Ê¹ÄÜRCLK´¥·¢DMA¿ªÊ¼ÖÐ¶Ï                      
			}
			else                                      
			{
					NVIC_DisableIRQ(EAGLE_VSYNC_EXTI_IRQ);                      
					OV7725_Eagle_img_flag = IMG_FAIL;                   
			}
			 
		 
		  EXTI_ClearITPendingBit(EAGLE_VSYNC_EXTI_LINE);//ÇåÖÐ¶Ï±êÖ¾Î»
	 }
}
#endif

#ifdef __EAGLE_RCLK_EXTI_INT_FUNCTION
extern  u8   *OV7725_Eagle_img_buff;
extern  u8 imgbuff[CAMERA_SIZE]; 
void EAGLE_RCLK_EXTI_INT_FUNCTION(void)
{
	if ( EXTI_GetITStatus(EAGLE_RCLK_EXTI_LINE) != RESET )
	{	
		DMA_TIME++;
	
	
	DMA_Cmd(DMA1_Channel6,DISABLE);
	Clear_DMA_FINISH_Flag ;
	DMA_SetCurrDataCounter(DMA1_Channel6,1);//DMAÍ¨µÀµÄDMA»º´æµÄ´óÐ¡
	DMA1_Channel6->CPAR = (u32)(imgbuff+DMA_TIME-1);
//	OV7725_Eagle_img_buff++;
	DMA_Cmd(DMA1_Channel6,ENABLE);
//	Enable_DMA_FINISH_IRQ	;	
		
		if(DMA_TIME == 600)
	{
		DMA_TIME=0;
		NVIC_DisableIRQ(EAGLE_RCLK_EXTI_IRQ);
		OV7725_Eagle_img_flag = IMG_FINISH ;
		LED3_TOGGLE;
	}
	
	EXTI_ClearITPendingBit(EAGLE_RCLK_EXTI_LINE);//ÇåÖÐ¶Ï±êÖ¾Î»
	}
}
#endif


#ifdef  __EAGLE_DMA_Handler 
extern uint16_t DMA_TIME;
extern volatile IMG_STATUS_e      OV7725_Eagle_img_flag; 
//extern uint8_t flag;
void EAGLE_DMA_Handler (void)
{
	//printf("DMAÍê³É");
	//LED2_ON;
 
	//Disable_DMA_FINISH_IRQ;
//	NVIC_EnableIRQ(EAGLE_RCLK_EXTI_IRQ);		//Ê¹ÄÜRCLK´¥·¢DMA¿ªÊ¼ÖÐ¶Ï 
 // DMA_ClearFlag(DMA1_FLAG_TC6);	
	
}
#endif


#ifdef __DMA_Handler
extern uint8_t aDST_Buffer[BUFFER_SIZE];
void DMA_Handler ()
{
	 u8 i;
	// µÈ´ýDMA´«ÊäÍê³É 
   printf("aDST_Buffer: ");
		for(i=0;i<8;_i++)
		printf("%d ",aDST_Buffer[i]);

//	Disable_DMA_IRQ;
	Clear_DMA_Flag ;
	DMA_ClearFlag(DMA_FLAG_TC);

	//printf("´«ÊäÍê³ÉÖÐ¶Ï´¥·¢");
}
#endif

#ifdef __NRF_EXTI_IRQHANDLER
void NRF_EXTI_IRQHandler(void)
{	
	if ( EXTI_GetITStatus(EAGLE_RCLK_EXTI_LINE) != RESET )
	{
		NRF24L01_CE = 0;	
		
		NRF_ReceivePacket();	
	
//	sta = SPI_Read_Byte(READ_REG_CMD+STATUS);				
//	if(sta & TX_DS){									
//		RX_Mode();											
//	}else if(sta & RX_DR){
//			 NRF_ReceivePacket();	
//	if(buttonFlag & 0x01){
//			LED1_ON;								
//	}else{
//		LED0_TOGGLE;								
//    }
//	}else if(sta & MAX_RT){									
//		RX_Mode();											
//		SPI_Write_Byte(WRITE_REG_CMD+STATUS,sta);		
//	}
	
		EXTI_ClearITPendingBit(NRF_EXTI_Line);
	}
}
#endif

#ifdef __ADC_IRQHANDLER
void ADC_IRQHandler(void)
{	
	if (ADC_GetITStatus(ADCx,ADC_IT_EOC)==SET) 
	{
		// ¶ÁÈ¡ADCµÄ×ª»»Öµ
		ADC_ConvertedValue = ADC_GetConversionValue(ADCx);
	}
	ADC_ClearITPendingBit(ADCx,ADC_IT_EOC);
}
#endif

#ifdef __WWDG_IRQHANDLER
// WWDG ÖÐ¶Ï¸´·þÎñ³ÌÐò£¬Èç¹û·¢ÉúÁË´ËÖÐ¶Ï£¬±íÊ¾³ÌÐòÒÑ¾­³öÏÖÁË¹ÊÕÏ£¬
// ÕâÊÇÒ»¸öËÀÇ°ÖÐ¶Ï¡£ÔÚ´ËÖÐ¶Ï·þÎñ³ÌÐòÖÐÓ¦¸Ã¸É×îÖØÒªµÄÊÂ£¬
// ±ÈÈç±£´æÖØÒªµÄÊý¾ÝµÈ£¬Õâ¸öÊ±¼ä¾ßÌåÓÐ¶à³¤£¬Òª
// ÓÉWDGTBµÄÖµ¾ö¶¨£º
// WDGTB:0   113us
// WDGTB:1   227us
// WDGTB:2   455us
// WDGTB:3   910us
void WWDG_IRQHandler(void)
{
	// Çå³ýÖÐ¶Ï±êÖ¾Î»
	WWDG_ClearFlag();
	
	//LED2ÁÁ£¬µãÁÁLEDÖ»ÊÇÊ¾ÒâÐÔµÄ²Ù×÷£¬
	//ÕæÕýÊ¹ÓÃµÄÊ±ºò£¬ÕâÀïÓ¦¸ÃÊÇ×ö×îÖØÒªµÄÊÂÇé
	LED2(ON); 
}
#endif

#ifdef __PVD_IRQHANDLER
void PVD_IRQHandler(void)
{
		/*¼ì²âÊÇ·ñ²úÉúÁËPVD¾¯¸æÐÅºÅ*/
		if(PWR_GetFlagStatus (PWR_FLAG_PVDO)==SET)			
		{
			/* ÁÁµÆ£¬Êµ¼ÊÓ¦ÓÃÖÐÓ¦½øÈë½ô¼±×´Ì¬´¦Àí */
			LED1_ON; 
			
		}
    /* Çå³ýÖÐ¶ÏÐÅºÅ*/
    EXTI_ClearITPendingBit(EXTI_Line16);
}
#endif

#ifdef __RTC_IRQHANDLER
void RTC_IRQHandler()	//RTCÊµÊ±Ê±ÖÓ
{
	if(RTC_GetITStatus((RTC_IT_SEC))!=RESET)
	{
		RTC_ClearITPendingBit(RTC_IT_SEC);
		timebz=1;	
	}
	/*ÄÖÖÓ*/
	  if (RTC_GetITStatus(RTC_IT_ALR) != RESET)
		{
			timeclock  = 1 ;
		}
		
		RTC_ClearITPendingBit(RTC_IT_ALR|RTC_IT_SEC);
}
#endif

/*
void EXTI3_IRQHandler(void)
{
	 NRF_IRQHandler();
}
*/

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
