#ifndef __EEPROM_I2CMONI_H
#define	__EEPROM_I2CMONI_H


#include "stm32f10x.h"



/* AT24C01/02每页有8个字节 */
#define I2C_PageSize           8

/* AT24C04/08A/16A每页有16个字节 */
//#define I2C_PageSize           16	

/*信息输出*/
#define EEPROM_DEBUG_ON         0

/* 
 * AT24C02 2kb = 2048bit = 2048/8 B = 256 B
 * 32 pages of 8 bytes each
 *
 * Device Address
 * 1 0 1 0 A2 A1 A0 R/W
 * 1 0 1 0 0  0  0  0 = 0XA0
 * 1 0 1 0 0  0  0  1 = 0XA1 
 */

/* EEPROM Addresses defines */
#define EEPROM_Block0_ADDRESS 0xA0   /* E2 = 0 */
//#define EEPROM_Block1_ADDRESS 0xA2 /* E2 = 0 */
//#define EEPROM_Block2_ADDRESS 0xA4 /* E2 = 0 */
//#define EEPROM_Block3_ADDRESS 0xA6 /* E2 = 0 */



void EEPROM_Conf(void);
void EEPROM_I2CMoni_WaitEepromStandbyState(void);
uint32_t EEPROM_I2CMoni_ByteWrite(u8* pBuffer, u8 WriteAddr);
uint32_t EEPROM_I2CMoni_BytesWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite);
uint32_t EEPROM_I2CMoni_PageWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite);
void EEPROM_I2CMoni_BufferWrite(u8* pBuffer, u8 WriteAddr, u16 NumByteToWrite);
uint32_t I2C_EE_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead);


#endif


