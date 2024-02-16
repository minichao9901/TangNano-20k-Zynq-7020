#ifndef _TIMER_H_
#define _TIMER_H_


#include "xscutimer.h"
#include "xscugic.h"
#include "xil_exception.h"
#include "stdio.h"


//��ʱ��ʱ��Ƶ��ΪCPUʱ��Ƶ�ʳ��Զ���666/2 = 333MHz
//��ʱ���ж�Ϊn΢��,��װ��ֵΪn*1000/(1000/333) - 1 = n*333 - 1
#define TIMER_LOAD_VALUE(n)    (n*333 - 1)	//��usת��Ϊ��ʱ��װ��ֵ


extern XScuGic Intc;	//�жϿ�����ʵ��
extern XScuTimer Timer;	//��ʱ��ʵ��


void Timer_Init(XScuGic *Intc_Ptr,XScuTimer *Timer_Ptr, uint32_t Time_us);
void Timer_Intr_Handler(void *CallBackRef);


#endif /* TIMER_TIMER_H_ */
