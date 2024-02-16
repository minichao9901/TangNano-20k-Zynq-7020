#ifndef ACZ702_LIB_COMMON_H_
#define ACZ702_LIB_COMMON_H_


//系统头文件
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


//Xilinx头文件
#include "xil_types.h"
#include "sleep.h"
#include "xparameters.h"
#include "xil_exception.h"
#include "xscugic.h"
#include "xscutimer.h"


//ACZ702头文件
#include "ISR.h"
#include "SCU_GIC.h"
#include "SCU_TIMER.h"

#include "PS_GPIO.h"
#include "PS_SPI.h"
#include "PS_IIC.h"

//用户头文件
#include "RTC.h"


//用户宏定义
#define	CPU_CLK_HZ	XPAR_PS7_CORTEXA9_0_CPU_CLK_FREQ_HZ	//CPU时钟频率(单位Hz)
#define INPUT		1
#define OUTPUT		0
#define	REG8		8
#define	REG16		16

#define DISPLAY_MODE	PS_GPIO_GetPort(54)	//数码管显示模式，SW0向上为模式1，向下为模式0
//用户声明引用


//用户定义





#endif /* ACZ702_LIB_COMMON_H_ */
