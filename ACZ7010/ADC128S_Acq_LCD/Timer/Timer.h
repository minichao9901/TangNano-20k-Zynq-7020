#ifndef _TIMER_H_
#define _TIMER_H_


#include "xscutimer.h"
#include "xscugic.h"
#include "xil_exception.h"
#include "stdio.h"


//定时器时钟频率为CPU时钟频率除以二，666/2 = 333MHz
//定时器中断为n微秒,则装载值为n*1000/(1000/333) - 1 = n*333 - 1
#define TIMER_LOAD_VALUE(n)    (n*333 - 1)	//将us转换为定时器装载值


extern XScuGic Intc;	//中断控制器实例
extern XScuTimer Timer;	//定时器实例


void Timer_Init(XScuGic *Intc_Ptr,XScuTimer *Timer_Ptr, uint32_t Time_us);
void Timer_Intr_Handler(void *CallBackRef);


#endif /* TIMER_TIMER_H_ */
