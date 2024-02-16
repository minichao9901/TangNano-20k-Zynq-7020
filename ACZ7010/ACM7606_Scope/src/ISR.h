#ifndef ACZ702_LIB_ISR_H_
#define ACZ702_LIB_ISR_H_

#include "COMMON.h"

//声明标志
extern uint8_t Wave_Run;			//波形采集标志
extern uint8_t Single_TriggerFlag;	//触发标志
extern uint8_t Cancel_Trigger;		//取消触发

//声明各个进程的标志
extern uint8_t Flag_DrawWave;		//绘制波形，10ms一次
extern uint8_t Flag_DrawGrid;		//绘制网格，20ms一次
extern uint8_t Flag_TouchScan;		//触摸扫描，30ms一次
extern uint8_t Flag_Refresh_Val;	//刷新电压和频率数值，500ms一次

//标志中断处理程序用于将事件通知应用程序上下文
extern volatile int RxDone;
extern volatile int Error;

void ScuTimer_IRQ_Handler(void *CallBackRef);
void AXI_DMARx_IRQHandler(void *Callback);

#endif /* ACZ702_LIB_ISR_H_ */
