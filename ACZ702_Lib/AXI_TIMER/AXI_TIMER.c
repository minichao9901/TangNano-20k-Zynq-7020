/**
  *****************************************************************************
  * 					AXI TIMER IP核的定时器中断相关应用库
  *****************************************************************************
  *
  * @File   : AXI_Timer.c
  * @By     : Sun
  * @Version: V1.0
  * @Date   : 2022 / 05 / 19
  * @Shop	: https://xiaomeige.taobao.com/
  *
  *****************************************************************************
**/

#include "AXI_TIMER.h"

XTmrCtr AXI_Timer0;		//定时器实例


/**
  *****************************************************
  * @brief	初始化AXI_Timer设备
  * @param	TmrCtrInstPtr	Timer对象指针
  * @param	DevId			Timer设备ID
  * @usage	//定时器初始化
  * 		AXI_Timer_Init(&Timer0,TMRCTR_DEVICE_ID);
  *****************************************************
**/
void AXI_Timer_Init(XTmrCtr *TmrCtrInstPtr,uint16_t DevId)
{
	XTmrCtr_Initialize(TmrCtrInstPtr, DevId);
}


/**
  *****************************************************
  * @brief	初始化AXI_Timer中断
  * @param	TmrCtrInstPtr	Timer对象指针
  * @param	IntrId			中断ID
  * @param	Timer_Handler	Timer的中断服务函数
  * @usage	//定时器中断初始化,中断服务函数为AXI_Timer0_Handler
  * 		AXI_Timer_Int_Init(&Timer0,TMRCTR_INTERRUPT_ID,AXI_Timer0_Handler);
  *****************************************************
**/
void AXI_Timer_Int_Init(XTmrCtr *TmrCtrInstPtr,uint16_t IntrId,XTmrCtr_Handler Timer_Handler)
{
	Set_ScuGic_Link(IntrId, 0xA8, HIGH_Level_Sensitive, XTmrCtr_InterruptHandler, TmrCtrInstPtr);
	XTmrCtr_SetHandler(TmrCtrInstPtr, Timer_Handler,TmrCtrInstPtr);
}

/**
  *****************************************************
  * @brief	设置AXI_Timer配置
  * @param	TmrCtrInstPtr	Timer对象指针
  * @param	TmrCtrNum		AXI_Timer计数器：0为计数器0，1为计数器1
  * @param	Mode			Timer模式配置
  * 模式有以下几种：	XTC_CASCADE_MODE_OPTION		启用级联模式，仅适用于TCSRO
  *					XTC_ENABLE_ALL_OPTION		一次性启用所有定时计数器
  *                 XTC_DOWN_COUNT_OPTION		配置为向下计数（默认为向上计数）
  *                 XTC_CAPTURE_MODE_OPTION		配置为输入捕获模式（默认为比较模式）
  *                 XTC_INT_MODE_OPTION			开定时器中断，向下计数到0或向上计数溢出时触发
  *                 XTC_AUTO_RELOAD_OPTION		开启自动重装载
  *                 XTC_EXT_COMPARE_OPTION		启用外部比较输出信号
  * @param	Reload_Val		装载值，每个CLK它会向上或下计数1，这里的CLK由AXI_TIMER IP核外部供给
  * @usage	//定时器计数器0配置为中断模式，自动重装载且向下计数
  * 		AXI_Timer_SetOption(&Timer0,TIMER_CNTR_0, XTC_INT_MODE_OPTION |
  *				XTC_AUTO_RELOAD_OPTION | XTC_DOWN_COUNT_OPTION,RELOAD_VALUE);
  *****************************************************
**/
void AXI_Timer_SetOption(XTmrCtr *TmrCtrInstPtr,uint8_t TmrCtrNum,uint32_t Mode,uint32_t Reload_Val)
{
	XTmrCtr_SetOptions(TmrCtrInstPtr, TmrCtrNum, Mode);

	//设置重装载值，这个值在计数器启动时被载入
	XTmrCtr_SetResetValue(TmrCtrInstPtr, TmrCtrNum, Reload_Val);

	//启动计数器
	XTmrCtr_Start(TmrCtrInstPtr, TmrCtrNum);
}
