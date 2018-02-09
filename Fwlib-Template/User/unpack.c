
#include "unpack.h"
#include "usart.h"

u8 dataPID;											
u8 buttonFlag = 0x00;									
vu16 remoteControl[4];							

u8 ex_buf[12];	
#define RxBuf  ex_buf
//===========================================================================================



void UnpackData(void)
{
	if(RxBuf[11]!=0xa5)								
		return;
	
	if(RxBuf[0] & 0x01){								
		remoteControl[1] = RxBuf[5]<<8|RxBuf[4];		//ADC1
		remoteControl[2] = RxBuf[7]<<8|RxBuf[6];		//ADC4
		remoteControl[3] = RxBuf[9]<<8|RxBuf[8];		//ADC3
		
		printf("youmen:::%d\r\n",remoteControl[1]);
		

	}else if(RxBuf[0] & 0x08){						
		buttonFlag = RxBuf[1];
	}
	

	dataPID = RxBuf[10];
}







