/***************************************************************************************
									声明
本项目代码仅供个人学习使用，可以自由移植修改，但必须保留此声明信息。移植过程中出现其他不可
估量的BUG，修远智控不负任何责任。请勿商用！

程序版本号：	2.0
日期：			2017-1-1
作者：			东方萧雨、小马哥
版权所有：		修远智控N0.1实验室
****************************************************************************************/
/************************************************************************
采用模拟I2C和MPU6050进行通讯

注意：这里只能得到原始数据，方便的测试MPU6050硬件连接上是否通过，要想得到融
合后的姿态数据，请使用mpu_dmp_useapi.c中定义的内容，并且在主函数初始化
MPU6050_MoniI2c_Config()函数前，请先初始化I2C_MoniConfig()

主函数测试代码：
	USART_Config();
	SysTick_init();
	
	//初始化模拟I2C的端口和引脚
	I2C_MoniConfig();

	MPU6050_MoniI2c_Config();
	printf("anything is ready\r\n");
	
	printf("ID is :%x\r\n",MPU6050_MoniI2c_GetId());
	
	while(1){
		printf("temp is :%f\r\n",MPU6050_MoniI2c_GetTemperature());
	}
************************************************************************/
#include "mpu6050_i2cmoni.h"
#include "i2c_moni.h"
#include "systick.h"

#define	SlaveAddress	0x68	//从机设备地址

/*****************************************************************************
MPU6050内部寄存器地址
*****************************************************************************/
#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125HZ)
#define	CONFIG			0x1A	//低通滤波器，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检,2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检，测量范围及高通滤波频率，典型值：0x01(不自检,2G,5Hz)
#define FIFO_EN			0x23	//各功能的FIFO的使能或失能
#define INT_PIN_CFG		0x37	//中断/旁路设置
#define INT_ENABLE		0x38	//中断使能寄存器
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define USER_CTRL		0x6A	//用户控制寄存器
#define	PWR_MGMT_1		0x6B	//电源管理，典型值:0x01(正常启用)
#define PWR_MGMT_2		0x6C	//设置正常工作模式还是唤醒模式
#define	WHO_AM_I		0x75	//IIC地址寄存器(默认数值0x68,只读)
/*******************************************************************************/





/**********************************函数区******************************************/
//初始化MPU6050
void MPU6050_MoniI2c_Config(void)
{
	
	//初始化MPU6050，解除睡眠状态
	MoniI2c_WriteByteToSlave(SlaveAddress,PWR_MGMT_1, 0x80);			//复位MPU6050
	delay_ms(100);														//延迟100ms
	MoniI2c_WriteByteToSlave(SlaveAddress,PWR_MGMT_1, 0x00);			//唤醒MPU6050
	
	MoniI2c_WriteByteToSlave(SlaveAddress,GYRO_CONFIG, 0x18);			//设置陀螺仪量程为：±2000dps
	MoniI2c_WriteByteToSlave(SlaveAddress,ACCEL_CONFIG, 0x00);			//设置加速度传感器量程为：±2g
	MoniI2c_WriteByteToSlave(SlaveAddress,SMPLRT_DIV, 0x13);			//设置采样率的分频值为19，从而采样率为50Hz
	MoniI2c_WriteByteToSlave(SlaveAddress,CONFIG, 0x04);				//设置数字低通滤波值，滤波带宽为20Hz
	MoniI2c_WriteByteToSlave(SlaveAddress,USER_CTRL, 0x00);				//关闭所有FIFO，并且关闭I2C主模式
	MoniI2c_WriteByteToSlave(SlaveAddress,FIFO_EN, 0x00);				//关闭各功能的FIFO
	MoniI2c_WriteByteToSlave(SlaveAddress,INT_PIN_CFG, 0x80);			//设置INT引脚（INT引脚低电平有效，推挽输出...)
	MoniI2c_WriteByteToSlave(SlaveAddress,INT_ENABLE, 0x00);			//关闭所有中断
	
	MoniI2c_WriteByteToSlave(SlaveAddress,PWR_MGMT_1, 0x01);			//使用X轴的PLL时钟作为工作时钟
	MoniI2c_WriteByteToSlave(SlaveAddress,PWR_MGMT_2, 0x00);			//设置陀螺仪和加速计处于正常工作模式
}


//开启MPU6050中断(INT)
void MPU6050_MoniI2c_ITConfig(){
	MoniI2c_WriteByteToSlave(SlaveAddress,INT_PIN_CFG,0x01);
	MoniI2c_WriteByteToSlave(SlaveAddress,INT_ENABLE,0x01);
}


//获得MPU6050的ID
u8 MPU6050_MoniI2c_GetId(void)
{	
	u8 data;
	if(!MoniI2c_ReadFromSlave(SlaveAddress,0x75,&data)){
		return data;
	}else{
		return 0;
	}
}


//读取X轴的加速计的原始值,读取成功则返回数据，失败则返回0；
u16 MPU6050_MoniI2c_GetAccX(void)
{
	u8 tem[2];
	if(!MoniI2c_ReadSomeDataFromSlave(SlaveAddress,ACCEL_XOUT_H,2,tem)){
		return tem[0]<<8|tem[1];
	}else{
		return 0;
	}
}

//读取Y轴的加速计的原始值,读取成功则放回数据，失败则返回0；
u16 MPU6050_MoniI2c_GetAccY(void)
{
	u8 tem[2];
	if(!MoniI2c_ReadSomeDataFromSlave(SlaveAddress,ACCEL_YOUT_H,2,tem)){
		return tem[0]<<8|tem[1];
	}else{
		return 0;
	}
}

//读取Z轴的加速计的原始值,读取成功则放回数据，失败则返回0；
u16 MPU6050_MoniI2c_GetAccZ(void)
{
	u8 tem[2];
	if(!MoniI2c_ReadSomeDataFromSlave(SlaveAddress,ACCEL_ZOUT_H,2,tem)){
		return tem[0]<<8|tem[1];
	}else{
		return 0;
	}
}


//读取X轴的陀螺仪的原始值,读取成功则放回数据，失败则返回0；
u16 MPU6050_MoniI2c_GetGyroX(void)
{
	u8 tem[2];
	if(!MoniI2c_ReadSomeDataFromSlave(SlaveAddress,GYRO_XOUT_H,2,tem)){
		return tem[0]<<8|tem[1];
	}else{
		return 0;
	}
}

//读取Y轴的陀螺仪的原始值,读取成功则放回数据，失败则返回0；
u16 MPU6050_MoniI2c_GetGyroY(void)
{
	u8 tem[2];
	if(!MoniI2c_ReadSomeDataFromSlave(SlaveAddress,GYRO_YOUT_H,2,tem)){
		return tem[0]<<8|tem[1];
	}else{
		return 0;
	}
}

//读取X轴的陀螺仪的原始值,读取成功则放回数据，失败则返回0；
u16 MPU6050_MoniI2c_GetGyroZ(void)
{
	u8 tem[2];
	if(!MoniI2c_ReadSomeDataFromSlave(SlaveAddress,GYRO_ZOUT_H,2,tem)){
		return tem[0]<<8|tem[1];
	}else{
		return 0;
	}
}


//读取MPU6050温度原始值,读取成功则放回数据，失败则返回0；
float MPU6050_MoniI2c_GetTemperature(void)
{
	u8 tem[2];
	short value;		
	
	if(!MoniI2c_ReadSomeDataFromSlave(SlaveAddress,TEMP_OUT_H,2,tem)){
		value = (short)(tem[0]<<8|tem[1]);
		return (36.53+(value/ 340.0));
	}else{
		return 0;
	}
}


