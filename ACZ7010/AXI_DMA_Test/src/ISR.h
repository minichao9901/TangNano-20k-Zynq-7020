#ifndef ACZ702_LIB_ISR_H_
#define ACZ702_LIB_ISR_H_

#include "COMMON.h"

extern volatile int TxDone;
extern volatile int RxDone;
extern volatile int Error;

void TxIntrHandler(void *Callback);
void RxIntrHandler(void *Callback);

void ScuTimer_IRQ_Handler(void *CallBackRef);

#endif /* ACZ702_LIB_ISR_H_ */
