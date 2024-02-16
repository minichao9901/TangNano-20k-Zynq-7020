#ifndef ACZ702_LIB_ISR_H_
#define ACZ702_LIB_ISR_H_

#include "COMMON.h"

void ScuTimer_IRQ_Handler(void *CallBackRef);
void TTC0_1_IRQ_Handler(void *CallBackRef);
void TTC0_2_IRQ_Handler(void *CallBackRef);
void PS_GPIO_IRQ_Handler(void *CallBackRef, uint32_t Bank, uint32_t Status);

#endif /* ACZ702_LIB_ISR_H_ */
