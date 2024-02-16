#ifndef ACZ702_LIB_ISR_H_
#define ACZ702_LIB_ISR_H_

#include "COMMON.h"

//中断里使用的标志位
extern volatile int All_Send_Flag;	//全部发送标志
extern volatile int All_Recv_Flag;	//全部接收标志

void ScuTimer_IRQ_Handler(void *CallBackRef);

void AXI_UART0_Send_IRQ_Handler(void *CallBackRef, unsigned int EventData);
void AXI_UART0_Recv_IRQ_Handler(void *CallBackRef, unsigned int EventData);

#endif /* ACZ702_LIB_ISR_H_ */
