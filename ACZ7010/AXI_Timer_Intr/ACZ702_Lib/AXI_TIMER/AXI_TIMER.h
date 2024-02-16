#ifndef AXI_Timer_AXI_Timer_H_
#define AXI_Timer_AXI_Timer_H_

#include "COMMON.h"
#include "xtmrctr.h"


#define TMRCTR_DEVICE_ID	XPAR_TMRCTR_0_DEVICE_ID
#define TMRCTR_INTERRUPT_ID	XPAR_FABRIC_TMRCTR_0_VEC_ID
#define INTC_DEVICE_ID		XPAR_SCUGIC_SINGLE_DEVICE_ID
#define TIMER_CNTR_0	 	0
#define TIMER_CNTR_1	 	1
#define TIMER0_FREQ			XPAR_AXI_TIMER_0_CLOCK_FREQ_HZ
#define RELOAD_VAL_US(n)	((TIMER0_FREQ/1000000)*n-1)		//计算n(us)的装载值
#define RELOAD_VAL_MS(n)	((TIMER0_FREQ/1000)*n-1)		//计算n(ms)的装载值
#define RELOAD_VAL_S(n)		((TIMER0_FREQ)*n-1)				//计算n(s)的装载值

extern XTmrCtr AXI_Timer0;		//定时器实例


void AXI_Timer_Init(XTmrCtr *TmrCtrInstPtr,uint16_t DevId);
void AXI_Timer_Int_Init(XTmrCtr *TmrCtrInstPtr,uint16_t IntrId,XTmrCtr_Handler Timer_Handler);
void AXI_Timer_SetOption(XTmrCtr *TmrCtrInstPtr,uint8_t TmrCtrNum,uint32_t Mode, uint32_t Reload_Val);


#endif /* AXI_Timer_AXI_Timer_H_ */
