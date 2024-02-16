#ifndef SRC_TTC_H_
#define SRC_TTC_H_

#include "COMMON.h"
#include "xttcps.h"


extern XTtcPs TTC0_0_Timer,TTC0_1_Timer,TTC0_2_Timer;


void TTC_Tick_Init(XTtcPs *TTC_Inst,int DeviceID,uint16_t IntrId,
		uint32_t Time_Us, Xil_InterruptHandler Handler);
void TTC_Tick_Start(XTtcPs *TTC_Inst);
void TTC_Tick_Stop(XTtcPs *TTC_Inst);

#endif /* SRC_TTC_H_ */
