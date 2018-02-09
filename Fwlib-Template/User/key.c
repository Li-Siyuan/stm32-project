/*********188开发板专用key函数****************
**********作者：李思远******************
**********参考：秉火******************
**********新增：KEY单个初始化函数以及屏蔽不同类型按键的硬件接口******************************/

#include "key.h"
#include "systick.h"
/*
初始化所有按键
*/
void KEY_Conf(void)
{
	//定义GPIO类型结构体，初始化GPIO时钟
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(KEYUP_GPIO_CLK|KEY0_GPIO_CLK|KEY1_GPIO_CLK|KEY2_GPIO_CLK,ENABLE);
	//初始化KEYUP为浮空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	
	GPIO_InitStructure.GPIO_Pin = KEYUP_GPIO_PIN;
	GPIO_Init(KEYUP_GPIO_PORT,&GPIO_InitStructure);
	//初始化KEY0为上拉输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_InitStructure.GPIO_Pin = KEY0_GPIO_PIN;
	GPIO_Init(KEY0_GPIO_PORT,&GPIO_InitStructure);
  //初始化KEY1为上拉输入
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN;
	GPIO_Init(KEY1_GPIO_PORT,&GPIO_InitStructure);
	//初始化KEY2为上拉输入
	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN;
	GPIO_Init(KEY2_GPIO_PORT,&GPIO_InitStructure);
}

/*
初始化单个KEY
x为0，1，2
*/
void KEYUP_Conf(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(KEYUP_GPIO_CLK,ENABLE);
		//初始化KEYUP为浮空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	
	GPIO_InitStructure.GPIO_Pin = KEYUP_GPIO_PIN;
	GPIO_Init(KEYUP_GPIO_PORT,&GPIO_InitStructure);
}
void KEYx_Conf(uint8_t x)
{
	switch(x)
	{
		case 0:
		{
			//定义GPIO类型结构体，初始化GPIO时钟
			GPIO_InitTypeDef GPIO_InitStructure;
			//判断按键类型
			RCC_APB2PeriphClockCmd(KEY0_GPIO_CLK,ENABLE);
			//初始化KEYx为上拉输入
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
			GPIO_InitStructure.GPIO_Pin = KEY0_GPIO_PIN;
			GPIO_Init(KEY0_GPIO_PORT,&GPIO_InitStructure);
			break;
		}
		case 1:
		{
			//定义GPIO类型结构体，初始化GPIO时钟
			GPIO_InitTypeDef GPIO_InitStructure;
			//判断按键类型
			RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK,ENABLE);
			//初始化KEYx为上拉输入
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
			GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN;
			GPIO_Init(KEY1_GPIO_PORT,&GPIO_InitStructure);
			break;
		}
		case 2:
		{
			//定义GPIO类型结构体，初始化GPIO时钟
			GPIO_InitTypeDef GPIO_InitStructure;
			//判断按键类型
			RCC_APB2PeriphClockCmd(KEY2_GPIO_CLK,ENABLE);
			//初始化KEYx为上拉输入
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
			GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN;
			GPIO_Init(KEY2_GPIO_PORT,&GPIO_InitStructure);
			break;
		}
	}
}

/*
检测KEYUP
x为UP，0，1，2
*/

uint8_t KEYUP_Scan(void)
{
	if(GPIO_ReadInputDataBit(KEYUP_GPIO_PORT, KEYUP_GPIO_PIN) == KEYUP_ON)
		{
			while(GPIO_ReadInputDataBit(KEYUP_GPIO_PORT, KEYUP_GPIO_PIN) == KEYUP_ON);
			return KEYUP_ON;	
		}
	else
		  return KEYUP_OFF;
}

uint8_t KEYx_Scan(uint8_t x)
{
	switch(x)
	{
		case 0:
		{
			if(GPIO_ReadInputDataBit(KEY0_GPIO_PORT,KEY0_GPIO_PIN	) == KEY_ON)
			{
				while(GPIO_ReadInputDataBit(KEY0_GPIO_PORT,KEY0_GPIO_PIN	) == KEY_ON);
				return KEY_ON;	
			}
		  else
			  return KEY_OFF;
		}
		case 1:
		{
			if(GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY1_GPIO_PIN	) == KEY_ON)
			{
				while(GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY1_GPIO_PIN	) == KEY_ON);
				return KEY_ON;	
			}
		  else
			  return KEY_OFF;
		}
		case 2:
		{
			if(GPIO_ReadInputDataBit(KEY2_GPIO_PORT,KEY2_GPIO_PIN	) == KEY_ON)
			{
				while(GPIO_ReadInputDataBit(KEY2_GPIO_PORT,KEY2_GPIO_PIN	) == KEY_ON);
				return KEY_ON;	
			}
		  else
			  return KEY_OFF;
		}
		default:return 0;
	}
}


uint8_t KEY_Scan(void)
{
       uint8_t keyValue = 0;

    if((KEYUP == KEYUP_ON) || (KEY0 == KEY_ON) || (KEY1 == KEY_ON) || (KEY2 == KEY_ON)) //检测是否有按键按下
    {
        delay_ms(10);                                      //延时消抖

        /* 检测是哪个按键按下 */
        if(KEYUP == KEYUP_ON)
        {
            keyValue = KEY_UP;
        }
        else if(KEY0 == KEY_ON)
        {
            keyValue = KEY_LEFT;
        }
        else if(KEY1 == KEY_ON)
        {
            keyValue = KEY_DOWN;
        }
        else if(KEY2 == KEY_ON)
        {
            keyValue = KEY_RIGHT;
        }
        else
        {
            keyValue = 0;
        }
        
        /* 有按键按下时，做松手检测 */
        if(keyValue != 0)
        {
            while(((KEYUP == KEYUP_ON) || (KEY0 == KEY_ON) || (KEY1 == KEY_ON) || (KEY2 == KEY_ON)));
//            delay_ms(10); //由于主函数中程序较少，连续扫描的速度太快，加一个松手消抖减少误读   
        }        
    }

    return keyValue;
}


