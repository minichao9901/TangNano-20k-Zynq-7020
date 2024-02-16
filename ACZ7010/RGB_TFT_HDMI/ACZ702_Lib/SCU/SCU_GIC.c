/**
  *****************************************************************************
  * 						通用中断控制器库
  *****************************************************************************
  *
  * @File   : SCU_GIC.c
  * @By     : Sun
  * @Version: V0.5
  * @Date   : 2022 / 05 / 30
  * @Shop	: https://xiaomeige.taobao.com/
  *
  *****************************************************************************
**/
#include "SCU_GIC.h"

XScuGic ScuGic;	//通用中断控制器对象

/**
  *****************************************************
  * @brief	初始化通用中断控制器
  * @usage	ScuGic_Init();	//初始化通用中断控制器
  * @tag	初始化通用中断控制器后，才可链接中断
  *****************************************************
**/
void ScuGic_Init()
{
	XScuGic_Config *IntcConfig;
	IntcConfig = XScuGic_LookupConfig(XPAR_SCUGIC_SINGLE_DEVICE_ID);
	XScuGic_CfgInitialize(&ScuGic, IntcConfig,IntcConfig->CpuBaseAddress);

	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			(Xil_ExceptionHandler)XScuGic_InterruptHandler,&ScuGic);
	Xil_ExceptionEnable();
}

/**
  *****************************************************
  * @brief	将中断链接到通用中断控制器
  * @param	IntrId		中断请求ID
  * @param	Priority	中断优先级，高5位有效，越大优先级越低
  * @param	Trigger		中断触发模式：
  * 							高电平触发 -> HIGH_Level_Sensitive
  * 							上升沿触发 -> Rising_Edge_Sensitive
  * @param	Handler		中断服务函数
  * @param	CallBackRef	回调引用，通常是连接驱动程序的实例指针
  * @usage	//链接XPAR_SCUTIMER_INTR到中断控制器，中断服务函数为ScuTimer_IRQ_Handler
  * 		Set_ScuGic_Link(XPAR_SCUTIMER_INTR, 0xA8, HIGH_Level_Sensitive,
  * 			ScuTimer_IRQ_Handler, (void *)&ScuTimer);
  *****************************************************
**/
void Set_ScuGic_Link(uint16_t IntrId, uint8_t Priority, uint8_t Trigger,
		Xil_InterruptHandler Handler, void *CallBackRef)
{
	XScuGic_SetPriorityTriggerType(&ScuGic, IntrId, Priority, Trigger);
	XScuGic_Connect(&ScuGic, IntrId,
			(Xil_ExceptionHandler)Handler,CallBackRef);
	XScuGic_Enable(&ScuGic, IntrId);
}
