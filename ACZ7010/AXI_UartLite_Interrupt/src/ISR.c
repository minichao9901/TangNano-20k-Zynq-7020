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

//中断里使用的标志位
volatile int All_Send_Flag = 0;	//全部发送标志
volatile int All_Recv_Flag = 0;	//全部接收标志

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


void AXI_UART0_Send_IRQ_Handler(void *CallBackRef, unsigned int EventData)
{
	/* ↓↓↓用户处理↓↓↓ */
	All_Send_Flag = 1;
	/* ↑↑↑结束处理↑↑↑ */
}

void AXI_UART0_Recv_IRQ_Handler(void *CallBackRef, unsigned int EventData)
{
	/* ↓↓↓用户处理↓↓↓ */
	All_Recv_Flag = 1;
	/* ↑↑↑结束处理↑↑↑ */
}
