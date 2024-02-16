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

extern uint8_t Dig_Data[8][2];
extern uint8_t ADC_Channel;
static uint8_t i,Flag_200ms;	//200ms标志位


/**
  *****************************************************
  * @brief	私有定时器中断处理程序
  * @tag	本函数用来处理私有定时器中断，在内部加入用户程序即可
  *****************************************************
**/
void ScuTimer_IRQ_Handler(void *CallBackRef)
{
	/* ↓↓↓用户处理↓↓↓ */

    //每过200ms检测一次按键
    if (Flag_200ms < 100) {
    	Flag_200ms++;
    } else {
    	Flag_200ms = 0;
    	if((!PS_GPIO_GetPort(54)) && ADC_Channel < 7)
    		ADC_Channel++;
    	else if ((!PS_GPIO_GetPort(55)) && ADC_Channel > 0)
    		ADC_Channel--;
    }

    //每过2ms用SPI控制数码管显示第i位数据
	PS_SPI_Transfer(&SPI1, 0, 0,Dig_Data[i], 2);
	if(i<8)
		i++;
	else
		i = 0;

	/* ↑↑↑结束处理↑↑↑ */
    XScuTimer_ClearInterruptStatus(&ScuTimer);
}
