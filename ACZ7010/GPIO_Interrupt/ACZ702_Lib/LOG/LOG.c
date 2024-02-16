/**
  *****************************************************************************
  * 				存储要打印的日志信息，并在CPU空闲时打印出来
  *****************************************************************************
  *
  * @File   : LOG.c
  * @By     : Sun
  * @Version: V1.0
  * @Date   : 2021 / 12 / 23
  * @Shop	: https://xiaomeige.taobao.com/
  * @Tag	: LOG打印功能占用了PS UART1和TTC0的计数器2
  *****************************************************************************
**/
#include "LOG.h"


char log_text[4096] = {};
char *p_log;
uint32_t log_cnt = 0;
XUartPs Debug_UART;

void Debug_LOG_IRQ_Handler(void *CallBackRef)
{
	/* ↓↓↓用户处理↓↓↓ */

	LOGP();//LOG打印

	/* ↑↑↑结束处理↑↑↑ */

	//清除中断标志位（读取会自动清除）
	XTtcPs_GetInterruptStatus(&TTC0_2_Timer);
}

//PS串口中断服务函数
void Debug_UART_IRQ_Handler(void *CallBackRef)
{
	uint32_t IsrStatus;

	IsrStatus = XUartPs_ReadReg(Debug_UART.Config.BaseAddress,XUARTPS_ISR_OFFSET);

	//TX FIFO从 “非空” 变为 “空” 时
	if(IsrStatus & XUARTPS_IXR_TXEMPTY) {
		if (Debug_UART.SendBuffer.RemainingBytes == 0) {
			//发送完毕，禁用TX FIFO中断
			XUartPs_WriteReg(Debug_UART.Config.BaseAddress,XUARTPS_IDR_OFFSET,XUARTPS_IXR_TXEMPTY);

		} else {
			Debug_UART_SendBuffer(&Debug_UART);
		}
	}

	/* 清除中断状态*/
	XUartPs_WriteReg(Debug_UART.Config.BaseAddress, XUARTPS_ISR_OFFSET,IsrStatus);
}




/**
  *****************************************************
  * @brief	Debug初始化，包含初始化串口、初始化日志定时器中断
  * @usage	Log_Debug_Init();	//初始化Debug配置
  * @tag	Debug打印功能基于PS端的串口1，和TTC0
  * 		使用之前需要开启PS端UART1和TTC0
  *****************************************************
**/
void Log_Debug_Init()
{
	if(LOG_DEBUG) {

		//调试串口初始化
		Debug_UART_Init();

		//设置TTC0_0为滴答定时器，定时器开启后，每10ms检查一次日志
		TTC_Tick_Init(&TTC0_2_Timer, XPAR_XTTCPS_2_DEVICE_ID,
				XPS_TTC0_2_INT_ID, 10000, Debug_LOG_IRQ_Handler);
		TTC_Tick_Start(&TTC0_2_Timer);//开启定时器
	}

}

void LOGP()
{
	if(log_cnt && LOG_DEBUG)
	{
		//停止Fifo中断防止干扰
		XUartPs_WriteReg(Debug_UART.Config.BaseAddress, XUARTPS_IDR_OFFSET,
				XUARTPS_IXR_TXEMPTY);

		//赋予传输参数
		Debug_UART.SendBuffer.RequestedBytes = log_cnt;
		Debug_UART.SendBuffer.RemainingBytes = log_cnt;
		Debug_UART.SendBuffer.NextBytePtr = (uint8_t*)log_text;

		Debug_UART_SendBuffer(&Debug_UART);

		log_cnt = 0;
	}
}

void Debug_UART_Init()
{
	XUartPs_Config *Config;
	//在配置表中查找配置，然后对串口进行初始化
	Config = XUartPs_LookupConfig(DEBUG_UART_DEV_ID);
	XUartPs_CfgInitialize(&Debug_UART, Config, Config->BaseAddress);
	XUartPs_SetBaudRate(&Debug_UART, DEBUG_UART_BAUDRATE);
    //链接到中断控制器
    Set_ScuGic_Link(DEBUG_UART_INT_ID, 0xA0, HIGH_Level_Sensitive,
    		Debug_UART_IRQ_Handler, (void *)&Debug_UART);

}

void Debug_UART_SendBuffer(XUartPs *InstPtr)
{
	uint32_t Send_Cnt = 0;

	while ((!XUartPs_IsTransmitFull(InstPtr->Config.BaseAddress)) &&
		   (InstPtr->SendBuffer.RemainingBytes > Send_Cnt)) {

		/* 将数据填入FIFO */
		XUartPs_WriteReg(InstPtr->Config.BaseAddress,XUARTPS_FIFO_OFFSET,
				((uint32_t)InstPtr->SendBuffer.NextBytePtr[Send_Cnt]));

		Send_Cnt++;
	}

	InstPtr->SendBuffer.NextBytePtr += Send_Cnt;
	InstPtr->SendBuffer.RemainingBytes -= Send_Cnt;

	//打开TX FIFO空中断
	XUartPs_WriteReg(InstPtr->Config.BaseAddress,
			XUARTPS_IER_OFFSET,(uint32_t)XUARTPS_IXR_TXEMPTY);
}
