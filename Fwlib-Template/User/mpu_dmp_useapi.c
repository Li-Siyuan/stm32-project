/***************************************************************************************
									声明
本项目代码仅供个人学习使用，可以自由移植修改，但必须保留此声明信息。移植过程中出现其他不可
估量的BUG，修远智控不负任何责任。请勿商用！

程序版本号：	2.0
日期：			2017-1-1
作者：			东方萧雨,小马哥
版权所有：		修远智控N0.1实验室
****************************************************************************************/
/************************************************************************
此.c文件是提供给用户快速使用MPU6050的DMP功能的API，包括MPU6050
的初始化，DMP固件加载和初始化，DMP特性的使能，DMP中断的配置和
其他的一些设定，
同时提供了从DMP中获得四元数，并转化成姿态角的函数（dmp_getdata（...）），
并且还可以获得陀螺仪和加速计的原始数据

主函数初始化代码为：
#include "stm32f10x.h"
#include "mpu_dmp_useapi.h"
#include "usart.h"
#include "i2c_moni.h"
#include "exti.h"
#include "nvic.h"
#include "mpu6050_i2cmoni.h"



int main(void)
{	
	USART_Conf();
	I2C_MoniConf();
	mpu_dmp_init();
	DMP_EXTI_CONF();
	while(1){
	}	
}


#include "stm32f10x.h"
#include "include.h"
                        
extern float pit,rol,yaw;	

int main()
{
	Site_t site     = {0, 0}; 
  LCD_ST7735S_Conf();
	LCD_ST7735S_str(site,(u8*)"LCD Init Ok",FCOLOUR,BCOLOUR);
	
	USART_Conf();
	
	I2C_Moni_Conf();
	mpu_dmp_init();
	DMP_EXTI_CONF();
	
	while(1)
	{
		site.x=0;
		site.y=20;
		LCD_ST7735S_str(site,(u8*)"pit:  ",FCOLOUR,BCOLOUR);
		site.x=40;
		LCD_num(site,pit,FCOLOUR,BCOLOUR);
		site.y=40;
		site.x=0;
		LCD_ST7735S_str(site,(u8*)"rol:  ",FCOLOUR,BCOLOUR);
		site.x=40;
		LCD_num(site,rol,FCOLOUR,BCOLOUR);
		site.x=0;
		site.y=60;
		LCD_ST7735S_str(site,(u8*)"yaw:  ",FCOLOUR,BCOLOUR);
		site.x=40;
		LCD_num(site,yaw,FCOLOUR,BCOLOUR);
	}	
}


*****************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "stm32f10x.h"
#include "mpu_dmp_useapi.h"
#include "i2c_moni.h"

#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
//#include "ANO_DT.h"

/* Data requested by client. */
#define PRINT_ACCEL     (0x01)			//向上位机上报加速计数据的标志
#define PRINT_GYRO      (0x02)			//向上位机上报陀螺仪数据的标志
#define PRINT_QUAT      (0x04)			//向上位机上报四元数数据的标志

#define ACCEL_ON        (0x01)			//指示加速计已经打开的标志
#define GYRO_ON         (0x02)			//指示陀螺仪已经打开的标志

#define MOTION          (0)				//指示MPU正在运动状态
#define NO_MOTION       (1)				//指示MPU处于静止状态

/* Starting sampling rate. */
#define DEFAULT_MPU_HZ  (200)			//设定MPU的采样率，DMP的输出频率（注意：当DMP打开时，最后会使MPU的采样率和DMP的采样率保持一致）

#define FLASH_SIZE      (512)
#define FLASH_MEM_START ((void*)0x1800)


//接收的数据（用于根据外部输入的指令，来操作MPU的相应的功能）
struct rx_s {
    unsigned char header[3];			//从外部接收的数据包
    unsigned char cmd;					//操作指令
};

//硬件抽象层接口（标志层）（各变量的取值范围参见inv_mpu.c中的chip_cfg_s结构体中的注释）
struct hal_s {
    unsigned char sensors;				
    unsigned char dmp_on;				
    unsigned char wait_for_tap;			
    volatile unsigned char new_gyro;	
    unsigned short report;				
    unsigned short dmp_features;		
    unsigned char motion_int_mode;		
    struct rx_s rx;						
};
struct hal_s hal = {0};					//初始化hal，全部成员都复位为0

