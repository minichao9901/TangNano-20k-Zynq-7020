/**
  *****************************************************************************
  * 						TTC滴答定时器应用
  *****************************************************************************
  *
  * @File   : TTC.c
  * @By     : Sun
  * @Version: V0.5
  * @Date   : 2022 / 05 / 30
  * @Shop	: https://xiaomeige.taobao.com/
  *
  *****************************************************************************
**/

#include "TTC.h"

XTtcPs TTC0_0_Timer,TTC0_1_Timer,TTC0_2_Timer;

/**
  *****************************************************
  * @brief	TTC初始化，开启滴答定时中断
  * @param	TTC_Inst	TTC对象指针
  * @param	DeviceID	TTC设备ID
  * @param	IntrId		中断ID
  * @param	Time_Us		定时的时间，单位us
  * @param	Handler		中断服务函数
  * @usage	//初始化TTC0_0作为滴答定时器，每10us触发一次中断
  * 		TTC_Tick_Init(&TTC0_0_Timer, XPAR_XTTCPS_0_DEVICE_ID, XPS_TTC0_0_INT_ID,
  * 				10, TTC0_0_IRQ_Handler);
  * @tag	初始化后，需要使用TTC_Tick_Start开启计数
  *****************************************************
**/
void TTC_Tick_Init(XTtcPs *TTC_Inst,int DeviceID,uint16_t IntrId,
		uint32_t Time_Us, Xil_InterruptHandler Handler)
{
	XInterval Interval;
	uint8_t Prescaler = 0;
	uint32_t Load_Val;
	uint32_t Tmp_Presc;

	XTtcPs_Config *Config;
	Config = XTtcPs_LookupConfig(DeviceID);
	XTtcPs_CfgInitialize(TTC_Inst, Config, Config->BaseAddress);

	XTtcPs_SetOptions(TTC_Inst, XTTCPS_OPTION_INTERVAL_MODE | XTTCPS_OPTION_WAVE_DISABLE);

	//计算给定频率的间隔值和分频值

	Load_Val = (((float)TTC_Inst->Config.InputClockHz / 1000000) * Time_Us - 1);

	//是否需要分频
	if(Load_Val > XTTCPS_MAX_INTERVAL_COUNT) {
		//计算理想分频系数
		Tmp_Presc = Load_Val / XTTCPS_MAX_INTERVAL_COUNT;
		if(Load_Val % XTTCPS_MAX_INTERVAL_COUNT) {
			Tmp_Presc++;
		}

		//计算实际分频系数
		while((2 << Prescaler) < Tmp_Presc) {
			Prescaler++;
		}
		Tmp_Presc = (2 << Prescaler);

		//根据实际分频系数计算实际间隔
		Interval = Load_Val / Tmp_Presc;

	} else {
		//不分频
		Prescaler = XTTCPS_CLK_CNTRL_PS_DISABLE;
		Interval = Load_Val;
	}

	//设置间隔值、分频值
	XTtcPs_SetInterval(TTC_Inst, Interval);
	XTtcPs_SetPrescaler(TTC_Inst, Prescaler);

	//链接中断服务函数，中断优先级设置为0xB0
	Set_ScuGic_Link(IntrId, 0xB0, HIGH_Level_Sensitive,
			(Xil_ExceptionHandler)Handler, (void *)TTC_Inst);

	//使能间隔中断，计数完成时触发
	XTtcPs_EnableInterrupts(TTC_Inst, XTTCPS_IXR_INTERVAL_MASK);
}

/**
  *****************************************************
  * @brief	TTC定时器开始计数
  * @param	TTC_Inst	TTC对象指针
  * @usage	//TTC0_0开始计数
  * 		TTC_Tick_Start(&TTC0_0_Timer);
  *****************************************************
**/
void TTC_Tick_Start(XTtcPs *TTC_Inst)
{
	//开启定时/计数
	XTtcPs_Start(TTC_Inst);
}

/**
  *****************************************************
  * @brief	TTC定时器停止计数
  * @param	TTC_Inst	TTC对象指针
  * @usage	//TTC0_0停止计数
  * 		TTC_Tick_Stop(&TTC0_0_Timer);
  *****************************************************
**/
void TTC_Tick_Stop(XTtcPs *TTC_Inst)
{
	//关闭定时/计数
	XTtcPs_Stop(TTC_Inst);

	//复位计数值
	XTtcPs_ResetCounterValue(TTC_Inst);
}
