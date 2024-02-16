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

#include "AXI_SPI.h"
#include "PS_GPIO.h"
#include "TTC.h"



//用户头文件
#include "send.h"
#include "Digital_Tube_8Bits.h"


//用户宏定义
#define	CPU_CLK_HZ	XPAR_PS7_CORTEXA9_0_CPU_CLK_FREQ_HZ	//CPU时钟频率(单位Hz)

#define	OUTPUT	0
#define INPUT	1

#define CHA_CtrlWord (0xC << 12)//1100 <<12
#define CHB_CtrlWord (0x4 << 12)// 0100 << 12
#define CHAB_CtrlWord (0x5 << 12)//0101 <<12
#define CHABC_CtrlWord (0xC << 12)//1100  <<12

//EMIOn对应的GPIO编号为54+n，因为MIO总数为54，EMIO是从54开始算的
#define KEY_S0 (54 + 0) //KEY_S0为EMIO0，对应的GPIO编号为54+0=54
#define KEY_S1 (54 + 1) //KEY_S1为EMIO1，对应的GPIO编号为54+1=55
#define KEY_S2 (54 + 2) //KEY_S2为EMIO2，对应的GPIO编号为54+2=56
#define KEY_S3 (54 + 3) //KEY_S3为EMIO3，对应的GPIO编号为54+3=57
#define KEY_S4 (54 + 4) //KEY_S4为EMIO4，对应的GPIO编号为54+4=58
#define SW0 (54 + 5) //SW0为EMIO3，对应的GPIO编号为54+5=59
#define SW1 (54 + 6) //SW1为EMIO4，对应的GPIO编号为54+6=60

extern uint8_t Channel,DAC_Wave,F_Word_cnt,P_Word_cnt;

extern uint8_t Dig_Data[8];

extern uint16_t DAC_CH_CtrlWord,P_Word;
extern uint32_t F_Word;
extern unsigned short *Wave_Point;

//用户声明引用

#endif /* ACZ702_LIB_COMMON_H_ */