/* USB RX binary semaphore. Actually, it's just a flag. Not included in struct
 * because it's declared extern elsewhere.
 */
volatile unsigned char rx_new;			//用于指示有没有接收到外部新的数据

/* 方向矩阵
	The sensors can be mounted onto the board in any orientation. The mounting
 * matrix seen below tells the MPL how to rotate the raw data from thei
 * driver(s).
 * TODO: The following matrices refer to the configuration on an internal test
 * board at Invensense. If needed, please modify the matrices to match the
 * chip-to-body matrix for your particular set up.
 */
signed char gyro_orientation[9] = {-1, 0, 0,
                                    0,-1, 0,
                                    0, 0, 1 };

									

/* These next two functions converts the orientation matrix (see
 * gyro_orientation) to a scalar representation for use by the DMP.
 * NOTE: These functions are borrowed from Invensense's MPL.
 */
unsigned short inv_row_2_scale(const signed char *row)
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;      // error
    return b;
}

unsigned short inv_orientation_matrix_to_scalar(const signed char *mtx)
{
    unsigned short scalar;

    /*
       XYZ  010_001_000 Identity Matrix
       XZY  001_010_000
       YXZ  010_000_001
       YZX  000_010_001
       ZXY  001_000_010
       ZYX  000_001_010
     */
    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;

    return scalar;
}

///*根据设定，开启/关闭传感器
//	Handle sensor on/off combinations. */
//static void setup_gyro(void)
//{
//    unsigned char mask = 0;
//    if (hal.sensors & ACCEL_ON)
//        mask |= INV_XYZ_ACCEL;
//    if (hal.sensors & GYRO_ON)
//        mask |= INV_XYZ_GYRO;
//    /* If you need a power transition, this function should be called with a
//     * mask of the sensors still enabled. The driver turns off any sensors
//     * excluded from this mask.
//     */
//    mpu_set_sensors(mask);
//    if (!hal.dmp_on)
//        mpu_configure_fifo(mask);
//}

//手势回调函数
//static void tap_cb(unsigned char direction, unsigned char count)
//{
//    char data[2];
//    data[0] = (char)direction;
//    data[1] = (char)count;
//    send_packet(PACKET_TYPE_TAP, data);
//}


//安卓设备要求的显示方向的回调函数
//static void android_orient_cb(unsigned char orientation)
//{
//    send_packet(PACKET_TYPE_ANDROID_ORIENT, &orientation);
//}



/**************************************************************************
*自检，并校正偏差
注意：陀螺仪是检测角速度的，所以陀螺仪在自检和校准时，必须保持静止，同时DMP的
一项功能使得陀螺仪在正常运行过程中静止8秒以上的时候就会自动重新校准陀螺仪，
消除陀螺仪的累计误差
而加速计时检测加速度的，所以加速计在自检和校准的时候，Z轴必须和重力方向一致，
否则，输出的加速计的值将以自检时Z轴的方向作为基准，所以一般，为了更方便
的使用MPU，一般不进行加速计的校准，直接使加速计以默认的方向（即以芯片自身的
坐标系中的Z轴）作为基准。但是，当要使用非水平面作为MPU的初始面的目的时，则
必须进行加速计的校准
**************************************************************************/
void run_self_test(void)
{
    int result;
//    char test_packet[4] = {0};
    long gyro[3], accel[3];

	//传感器自检，并返回偏差值
    result = mpu_run_self_test(gyro, accel);
	
	//注意，这里由于只开启了陀螺仪和加速计，所以result为0x03,当开启了陀螺仪，加速计和磁力计时，result为0x07
    if (result == 0x3) {
        /* Test passed. We can trust the gyro data here, so let's push it down
         * to the DMP.
         */
        float sens;
        unsigned short accel_sens;
        mpu_get_gyro_sens(&sens);				//获得陀螺仪的灵敏度因子
        gyro[0] = (long)(gyro[0] * sens);
        gyro[1] = (long)(gyro[1] * sens);
        gyro[2] = (long)(gyro[2] * sens);
        dmp_set_gyro_bias(gyro);				//重新校准陀螺仪
		
        mpu_get_accel_sens(&accel_sens);		//获得加速计的灵敏度因子
		accel_sens = 0;						//写上这句，则不校准加速计（注意：校准加速计时，会导致加速计输出值取决于设备初始角度），因为加速计校准必须要让Z轴和重力方向一致，这导致有些设备初始化时候不方便
		accel[0] *= accel_sens;
        accel[1] *= accel_sens;
        accel[2] *= accel_sens;
        dmp_set_accel_bias(accel);				//重新校准加速计
    }

}

