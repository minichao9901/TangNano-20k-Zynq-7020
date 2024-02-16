#ifndef ACZ702_LIB_COMMON_H_
#define ACZ702_LIB_COMMON_H_


//ϵͳͷ�ļ�
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


//Xilinxͷ�ļ�
#include "xil_types.h"
#include "sleep.h"
#include "xparameters.h"
#include "xil_exception.h"
#include "xscugic.h"
#include "xscutimer.h"


//ACZ702ͷ�ļ�
#include "ISR.h"
#include "SCU_GIC.h"
#include "SCU_TIMER.h"

#include "PS_GPIO.h"
#include "PS_SPI.h"
#include "PS_IIC.h"

//�û�ͷ�ļ�
#include "RTC.h"


//�û��궨��
#define	CPU_CLK_HZ	XPAR_PS7_CORTEXA9_0_CPU_CLK_FREQ_HZ	//CPUʱ��Ƶ��(��λHz)
#define INPUT		1
#define OUTPUT		0
#define	REG8		8
#define	REG16		16

#define DISPLAY_MODE	PS_GPIO_GetPort(54)	//�������ʾģʽ��SW0����Ϊģʽ1������Ϊģʽ0
//�û���������


//�û�����





#endif /* ACZ702_LIB_COMMON_H_ */
