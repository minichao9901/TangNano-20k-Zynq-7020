#ifndef ACZ702_LIB_ISR_H_
#define ACZ702_LIB_ISR_H_

#include "COMMON.h"

//�ж���ʹ�õı�־λ
extern volatile int All_Send_Flag;	//ȫ�����ͱ�־
extern volatile int All_Recv_Flag;	//ȫ�����ձ�־

void ScuTimer_IRQ_Handler(void *CallBackRef);

void AXI_UART0_Send_IRQ_Handler(void *CallBackRef, unsigned int EventData);
void AXI_UART0_Recv_IRQ_Handler(void *CallBackRef, unsigned int EventData);

#endif /* ACZ702_LIB_ISR_H_ */
