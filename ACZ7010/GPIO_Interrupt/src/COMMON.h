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

#include "AXI_GPIO.h"
#include "LOG.h"
#include "TTC.h"
#include "PS_GPIO.h"

//用户头文件



//用户宏定义
#define	CPU_CLK_HZ	XPAR_PS7_CORTEXA9_0_CPU_CLK_FREQ_HZ	//CPU时钟频率(单位Hz)
#define INPUT		1
#define OUTPUT		0
#define	REG8		8
#define	REG16		16

#define KEY_S1  0       //AXI GPIO端口编号
#define PS_KEY 47       //PS_KEY为MIO47，对应的GPIO编号为47

//用户声明引用

#endif /* ACZ702_LIB_COMMON_H_ */
