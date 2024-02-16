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

#include "AXI_SPI.h"
#include "PS_GPIO.h"
#include "TTC.h"



//�û�ͷ�ļ�
#include "send.h"
#include "Digital_Tube_8Bits.h"


//�û��궨��
#define	CPU_CLK_HZ	XPAR_PS7_CORTEXA9_0_CPU_CLK_FREQ_HZ	//CPUʱ��Ƶ��(��λHz)

#define	OUTPUT	0
#define INPUT	1

#define CHA_CtrlWord (0xC << 12)//1100 <<12
#define CHB_CtrlWord (0x4 << 12)// 0100 << 12
#define CHAB_CtrlWord (0x5 << 12)//0101 <<12
#define CHABC_CtrlWord (0xC << 12)//1100  <<12

//EMIOn��Ӧ��GPIO���Ϊ54+n����ΪMIO����Ϊ54��EMIO�Ǵ�54��ʼ���
#define KEY_S0 (54 + 0) //KEY_S0ΪEMIO0����Ӧ��GPIO���Ϊ54+0=54
#define KEY_S1 (54 + 1) //KEY_S1ΪEMIO1����Ӧ��GPIO���Ϊ54+1=55
#define KEY_S2 (54 + 2) //KEY_S2ΪEMIO2����Ӧ��GPIO���Ϊ54+2=56
#define KEY_S3 (54 + 3) //KEY_S3ΪEMIO3����Ӧ��GPIO���Ϊ54+3=57
#define KEY_S4 (54 + 4) //KEY_S4ΪEMIO4����Ӧ��GPIO���Ϊ54+4=58
#define SW0 (54 + 5) //SW0ΪEMIO3����Ӧ��GPIO���Ϊ54+5=59
#define SW1 (54 + 6) //SW1ΪEMIO4����Ӧ��GPIO���Ϊ54+6=60

extern uint8_t Channel,DAC_Wave,F_Word_cnt,P_Word_cnt;

extern uint8_t Dig_Data[8];

extern uint16_t DAC_CH_CtrlWord,P_Word;
extern uint32_t F_Word;
extern unsigned short *Wave_Point;

//�û���������

#endif /* ACZ702_LIB_COMMON_H_ */
