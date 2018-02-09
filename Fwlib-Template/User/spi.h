#ifndef __SPI_H
#define __SPI_H


#include "stm32f10x.h"


/* 定义全局变量 */
void SPI2_Conf(void);
void SPI2_SetSpeed(uint8_t Speed);
uint8_t SPI2_WriteReadData(uint8_t dat);

void SPI1_Conf(void);
void SPI1_SetSpeed(uint8_t speed);
uint8_t SPI1_WriteReadData(uint8_t dat);

u8 SPI_Write_Byte(u8 reg, u8 value);
u8 SPI_Write_Buf(u8 reg, u8 *pBuf, u8 len);
u8 SPI_Read_Byte(u8 reg);
u8 SPI_Read_Buf(u8 reg, u8 *pBuf, u8 len);











#endif