//根据外部输入的指令配置MPU和DMP
//static void handle_input(void)
//{
//    char c;
//    const unsigned char header[3] = "inv";
//    unsigned long pedo_packet[2];

//    /* Read incoming byte and check for header.
//     * Technically, the MSP430 USB stack can handle more than one byte at a
//     * time. This example allows for easily switching to UART if porting to a
//     * different microcontroller.
//     */
//    rx_new = 0;
//    cdcReceiveDataInBuffer((BYTE*)&c, 1, CDC0_INTFNUM);
//    if (hal.rx.header[0] == header[0]) {
//        if (hal.rx.header[1] == header[1]) {
//            if (hal.rx.header[2] == header[2]) {
//                memset(&hal.rx.header, 0, sizeof(hal.rx.header));
//                hal.rx.cmd = c;
//            } else if (c == header[2])
//                hal.rx.header[2] = c;
//            else
//                memset(&hal.rx.header, 0, sizeof(hal.rx.header));
//        } else if (c == header[1])
//            hal.rx.header[1] = c;
//        else
//            memset(&hal.rx.header, 0, sizeof(hal.rx.header));
//    } else if (c == header[0])
//        hal.rx.header[0] = header[0];
//    if (!hal.rx.cmd)
//        return;

//    switch (hal.rx.cmd) {
//    /* These commands turn the hardware sensors on/off. */
//    case '8':
//        if (!hal.dmp_on) {
//            /* Accel and gyro need to be on for the DMP features to work
//             * properly.
//             */
//            hal.sensors ^= ACCEL_ON;
//            setup_gyro();
//        }
//        break;
//    case '9':
//        if (!hal.dmp_on) {
//            hal.sensors ^= GYRO_ON;
//            setup_gyro();
//        }
//        break;
//    /* The commands start/stop sending data to the client. */
//    case 'a':
//        hal.report ^= PRINT_ACCEL;
//        break;
//    case 'g':
//        hal.report ^= PRINT_GYRO;
//        break;
//    case 'q':
//        hal.report ^= PRINT_QUAT;
//        break;
//    /* The hardware self test can be run without any interaction with the
//     * MPL since it's completely localized in the gyro driver. Logging is
//     * assumed to be enabled; otherwise, a couple LEDs could probably be used
//     * here to display the test results.
//     */
//    case 't':
//        run_self_test();
//        break;
//    /* Depending on your application, sensor data may be needed at a faster or
//     * slower rate. These commands can speed up or slow down the rate at which
//     * the sensor data is pushed to the MPL.
//     *
//     * In this example, the compass rate is never changed.
//     */
//    case '1':
//        if (hal.dmp_on)
//            dmp_set_fifo_rate(10);
//        else
//            mpu_set_sample_rate(10);
//        break;
//    case '2':
//        if (hal.dmp_on)
//            dmp_set_fifo_rate(20);
//        else
//            mpu_set_sample_rate(20);
//        break;
//    case '3':
//        if (hal.dmp_on)
//            dmp_set_fifo_rate(40);
//        else
//            mpu_set_sample_rate(40);
//        break;
//    case '4':
//        if (hal.dmp_on)
//            dmp_set_fifo_rate(50);
//        else
//            mpu_set_sample_rate(50);
//        break;
//    case '5':
//        if (hal.dmp_on)
//            dmp_set_fifo_rate(100);
//        else
//            mpu_set_sample_rate(100);
//        break;
//    case '6':
//        if (hal.dmp_on)
//            dmp_set_fifo_rate(200);
//        else
//            mpu_set_sample_rate(200);
//        break;
//	case ',':
//        /* Set hardware to interrupt on gesture event only. This feature is
//         * useful for keeping the MCU asleep until the DMP detects as a tap or
//         * orientation event.
//         */
//        dmp_set_interrupt_mode(DMP_INT_GESTURE);
//        break;
//    case '.':
//        /* Set hardware to interrupt periodically. */
//        dmp_set_interrupt_mode(DMP_INT_CONTINUOUS);
//        break;
//    case '7':
//        /* Reset pedometer. */
//        dmp_set_pedometer_step_count(0);
//        dmp_set_pedometer_walk_time(0);
//        break;
//    case 'f':
//        /* Toggle DMP. */
//        if (hal.dmp_on) {
//            unsigned short dmp_rate;
//            hal.dmp_on = 0;
//            mpu_set_dmp_state(0);
//            /* Restore FIFO settings. */
//            mpu_configure_fifo(INV_XYZ_ACCEL | INV_XYZ_GYRO);
//            /* When the DMP is used, the hardware sampling rate is fixed at
//             * 200Hz, and the DMP is configured to downsample the FIFO output
//             * using the function dmp_set_fifo_rate. However, when the DMP is
//             * turned off, the sampling rate remains at 200Hz. This could be
//             * handled in inv_mpu.c, but it would need to know that
//             * inv_mpu_dmp_motion_driver.c exists. To avoid this, we'll just
//             * put the extra logic in the application layer.
//             */
//            dmp_get_fifo_rate(&dmp_rate);
//            mpu_set_sample_rate(dmp_rate);
//        } else {
//            unsigned short sample_rate;
//            hal.dmp_on = 1;
//            /* Both gyro and accel must be on. */
//            hal.sensors |= ACCEL_ON | GYRO_ON;
//            mpu_set_sensors(INV_XYZ_ACCEL | INV_XYZ_GYRO);
//            mpu_configure_fifo(INV_XYZ_ACCEL | INV_XYZ_GYRO);
//            /* Preserve current FIFO rate. */
//            mpu_get_sample_rate(&sample_rate);
//            dmp_set_fifo_rate(sample_rate);
//            mpu_set_dmp_state(1);
//        }
//        break;
//    case 'm':
//        /* Test the motion interrupt hardware feature. */
//        hal.motion_int_mode = 1;
//        break;
//    case 'p':
//        /* Read current pedometer count. */
//        dmp_get_pedometer_step_count(pedo_packet);
//        dmp_get_pedometer_walk_time(pedo_packet + 1);
//        send_packet(PACKET_TYPE_PEDO, pedo_packet);
//        break;
//    case 'x':
//        msp430_reset();
//        break;
//    case 'v':
//        /* Toggle LP quaternion.
//         * The DMP features can be enabled/disabled at runtime. Use this same
//         * approach for other features.
//         */
//        hal.dmp_features ^= DMP_FEATURE_6X_LP_QUAT;
//        dmp_enable_feature(hal.dmp_features);
//        break;
//    default:
//        break;
//    }
//    hal.rx.cmd = 0;
//}


