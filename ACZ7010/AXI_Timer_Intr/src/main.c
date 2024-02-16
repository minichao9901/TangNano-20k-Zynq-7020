/**
  *****************************************************************************
  * 				创建一个AXI TIMER IP核并使用它的定时器中断功能
  *****************************************************************************
  *
  * @File   : main.c
  * @By     : Sun
  * @Version: V1.0
  * @Date   : 2022 / 05 / 19
  * @Shop	: https://xiaomeige.taobao.com/
  *
  *****************************************************************************
**/
#include "COMMON.h"


int main(void)
{
	//初始化通用中断控制器
	ScuGic_Init();
	//定时器初始化
	AXI_Timer_Init(&AXI_Timer0,XPAR_TMRCTR_0_DEVICE_ID);
	//定时器中断初始化
	AXI_Timer_Int_Init(&AXI_Timer0,TMRCTR_INTERRUPT_ID,AXI_Timer0_IRQ_Handler);
	//定时器计数器0配置为中断模式，自动重装载且向下计数，定时1s
	AXI_Timer_SetOption(&AXI_Timer0,TIMER_CNTR_0, XTC_INT_MODE_OPTION |
			XTC_AUTO_RELOAD_OPTION | XTC_DOWN_COUNT_OPTION,RELOAD_VAL_S(1));
	//定时器计数器1配置为中断模式，自动重装载且向下计数，定时0.5s
	AXI_Timer_SetOption(&AXI_Timer0,TIMER_CNTR_1, XTC_INT_MODE_OPTION |
			XTC_AUTO_RELOAD_OPTION | XTC_DOWN_COUNT_OPTION,RELOAD_VAL_S(0.5));

	while(1) {
		//判断计数器0的计数标志
		if(Cnt0_Flag) {
			Cnt0_Flag = 0;	//清除标志

			printf("Timer0:%d\n",Cnt0);
		}
		//判断计数器1的计数标志
		if(Cnt1_Flag) {
			Cnt1_Flag = 0;	//清除标志

			printf("Timer1:%d\n",Cnt1);
		}
	}
	return 0;
}
