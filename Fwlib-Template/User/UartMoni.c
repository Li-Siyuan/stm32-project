/*
 *
 **********模拟uart函数************************
 **********作者：李思远************************
 **********自网络匿名51模拟串口例程改编********
 **********波特率自定，8位数据位，无校验位*****
 *
*/
#include "UartMoni.h"
#include "systick.h"

static uint32_t Buad; //存储波特率，方便接收第一次中断改变周期

uint8_t RxdOrTxd = 0;  //判断当前状态为空闲0发送1还是接收2
uint8_t RxdEnd = 0;    //接受结束标志
uint8_t TxdEnd = 0;    //发送结束标志
uint8_t TxdBuf = 0;    //发送缓冲区
uint8_t RxdBuf = 0;    //接收缓冲区


//配置模拟uart，初始化TX与RX的GPIO，初始化SysTick定时器
void UartMoni_Conf(uint32_t buad)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	Buad = buad; //存储波特率
	
	//配置RX_GPIO为浮空输入
	RCC_APB2PeriphClockCmd(TX_GPIO_CLK|RX_GPIO_CLK,ENABLE);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = TX_GPIO_Pin;
	GPIO_Init(TX_GPIO_PORT,&GPIO_InitStructure);
	//配置TX_GPIO为推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = RX_GPIO_Pin;
	GPIO_Init(TX_GPIO_PORT,&GPIO_InitStructure);
	GPIO_ResetBits(TX_GPIO_PORT,TX_GPIO_Pin);  //拉高电平
	//配置滴答定时器
	//SysTick_Conf(72000000*Buad);
}


//开始接收函数
void StartRxd(void)
{
	while(GPIO_ReadInputDataBit(RX_GPIO_PORT, RX_GPIO_Pin));    //等到开始位（低电平）
	SysTick->LOAD  = ((144*Buad)& SysTick_LOAD_RELOAD_Msk) - 1;   //发送时第一次周期变为原来的一半
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;  //使能滴答定时器
	RxdEnd = 0;     //清零接收结束标志位
  RxdOrTxd = 2;   //标志接收中
}



//等待接收函数
uint8_t RxdData(void)
{
	StartRxd();         //开始接收
	while(!RxdEnd);  //等到接受结束
	return RxdBuf;
}



//开始发送函数，data要发送的数据
void StartTxd(uint8_t data)
{
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
  TxdBuf = data;     //缓存发送数据
  GPIO_SetBits(TX_GPIO_PORT,TX_GPIO_Pin);    //发送开始,拉低电平
	TxdEnd = 0;     //清零发送结束标志位
  RxdOrTxd = 1;	  //标志发送中
}



/******************发送数据********************************/
void TxdData(uint8_t data)
{
	StartTxd(data);
	while(!TxdEnd);
}




//中断服务函数，被SysTick_Handler调用
void UartMoni_Handler(void)
{
	static uint8_t nbit = 0;   //记录数据位数
	if(RxdOrTxd==1)  //若为发送中断
	{
		nbit++;
        if (nbit <= 8)  //低位开始发送8bits
        {
					  TxdBuf & 0x01?GPIO_ResetBits(TX_GPIO_PORT,TX_GPIO_Pin):GPIO_SetBits(TX_GPIO_PORT,TX_GPIO_Pin);
            TxdBuf >>= 1;
        }
        else if (nbit == 9)  //发送停止位
        {
            GPIO_ResetBits(TX_GPIO_PORT,TX_GPIO_Pin);  //拉高
        }
        else  //发送结束
        {
            nbit = 0;    //复原位记录
            SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;  //失能中断标志位
						RxdOrTxd = 0;   //标志空闲
            TxdEnd = 1;   //标志发送结束
        }
	}
	else if(RxdOrTxd==2)  //若为接收中断
	{
		    if (nbit == 0)     //起始位
        {
					  SysTick->LOAD  = ((72*Buad)& SysTick_LOAD_RELOAD_Msk) - 1;   //恢复原来周期
            if (!GPIO_ReadInputDataBit(RX_GPIO_PORT, RX_GPIO_Pin)) //起始位若为0，清零接收缓冲区，准备接收
            {
                RxdBuf = 0;
                nbit++;
            }
            else          //起始位不为0，结束接收
            {
                SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;  //失能中断标志位
						  	RxdOrTxd = 0;   //标志空闲
            }
        }
        else if (nbit <= 8)   //低位开始接收数据位
        {
            RxdBuf >>= 1;    //先将之前低位所在位右移
            if (GPIO_ReadInputDataBit(RX_GPIO_PORT, RX_GPIO_Pin))     //接收为1时，最高位置1；为0时不处理仍为0
            {
                RxdBuf |= 0x80;
            }
            nbit++;
        }
        else  //接收结束
        {
            nbit = 0;         //复原位记录
            SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;  //失能中断标志位
						RxdOrTxd = 0;   //标志空闲
            if (GPIO_ReadInputDataBit(RX_GPIO_PORT, RX_GPIO_Pin))     //停止位为1，方认为数据有效
            {
                RxdEnd = 1;  //标志接受结束
            }
        }
	}
}



/****************** 发送8位的数组 ************************/
void TxdArray(uint8_t *array, uint16_t num)
{
  uint8_t i;
	
	for(i=0; i<num; i++)
  {
	    /* 发送一个字节数据到USART */
	    TxdData(array[i]);	
  
  }
	/* 等待发送完成 */
  while(!TxdEnd);
}

/******************接收8位的数组**********************/
void RxdArray(uint8_t *array, uint16_t num)
{
	 uint8_t i;
	
	for(i=0; i<num; i++)
  {
	    /* 接收uart的一个字节数据 */
	    array[i] = RxdData();	
  }
	 while(!RxdEnd);
}

/*****************  发送字符串 **********************/
void TxdString(char *str)
{
	unsigned int k=0;
  do 
  {
       TxdData( *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* 等待发送完成 */
  while(!TxdEnd);
}

/********************接收字符串**************************/
void RxdString(char *str)
{
	unsigned int k = 0;
	do
	{
			*(str+k) = RxdData();
		  k++;
	}while(*(str+k)=='\n');
	while(!RxdEnd);
}

/*****************  发送一个16位数 **********************/
void TxdHalfWord(uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* 取出高八位 */
	temp_h = (ch&0XFF00)>>8;
	/* 取出低八位 */
	temp_l = ch&0XFF;
	
	/* 发送高八位 */
	TxdData(temp_h);	
	/* 发送低八位 */
	TxdData(temp_l);	
}


/********************接收一个16位的数*********************/
void RxdHalfWord(uint16_t *ch)
{
	uint8_t temp_h, temp_l;
	
	temp_h = RxdData();
	temp_l = RxdData();
	
	*ch = temp_h<<8|temp_l;
}


#if 0
#include "stdio.h"
///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口 */
		TxdData((uint8_t) ch);
		
		/* 等待发送完毕 */
		while(!TxdEnd);		
	
		return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
    while(GPIO_ReadInputDataBit(RX_GPIO_PORT, RX_GPIO_Pin));    //等到开始位（低电平）
	
		return (int)RxdData();
}
#endif


