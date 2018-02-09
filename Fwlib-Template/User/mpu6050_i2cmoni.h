#ifndef __MPU6050_I2CMONI_H
#define __MPU6050_I2CMONI_H

#include "stm32f10x.h"

void MPU6050_MoniI2c_Config(void);
void MPU6050_MoniI2c_ITConfig(void);
u8 MPU6050_MoniI2c_GetId(void);
u16 MPU6050_MoniI2c_GetAccX(void);
u16 MPU6050_MoniI2c_GetAccY(void);
u16 MPU6050_MoniI2c_GetAccZ(void);
u16 MPU6050_MoniI2c_GetGyroX(void);
u16 MPU6050_MoniI2c_GetGyroY(void);
u16 MPU6050_MoniI2c_GetGyroZ(void);
float MPU6050_MoniI2c_GetTemperature(void);

#endif