/************************************************************************ 
陀螺仪数据已准备好,用于DMP中断触发时，和INT引脚相连的stm32引脚的外部中断
处理函数中向主机传递DMP数据已经准备好的信号
Every time new gyro data is available, this function is called in an
 * ISR context. In this example, it sets a flag protecting the FIFO read
 * function.
 ********************************************************************/
void gyro_data_ready_cb(void)
{
    hal.new_gyro = 1;
}



//重启系统
void SYSTEM_Reset(void)
{
	__set_FAULTMASK(1);			//关闭所有中断
	NVIC_SystemReset();			//系统复位
}


/***********************************************************
MPU和DMP初始化，并完成自检和校准
返回值：0:初始化成功
		其他值，初始化失败，并可以根据返回值判断哪里出错了
***********************************************************/
u8 mpu_dmp_init(void)
{
	int result;
	
	//MPU初始化，复位寄存器的值，并给一些寄存器设置初始值
    result = mpu_init();					
    
	//mpu初始化失败，重启系统
	if (result){
		SYSTEM_Reset();
		return 1;
	}

    /* If you're not using an MPU9150 AND you're not using DMP features, this
     * function will place all slaves on the primary bus.
     * mpu_set_bypass(1);
     */

    /* Get/set hardware configuration. Start gyro. */
    /* Wake up all sensors. */
    if(mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL))			//开启所有的传感器
		return 2;
    /* Push both gyro and accel data into the FIFO. */
    if(mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL))		//配置FIFO保存陀螺仪数据和加速计的原始数据
		return 3;
    if(mpu_set_sample_rate(DEFAULT_MPU_HZ))						//设置MPU采样率，这里为200Hz（当DMP没用使用时，则MPU的采样率由此设置决定，当使用了DMP时，则有DMP的输出频率决定采样率）
		return 4;
	
