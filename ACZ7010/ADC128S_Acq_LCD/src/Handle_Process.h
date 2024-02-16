#ifndef __HANDLE_PROCESS_H__
#define __HANDLE_PROCESS_H__

#include "Touch.h"
#include "ADC128S102.h"
#include "sleep.h"
#include "lcd.h"
#include "Touch.h"
#include "string.h"
#include "Timer.h"
#include "PageDesign.h"
#include "RTC.h"
#include "BMP_WR.h"

//声明标志
extern uint8_t Refresh_Flag;		//波形刷新标志
extern uint8_t Wave_Run;			//波形采集标志
extern uint8_t Single_TriggerFlag;	//触发标志
extern uint8_t Cancel_Trigger;		//取消触发

//声明各个进程的标志
extern uint8_t Flag_DrawWave;		//绘制波形，10ms一次
extern uint8_t Flag_DrawGrid;		//绘制网格，20ms一次
extern uint8_t Flag_TouchScan;		//触摸扫描，30ms一次
extern uint8_t Flag_Refresh_Val;	//刷新电压和频率数值，500ms一次
extern uint8_t Flag_Refresh_Time;	//刷新时间，500ms一次


//声明操作函数
void Read_Wave_Data();		//从DDR读取波形数据
void Init_Homepage();		//初始化主界面
void Refresh_Measure_Val();	//刷新测量值
void Refresh_Time();		//刷新时间
void Refresh_WaveWindow();	//刷新波形窗口
void Touch_Scan();			//触摸扫描
void STOP_Disable_Press();		//在STOP状态下冻结某些操作
void STOP_Ensable_Press();		//解除在STOP状态下被冻结的操作
void Trigger_Disable_Press();	//在单次触发状态下冻结某些操作
void Trigger_Enable_Press();	//解除在单次触发状态下被冻结的操作
void Perform_STOP();		//执行STOP操作
uint8_t Judge_TpXY(Touch_Data Touch_LCD, Box_XY Box);//判断触摸区域
uint8_t Judge_Two_TpXY(Touch_Data Touch_LCD, Box_XY Box);
void Handle_Single_Trigger();
void Handle_Round_Done();

#endif /* HANDLE_PROCESS_H */
