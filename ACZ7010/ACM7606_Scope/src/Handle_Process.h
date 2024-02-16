#ifndef __HANDLE_PROCESS_H__
#define __HANDLE_PROCESS_H__

#include "COMMON.h"
#include "Touch.h"


extern uint16_t *P_ADC_Data;


//声明操作函数
void Read_Wave_Data();		//从DDR读取波形数据
void Init_Homepage();		//初始化主界面
void Refresh_Measure_Val();	//刷新测量值
void Refresh_WaveWindow();	//刷新波形窗口
void Touch_Scan();			//触摸扫描
void STOP_Disable_Press();		//在STOP状态下冻结某些操作
void STOP_Ensable_Press();		//解除在STOP状态下被冻结的操作
void Trigger_Disable_Press();	//在单次触发状态下冻结某些操作
void Trigger_Enable_Press();	//解除在单次触发状态下被冻结的操作
void Perform_STOP();		//执行STOP操作

void Handle_Single_Trigger();
void Handle_Round_Done();

#endif /* HANDLE_PROCESS_H */