//    /* Read back configuration in case it was set improperly. */
//    mpu_get_sample_rate(&gyro_rate);
//    mpu_get_gyro_fsr(&gyro_fsr);
//    mpu_get_accel_fsr(&accel_fsr);

    /* Initialize HAL state variables. */
    memset(&hal, 0, sizeof(hal));
    hal.sensors = ACCEL_ON | GYRO_ON;							//保存陀螺仪和加速计已打开的配置信息
//    hal.report = PRINT_QUAT;									

    /* To initialize the DMP:
     * 1. Call dmp_load_motion_driver_firmware(). This pushes the DMP image in
     *    inv_mpu_dmp_motion_driver.h into the MPU memory.
     * 2. Push the gyro and accel orientation matrix to the DMP.
     * 3. Register gesture callbacks. Don't worry, these callbacks won't be
     *    executed unless the corresponding feature is enabled.
     * 4. Call dmp_enable_feature(mask) to enable different features.
     * 5. Call dmp_set_fifo_rate(freq) to select a DMP output rate.
     * 6. Call any feature-specific control functions.
     *
     * To enable the DMP, just call mpu_set_dmp_state(1). This function can
     * be called repeatedly to enable and disable the DMP at runtime.
     *
     * The following is a short summary of the features supported in the DMP
     * image provided in inv_mpu_dmp_motion_driver.c:
     * DMP_FEATURE_LP_QUAT: Generate a gyro-only quaternion on the DMP at
     * 200Hz. Integrating the gyro data at higher rates reduces numerical
     * errors (compared to integration on the MCU at a lower sampling rate).
     * DMP_FEATURE_6X_LP_QUAT: Generate a gyro/accel quaternion on the DMP at
     * 200Hz. Cannot be used in combination with DMP_FEATURE_LP_QUAT.
     * DMP_FEATURE_TAP: Detect taps along the X, Y, and Z axes.
     * DMP_FEATURE_ANDROID_ORIENT: Google's screen rotation algorithm. Triggers
     * an event at the four orientations where the screen should rotate.
     * DMP_FEATURE_GYRO_CAL: Calibrates the gyro data after eight seconds of
     * no motion.
     * DMP_FEATURE_SEND_RAW_ACCEL: Add raw accelerometer data to the FIFO.
     * DMP_FEATURE_SEND_RAW_GYRO: Add raw gyro data to the FIFO.
     * DMP_FEATURE_SEND_CAL_GYRO: Add calibrated gyro data to the FIFO. Cannot
     * be used in combination with DMP_FEATURE_SEND_RAW_GYRO.
     */
    if(dmp_load_motion_driver_firmware())							//加载DMP固件				
		return 5;
    if(dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation)))		//确定dmp姿态演算的坐标
		return 6;
//    dmp_register_tap_cb(tap_cb);													
//    dmp_register_android_orient_cb(android_orient_cb);							
    hal.dmp_features = DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |DMP_FEATURE_GYRO_CAL;
    if(dmp_enable_feature(hal.dmp_features))										//使能要使用到的dmp的特征
		return 7;
    if(dmp_set_fifo_rate(DEFAULT_MPU_HZ))										//输出dmp输出采样频率	
		return 8;
	
    mpu_set_dmp_state(1);			
    hal.dmp_on = 1;					

	//开始自检，并重新调零
	  run_self_test();
	
	return 0;
	
