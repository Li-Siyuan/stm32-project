/*********188开发板专用硬件iic函数****************
**********作者：李思远******************
**********参考：秉火******************
**********新增：改为通用硬件IIC，脱离eeprom***********/

#include "i2c_hard.h"
#include "usart.h"	

static __IO uint32_t  I2CTimeout = I2CT_LONG_TIMEOUT;   

/**
  * @brief  Basic management of the timeout situation.
  * @param  errorCode：错误代码，可以用来定位是哪个环节出错.
  * @retval 返回0，表示IIC读取失败.
  */
static  uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
{
  I2C_ERROR("I2C 等待超时!errorCode = %d",errorCode);
  return 0;
}



/**
  * @brief  I2C I/O配置
  * @param  无
  * @retval 无
  */
static void I2C_GPIO_Conf(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 

	/* 使能与 I2C 有关的时钟 */
	RCC_APB1PeriphClockCmd( I2C_CLK, ENABLE );
	RCC_APB2PeriphClockCmd( I2C_GPIO_CLK, ENABLE );
	
    
  /* I2C_SCL、I2C_SDA*/
  GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	       // 开漏输出
  GPIO_Init(I2C_SCL_PORT, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	       // 开漏输出
  GPIO_Init(I2C_SDA_PORT, &GPIO_InitStructure);		
}



/**
  * @brief  I2C 工作模式配置
  * @param  无
  * @retval 无
  */
static void I2C_Mode_Conf(void)
{
  I2C_InitTypeDef  I2C_InitStructure; 

  /* I2C 配置 */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	
	/* 高电平数据稳定，低电平数据变化 SCL 时钟线的占空比 */
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	
  I2C_InitStructure.I2C_OwnAddress1 =I2Cx_OWN_ADDRESS7; 
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
	 
	/* I2C的寻址模式 */
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	
	/* 通信速率 */
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
  
	/* I2C 初始化 */
  I2C_Init(I2Cx_Hard, &I2C_InitStructure);
  
	/* 使能 I2C */
  I2C_Cmd(I2Cx_Hard, ENABLE);   
}



void I2C_Hard_Conf(void)
{
  I2C_GPIO_Conf();  
  I2C_Mode_Conf();
}


/*
  官方库函数
	void I2C_GenerateSTART(I2C_TypeDef* I2Cx, FunctionalState NewState);
	void I2C_GenerateSTOP(I2C_TypeDef* I2Cx, FunctionalState NewState);
  void I2C_AcknowledgeConfig(I2C_TypeDef* I2Cx, FunctionalState NewState);
  void I2C_SendData(I2C_TypeDef* I2Cx, uint8_t Data);
	uint8_t I2C_ReceiveData(I2C_TypeDef* I2Cx);
	void I2C_Send7bitAddress(I2C_TypeDef* I2Cx, uint8_t Address, uint8_t I2C_Direction);
	uint16_t I2C_ReadRegister(I2C_TypeDef* I2Cx, uint8_t I2C_Register);
	FlagStatus I2C_GetFlagStatus(I2C_TypeDef* I2Cx, uint32_t I2C_FLAG);
*/



uint32_t I2C_ByteWrite_ToRegister(u8* pBuffer, uint16_t ADDR ,u8 WriteAddr) 
{
	//检测总线是否空闲
  while(I2C_GetFlagStatus(I2Cx_Hard, I2C_FLAG_BUSY))   
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(0);
  } 
	
	//开始I2C，并检测EV5
	I2CTimeout = I2CT_LONG_TIMEOUT;
	I2C_GenerateSTART(I2Cx_Hard, ENABLE);
	while(!I2C_CheckEvent(I2Cx_Hard, I2C_EVENT_MASTER_MODE_SELECT))  
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(0);
  } 
	
	
	//发送设备地址并检测EV6
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	I2C_Send7bitAddress(I2Cx_Hard, ADDR, I2C_Direction_Transmitter);
  while(!I2C_CheckEvent(I2Cx_Hard, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(1);
  }  


	//发送写入寄存器地址并检测EV8
	 I2CTimeout = I2CT_FLAG_TIMEOUT;
	 I2C_SendData(I2Cx_Hard, WriteAddr);   
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2Cx_Hard, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(2);
  } 
	
	 //发送要写入的数据并检测EV8
	I2CTimeout = I2CT_FLAG_TIMEOUT;  
  I2C_SendData(I2Cx_Hard, *pBuffer);  
  while(!I2C_CheckEvent(I2Cx_Hard, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(3);
  } 
  
  //I2C停止
  I2C_GenerateSTOP(I2Cx_Hard, ENABLE);
  
  return 1;
}
	

/**
  * @brief   从设备里面读取一块数据 
  * @param   
  *		@arg pBuffer:存放从设备读取的数据的缓冲区指针
  *		@arg WriteAddr:接收数据的设备的地址
  *     @arg NumByteToWrite:要从设备读取的字节数
  * @retval  无
  */
uint32_t I2C_Read_FromRegister(u8* pBuffer, uint16_t ADDR,u8 ReadAddr, u16 NumByteToRead)
{  
	I2CTimeout = I2CT_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(I2Cx_Hard, I2C_FLAG_BUSY))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(9);
   }
  
  //开始I2C并检测EV5
	I2CTimeout = I2CT_LONG_TIMEOUT;
  I2C_GenerateSTART(I2Cx_Hard, ENABLE);  
  while(!I2C_CheckEvent(I2Cx_Hard, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(10);
   }
 
	 
  //发送设备地址并检测EV6
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  I2C_Send7bitAddress(I2Cx_Hard, ADDR, I2C_Direction_Transmitter);
  while(!I2C_CheckEvent(I2Cx_Hard, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(11);
   }
    
  /* Clear EV6 by setting again the PE bit */
  I2C_Cmd(I2Cx_Hard, ENABLE);


  //发送要读的寄存器地址并检测EV8	 
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  I2C_SendData(I2Cx_Hard, ReadAddr);   
  while(!I2C_CheckEvent(I2Cx_Hard, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(12);
   }
 
   
 //I2C开始并检测EV5 
	I2CTimeout = I2CT_FLAG_TIMEOUT;
  I2C_GenerateSTART(I2Cx_Hard, ENABLE);  
  while(!I2C_CheckEvent(I2Cx_Hard, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(13);
   }
    
  //发送设备地址与读方向并检测EV6	   
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  I2C_Send7bitAddress(I2Cx_Hard, ADDR, I2C_Direction_Receiver);
  while(!I2C_CheckEvent(I2Cx_Hard, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(14);
   }
  
  //读取多字节数据
  while(NumByteToRead)  
  {
    if(NumByteToRead == 1)
    {
      //失能应答
      I2C_AcknowledgeConfig(I2Cx_Hard, DISABLE);
      
      //I2C停止
      I2C_GenerateSTOP(I2Cx_Hard, ENABLE);
    }

    //检测EV7   
    I2CTimeout = I2CT_LONG_TIMEOUT;   
		while(I2C_CheckEvent(I2Cx_Hard, I2C_EVENT_MASTER_BYTE_RECEIVED)==0)  
		{
			if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(3);
		} 
    {      
      //读一个字节
      *pBuffer = I2C_ReceiveData(I2Cx_Hard);
      pBuffer++; 
      NumByteToRead--;        
    }   
  }

  //使能应答以便下次读取
  I2C_AcknowledgeConfig(I2Cx_Hard, ENABLE);
  
    return 1;
}

	


