#ifndef ACZ702_LIB_ISR_H_
#define ACZ702_LIB_ISR_H_

#include "COMMON.h"

extern uint8_t Tube_Refresh_Flag;
extern uint8_t RTC_Refresh_Flag;

void ScuTimer_IRQ_Handler(void *CallBackRef);


#endif /* ACZ702_LIB_ISR_H_ */