//    __enable_interrupt();
//    /* Wait for enumeration. */
//    while (USB_connectionState() != ST_ENUM_ACTIVE);
}


/******************************************************************************
设置哪些数据要上报给上位机的标志变量report的值
rawDataReportCmd取值为：
	SET时则表示将陀螺仪和加速计的原始数据均上报给上位机
	RESET时则表示不上报陀螺仪和加速计的原始数据给上位机
quatDataReportCmd取值为：
	SET时则表示将姿态角数据上报给上位机
	RESET时则表示不上报姿态角数据给上位机
*******************************************************************************/
void SetReportFlag(u8 rawDataReportCmd,u8 quatDataReportCmd)
{
	if(rawDataReportCmd == SET){
		hal.report |= PRINT_GYRO;
		hal.report |= PRINT_ACCEL;
	}else{
		hal.report &= ~PRINT_GYRO;
		hal.report &= ~PRINT_ACCEL;
	}
	
	if(quatDataReportCmd == SET){
		hal.report |= PRINT_QUAT;
	}else{
		hal.report &= ~PRINT_QUAT;
	}
}


/***********************************************************************
*获取数据，在外部调用了此函数后，可以直接通过访问全局变量pitch,roll,yaw
来获得姿态角的数据，以及gyro[3], accel[3]来获得原始数据
获取成功返回0，失败返回1
************************************************************************/
//定义在读取dmp输出数据时常用的变量
#define q30  1073741824.0f					//q30
float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;		//四元数除以q30后的临时变量
unsigned long sensor_timestamp;				//时间戳
short gyro[3], accel[3], sensors;			//gyro[3], accel[3]原始数据保存区,sensors指示从FIFO中读取的数据是什么数据（四元数，陀螺仪，加速计等）
unsigned char more;							//指示FIFO中是否还有剩余的数据
long quat[4];								//四元数
float pit,rol,yaw;							//姿态角

