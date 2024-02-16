#ifndef ACZ702_LIB_ISR_H_
#define ACZ702_LIB_ISR_H_

#include "COMMON.h"

//中断里使用的标志位
extern volatile int Send_All_Flag;	//全部发送标志
extern volatile int Recv_All_Flag;	//全部接收标志
extern volatile int TimeOut_Flag;	//超时标志

void ScuTimer_IRQ_Handler(void *CallBackRef);

void PS_UART1_IRQ_Handler(void *CallBackRef, uint32_t Event, unsigned int EventData);

#endif /* ACZ702_LIB_ISR_H_ */
