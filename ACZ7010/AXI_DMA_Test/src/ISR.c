/**
  *****************************************************************************
  * 					存放用户中断处理函数，方便统一处理
  *****************************************************************************
  *
  * @File   : ISR.c
  * @By     : Sun
  * @Version: V0.5
  * @Date   : 2022 / 06 / 01
  * @Shop	: https://xiaomeige.taobao.com/
  *
  *****************************************************************************
**/

#include "ISR.h"

//中断处理标志
volatile int TxDone;
volatile int RxDone;
volatile int Error;

/*****************************************************************************
* @brief	DMA TX中断处理函数。
*			从硬件获取中断状态，对其进行确认：
*			如果发生任何错误，它将重置硬件。
*			如果中断完成，则将TxDone标志置为1
*
* @param	Callback是指向DMA引擎的TX通道的指针。
******************************************************************************/
void TxIntrHandler(void *Callback)
{
	uint32_t IrqStatus;
	int TimeOut;
	XAxiDma *AxiDmaInst = (XAxiDma *)Callback;

	//读取挂起的中断，获取被声明的中断的位掩码
	IrqStatus = XAxiDma_IntrGetIrq(AxiDmaInst, XAXIDMA_DMA_TO_DEVICE);

	//确认挂起的中断
	XAxiDma_IntrAckIrq(AxiDmaInst, IrqStatus, XAXIDMA_DMA_TO_DEVICE);

	//如果没有中断被断言，直接返回
	if (!(IrqStatus & XAXIDMA_IRQ_ALL_MASK))
	{
		return;
	}

	//如果断言中断错误，则拉高错误标志位，复位硬件以从错误中恢复，然后直接返回
	if ((IrqStatus & XAXIDMA_IRQ_ERROR_MASK))
	{
		Error = 1;//将错误标志置为1

		//复位DMA通道
		XAxiDma_Reset(AxiDmaInst);

		//装载循环计数器复位的超时次数
		TimeOut = RESET_TIMEOUT_COUNTER;

		//等待DMA复位完成，若复位超时则也会跳出等待，超时次数由开头用户自定义
		while (TimeOut)
		{
			if (XAxiDma_ResetIsDone(AxiDmaInst))//如果复位完成，则break跳出while循环
			{
				break;
			}

			TimeOut --;//超时次数减一，达到0时，while循环也会跳出
		}

		return;
	}

	//如果中断完成，则将TxDone标志置为1
	if ((IrqStatus & XAXIDMA_IRQ_IOC_MASK))
	{
		TxDone = 1;//将发送完成标志置为1
	}
}

/*****************************************************************************
* @brief	DMA RX中断处理函数
* 			它从硬件获取中断状态，对其进行确认：
* 			如果发生任何错误，它将重置硬件。
* 			否则，如果中断完成，则将RxDone标志置为1。
*
* @param	Callback是指向DMA引擎的RX通道的指针。
******************************************************************************/
void RxIntrHandler(void *Callback)
{
	uint32_t IrqStatus;
	int TimeOut;
	XAxiDma *AxiDmaInst = (XAxiDma *)Callback;

	//读取挂起的中断
	IrqStatus = XAxiDma_IntrGetIrq(AxiDmaInst, XAXIDMA_DEVICE_TO_DMA);

	//确认挂起的中断
	XAxiDma_IntrAckIrq(AxiDmaInst, IrqStatus, XAXIDMA_DEVICE_TO_DMA);

	//如果没有中断被断言，直接返回
	if (!(IrqStatus & XAXIDMA_IRQ_ALL_MASK))
	{
		return;
	}

	/*
	 * 如果断言中断错误，则将错误标志置1，
	 * 复位硬件以从错误中恢复，然后直接返回
	 */
	if ((IrqStatus & XAXIDMA_IRQ_ERROR_MASK))
	{
		Error = 1;//将错误标志置为1

		XAxiDma_Reset(AxiDmaInst);

		TimeOut = RESET_TIMEOUT_COUNTER;

		//等待DMA复位完成，若复位超时也会跳出等待，超时次数由开头用户自定义
		while (TimeOut)
		{
			if(XAxiDma_ResetIsDone(AxiDmaInst))//如果复位完成，则break跳出while循环
			{
				break;
			}

			TimeOut -= 1;//超时次数减一，达到0时，while循环也会跳出
		}
		return;
	}

	//如果中断完成，则将RxDone标志置为1
	if ((IrqStatus & XAXIDMA_IRQ_IOC_MASK))
	{
		RxDone = 1;//将接收完成标志置为1
	}
}


/**
  *****************************************************
  * @brief	私有定时器中断处理程序
  * @tag	本函数用来处理私有定时器中断，在内部加入用户程序即可
  *****************************************************
**/
void ScuTimer_IRQ_Handler(void *CallBackRef)
{
	/* ↓↓↓用户处理↓↓↓ */


	/* ↑↑↑结束处理↑↑↑ */
    XScuTimer_ClearInterruptStatus(&ScuTimer);
}