u8 dmp_getdata(void)
{
	//判断上位机是否发送了新的数据
//	if (rx_new)
//		/* A byte has been received via USB. See handle_input for a list of
//		 * valid commands.
//		 */
//		handle_input();
//	msp430_get_clock_ms(&timestamp);

	//当运动中断模式开启时
//	if (hal.motion_int_mode) {
//		/* Enable motion interrupt. */
//		mpu_lp_motion_interrupt(500, 1, 5);				//低功耗运动中断配置
//		hal.new_gyro = 0;								//先将用于指示采样到新数据的标志变量复位，以便等待INT中断
	
//	/* Wait for the MPU interrupt. */
//		while (!hal.new_gyro)
//			__bis_SR_register(LPM0_bits + GIE);
//		/* Restore the previous sensor configuration. */
//		mpu_lp_motion_interrupt(0, 0, 0);
//		hal.motion_int_mode = 0;
//	}

	//当没有新数据被采样或者DMP转换出来时
//	if (!hal.sensors || !hal.new_gyro) {
//		/* Put the MSP430 to sleep until a timer interrupt or data ready
//		 * interrupt is detected.
//		 */
//		__bis_SR_register(LPM0_bits + GIE);
//		continue;
//	}

	//使用了DMP功能，可以获得原始数据和四元数数据
	if (hal.new_gyro && hal.dmp_on) {
		/* This function gets new data from the FIFO when the DMP is in
		 * use. The FIFO can contain any combination of gyro, accel,
		 * quaternion, and gesture data. The sensors parameter tells the
		 * caller which data fields were actually populated with new data.
		 * For example, if sensors == (INV_XYZ_GYRO | INV_WXYZ_QUAT), then
		 * the FIFO isn't being filled with accel data.
		 * The driver parses the gesture data to determine if a gesture
		 * event has occurred; on an event, the application will be notified
		 * via a callback (assuming that a callback function was properly
		 * registered). The more parameter is non-zero if there are
		 * leftover packets in the FIFO.
		 */
		if(dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors,&more)){		//开始从FIFO中读取数据
			return 1;
		}
		
		//当FIFO中已经没有剩余数据时,将指示剩余数据量的more标志变量复位
		if (!more)
			hal.new_gyro = 0;				//将指示有新数据的变量复位
		
		/* Gyro and accel data are written to the FIFO by the DMP in chip
		 * frame and hardware units. This behavior is convenient because it
		 * keeps the gyro and accel outputs of dmp_read_fifo and
		 * mpu_read_fifo consistent.
		 *表明了陀螺仪和加速计的原始数据都是以芯片自身的框架坐标系为基准的来采样的，从而使用
		 *dmp_read_fifo函数和mpu_read_fifo函数读取到的数据都是一致的，并且注意：当DMP没开的时候，使用
		 *mpu_read_fifo函数，而当开启了DMP的时候则使用dmp_read_fifo函数来读取FIFO中的数据
		 */
//		if (sensors & INV_XYZ_GYRO && hal.report & PRINT_GYRO)						//读取陀螺仪的原始数据并上报数据给上位机
//			send_packet(PACKET_TYPE_GYRO, gyro);
//		if (sensors & INV_XYZ_ACCEL && hal.report & PRINT_ACCEL)					//读取加速计的原始数据并上报数据给上位机
//			send_packet(PACKET_TYPE_ACCEL, accel);
		
		//将原始数据上报给上位机
//		if(sensors & INV_XYZ_GYRO && hal.report & PRINT_GYRO   &&   sensors & INV_XYZ_ACCEL && hal.report & PRINT_ACCEL){
//			ANO_DT_Send_Senser(accel[0],accel[1],accel[2],gyro[0],gyro[1],gyro[2],0,0,0,0);
//		}


		/* Unlike gyro and accel, quaternions are written to the FIFO in
		 * the body frame, q30. The orientation is set by the scalar passed
		 * to dmp_set_orientation during initialization.
		 *四元数是以设备安装的坐标系（方向矩阵）作为基准来获得数据的
		 */
		if (sensors & INV_WXYZ_QUAT){					//读取四元数的数据，并根据公式转换成姿态角
//			send_packet(PACKET_TYPE_QUAT, quat);
			q0 = quat[0] / q30;
			q1 = quat[1] / q30;
			q2 = quat[2] / q30;
			q3 = quat[3] / q30;
			pit	= asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3;
			rol = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; 
			yaw = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;
			
			//将姿态角数据上报给上位机
//			if(hal.report & PRINT_QUAT)
//				ANO_DT_Send_Status(rol,pit,yaw,0,0,0);
		}
		
		//DMP数据读取成功，返回0
		return 0;
		
	//未使用DMP，仅获取原始数据
	} else if (hal.new_gyro) {
		unsigned char sensors, more;
		/* This function gets new data from the FIFO. The FIFO can contain
		 * gyro, accel, both, or neither. The sensors parameter tells the
		 * caller which data fields were actually populated with new data.
		 * For example, if sensors == INV_XYZ_GYRO, then the FIFO isn't
		 * being filled with accel data. The more parameter is non-zero if
		 * there are leftover packets in the FIFO.
		 */
		if(mpu_read_fifo(gyro, accel, &sensor_timestamp, &sensors, &more)){				//仅从FIFO中获取原始数据
			//数据读取失败
			return 1;
		}
		if (!more)
			hal.new_gyro = 0;
//		if (sensors & INV_XYZ_GYRO && hal.report & PRINT_GYRO)
//			send_packet(PACKET_TYPE_GYRO, gyro);
//		if (sensors & INV_XYZ_ACCEL && hal.report & PRINT_ACCEL)
//			send_packet(PACKET_TYPE_ACCEL, accel);
		
		//将原始数据上报给上位机
//		if(sensors & INV_XYZ_GYRO && hal.report & PRINT_GYRO   &&   sensors & INV_XYZ_ACCEL && hal.report & PRINT_ACCEL){
//			ANO_DT_Send_Senser(accel[0],accel[1],accel[2],gyro[0],gyro[1],gyro[2],0,0,0,0);
//		}
		return 0;
	}
	
	//数据读取失败
	return 1;
}


