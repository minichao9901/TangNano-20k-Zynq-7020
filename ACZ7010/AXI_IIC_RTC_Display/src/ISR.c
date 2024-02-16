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

uint8_t Tube_Refresh_Flag = 1;	//数码管位刷新标志
uint8_t RTC_Refresh_Flag = 1;	//数码管位刷新标志
uint32_t Times_500ms = 0;	//500ms计时标志位

/**
  *****************************************************
  * @brief	私有定时器中断处理程序
  * @tag	本函数用来处理私有定时器中断，在内部加入用户程序即可
  *****************************************************
**/
void ScuTimer_IRQ_Handler(void *CallBackRef)
{
	/* ↓↓↓用户处理↓↓↓ */
	Tube_Refresh_Flag = 1;

	if(Times_500ms >= 499) {
		RTC_Refresh_Flag = 1;
		Times_500ms = 0;
	} else {
		Times_500ms++;
	}
	/* ↑↑↑结束处理↑↑↑ */
    XScuTimer_ClearInterruptStatus(&ScuTimer);
}

