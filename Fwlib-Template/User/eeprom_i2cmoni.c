/*********188开发板专用eeprom读写函数****************
**********作者：李思远******************
**********参考：秉火******************
**********新增：用模拟I2C实现读写EEPROM功能***************/

#include "eeprom_i2cmoni.h"
#include "i2c_moni.h"



uint16_t EEPROM_ADDRESS;

void EEPROM_Conf(void)
{
	/* 根据头文件i2c_ee.h中的定义来选择EEPROM的设备地址 */
#ifdef EEPROM_Block0_ADDRESS
  /* 选择 EEPROM Block0 来写入 */
  EEPROM_ADDRESS = EEPROM_Block0_ADDRESS;
#endif

#ifdef EEPROM_Block1_ADDRESS  
	/* 选择 EEPROM Block1 来写入 */
  EEPROM_ADDRESS = EEPROM_Block1_ADDRESS;
#endif

#ifdef EEPROM_Block2_ADDRESS  
	/* 选择 EEPROM Block2 来写入 */
  EEPROM_ADDRESS = EEPROM_Block2_ADDRESS;
#endif

#ifdef EEPROM_Block3_ADDRESS  
	/* 选择 EEPROM Block3 来写入 */
  EEPROM_ADDRESS = EEPROM_Block3_ADDRESS;
#endif
}



/**
  * @brief  Wait for EEPROM Standby state 
  * @param  无
  * @retval 无
  */
void EEPROM_I2CMoni_WaitEepromStandbyState(void)      
{
	FunctionalState state;							
  //一直发送起始信号，直到产生应答
  do
  {
    I2C_Start(); 
    state = MoniI2C_WriteByte(EEPROM_ADDRESS<<1|0);   
	}while(state!=ENABLE);
I2C_Stop();    			
}




/**
  * @brief   写一个字节到I2C EEPROM中
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址 
  * @retval  无
  */
uint32_t EEPROM_I2CMoni_ByteWrite(u8* pBuffer, u8 WriteAddr) 
{
  MoniI2c_WriteByteToSlave(EEPROM_ADDRESS, WriteAddr,* pBuffer);
  return 1;
}




/**
  * @brief   写多个字节到I2C EEPROM中
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址 
  *   @arg NumByteToWrite:写的字节数
  * @retval  无
  */
uint32_t EEPROM_I2CMoni_BytesWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite) 
{
	uint16_t i;
	uint8_t res;
	for(i=0;i<NumByteToWrite;i++)
	{
		EEPROM_I2CMoni_WaitEepromStandbyState();
		res = MoniI2c_WriteByteToSlave(EEPROM_ADDRESS, WriteAddr,* pBuffer);
	}
	return res;
}




//为快速发送数据，页发送，不能用i2c写寄存器函数
uint32_t EEPROM_I2CMoni_PageWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite)
{
	MoniI2c_WriteSomeDataToSlave(EEPROM_ADDRESS,WriteAddr,NumByteToWrite, pBuffer);
	return 0;
}


/**
  * @brief   将缓冲区中的数据写到I2C EEPROM中
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址
  *     @arg NumByteToWrite:写的字节数
  * @retval  无
  */
void EEPROM_I2CMoni_BufferWrite(u8* pBuffer, u8 WriteAddr, u16 NumByteToWrite)
{
	u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;

  Addr = WriteAddr % I2C_PageSize;
  count = I2C_PageSize - Addr;
  NumOfPage =  NumByteToWrite / I2C_PageSize;
  NumOfSingle = NumByteToWrite % I2C_PageSize;
	
	/* If WriteAddr is I2C_PageSize aligned  */
  if(Addr == 0) 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage == 0) 
    {
      EEPROM_I2CMoni_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      EEPROM_I2CMoni_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > I2C_PageSize */
    else  
    {
      while(NumOfPage--)
      {
        EEPROM_I2CMoni_PageWrite(pBuffer, WriteAddr, I2C_PageSize); 
    	  EEPROM_I2CMoni_WaitEepromStandbyState();
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;
      }

      if(NumOfSingle!=0)
      {
        EEPROM_I2CMoni_PageWrite(pBuffer, WriteAddr, NumOfSingle);
        EEPROM_I2CMoni_WaitEepromStandbyState();
      }
    }
  }
  /* If WriteAddr is not I2C_PageSize aligned  */
  else 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage== 0) 
    {
      EEPROM_I2CMoni_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      EEPROM_I2CMoni_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > I2C_PageSize */
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / I2C_PageSize;
      NumOfSingle = NumByteToWrite % I2C_PageSize;	
      
      if(count != 0)
      {  
        EEPROM_I2CMoni_PageWrite(pBuffer, WriteAddr, count);
        EEPROM_I2CMoni_WaitEepromStandbyState();
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(NumOfPage--)
      {
        EEPROM_I2CMoni_PageWrite(pBuffer, WriteAddr, I2C_PageSize);
        EEPROM_I2CMoni_WaitEepromStandbyState();
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;  
      }
      if(NumOfSingle != 0)
      {
        EEPROM_I2CMoni_PageWrite(pBuffer, WriteAddr, NumOfSingle); 
        EEPROM_I2CMoni_WaitEepromStandbyState();
      }
    }
  }  
}

/**
  * @brief   从EEPROM里面读取一块数据 
  * @param   
  *		@arg pBuffer:存放从EEPROM读取的数据的缓冲区指针
  *		@arg WriteAddr:接收数据的EEPROM的地址
  *     @arg NumByteToWrite:要从EEPROM读取的字节数
  * @retval  无
  */
uint32_t I2C_EE_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead)
{  
  MoniI2c_ReadSomeDataFromSlave(EEPROM_ADDRESS,ReadAddr,NumByteToRead, pBuffer);
	return 1;
}




