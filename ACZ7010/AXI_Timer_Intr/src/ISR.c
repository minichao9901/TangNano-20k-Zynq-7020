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

volatile uint16_t Cnt0 = 0;
volatile uint16_t Cnt1 = 0;

volatile uint16_t Cnt0_Flag = 0;
volatile uint16_t Cnt1_Flag = 0;

void AXI_Timer0_IRQ_Handler(void *CallBackRef, uint8_t TmrCtrNum)
{
	//Timer0的计数器0中断
	if (TmrCtrNum == TIMER_CNTR_0) {
	/* ↓↓↓用户处理↓↓↓ */
		Cnt0_Flag = 1;
		Cnt0++;

	/* ↑↑↑结束处理↑↑↑ */
	}

	//Timer0的计数器1中断
	if(TmrCtrNum == TIMER_CNTR_1) {
	/* ↓↓↓用户处理↓↓↓ */
		Cnt1_Flag = 1;
		Cnt1++;

	/* ↑↑↑结束处理↑↑↑ */

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

