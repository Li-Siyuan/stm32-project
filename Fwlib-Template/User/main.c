#include "stm32f10x.h"
#include "include.h"


int main()
{	
	//voltage = (dat16 / 65536.0) * 5.0;
	uint16_t voltage =  0x8000;
	
	//初始化GPIO
	AD5667_Init();
	
	//设置DAC
	AD5667_WRITE_REG(COMMAND, voltage);
	
	//死循环
	while(1);
}


