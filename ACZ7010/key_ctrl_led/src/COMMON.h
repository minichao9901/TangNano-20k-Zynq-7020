#ifndef ACZ702_LIB_COMMON_H_
#define ACZ702_LIB_COMMON_H_


//系统头文件
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>

//Xilinx头文件
#include "xil_types.h"
#include "xil_cache.h"
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


//用户头文件



//用户宏定义
#define	CPU_CLK_HZ	XPAR_PS7_CORTEXA9_0_CPU_CLK_FREQ_HZ	//CPU时钟频率(单位Hz)
#define INPUT		1
#define OUTPUT		0
#define	REG8		8
#define	REG16		16

//MIOn对应的GPIO编号为n
#define PS_LED      7       //MIO_LED为MIO7，对应的GPIO编号为7
#define PS_KEY      47      //MIO_KEY为MIO47，对应的GPIO编号为47
//EMIOn对应的GPIO编号为54+n，因为MIO总数为54，EMIO是从54开始算的
#define PL_LED  (54 + 0)    //EMIO_LED为EMIO0，对应的GPIO编号为54+0=54
#define PL_KEY  (54 + 1)    //EMIO_KEY为EMIO1，对应的GPIO编号为54+1=55
//用户声明引用

#endif /* ACZ702_LIB_COMMON_H_ */
