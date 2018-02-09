/*
 *
 **********模拟uart函数************************
 **********作者：李思远************************
 **********自网络匿名51模拟串口例程改编********
 **********波特率自定，8位数据位，无校验位*****
 *
*/


#ifndef __UARTMONI_H
#define __UARTMONI_H

#include "stm32f10x.h"

#define TX_GPIO_PORT      GPIOC
#define TX_GPIO_CLK       RCC_APB2Periph_GPIOC
#define TX_GPIO_Pin       GPIO_Pin_0

#define RX_GPIO_PORT      GPIOC
#define RX_GPIO_CLK       RCC_APB2Periph_GPIOC
#define RX_GPIO_Pin       GPIO_Pin_1

void UartMoni_Conf(uint32_t buad);
void StartRxd(void);
void StartTxd(uint8_t data);
void UartMoni_Handler(void);	
void TxdData(uint8_t data);
void TxdArray( uint8_t *array, uint16_t num);
void TxdString(char *str);
void TxdHalfWord(uint16_t ch);
uint8_t RxdData(void);
void RxdArray( uint8_t *array, uint16_t num);
void RxdString(char *str);
void RxdHalfWord(uint16_t *ch);
#if 0
#include "stdio.h"
int fputc(int ch, FILE *f);
int fgetc(FILE *f);
#endif

#endif

