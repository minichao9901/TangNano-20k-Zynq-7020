#ifndef ACZ702_LIB_ISR_H_
#define ACZ702_LIB_ISR_H_

#include "COMMON.h"

extern volatile uint16_t Cnt0;
extern volatile uint16_t Cnt1;
extern volatile uint16_t Cnt0_Flag;
extern volatile uint16_t Cnt1_Flag;

void ScuTimer_IRQ_Handler(void *CallBackRef);
void AXI_Timer0_IRQ_Handler(void *CallBackRef, uint8_t TmrCtrNum);

#endif /* ACZ702_LIB_ISR_H_ */
