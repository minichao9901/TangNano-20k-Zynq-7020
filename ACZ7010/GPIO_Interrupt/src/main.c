#include "COMMON.h"


int main(void)
{
	//GIC通用中断控制器初始化
	ScuGic_Init();

	//LOG调试功能初始化
	Log_Debug_Init();

	//初始化AXI GPIO 驱动程序
	AXI_GPIO_Init(&AXI_GPIO0, XPAR_AXI_GPIO_0_DEVICE_ID);

	//设置KEY_S1方向
	AXI_GPIO_SetPin_Dir(&AXI_GPIO0,XGPIO_IR_CH1_MASK,KEY_S1,INPUT);

	//初始化AXI GPIO中断
	AXI_GPIO_Intc_Init(&AXI_GPIO0,XPAR_FABRIC_AXI_GPIO_0_IP2INTC_IRPT_INTR,
			XGPIO_IR_CH1_MASK, AXI_GPIO_IRQ_Handler);

	//初始化PS端GPIO
	PS_GPIO_Init();

	//设置PS_KEY(MIO47)方向为输入
	PS_GPIO_SetMode(PS_KEY, INPUT, 0);

	//初始化PS GPIO中断
	PS_GPIO_Int_Init(PS_GPIO_IRQ_Handler);

	//设置按键PS_KEY的中断为边沿敏感
	PS_GPIO_SetInt(PS_KEY,XGPIOPS_IRQ_TYPE_EDGE_BOTH);

	//设置TTC0_1为滴答定时器，用来做按键延时消抖，中断触发间隔为10ms，等待开启
	TTC_Tick_Init(&TTC0_1_Timer, XPAR_XTTCPS_1_DEVICE_ID, XPS_TTC0_1_INT_ID,
			10000, TTC0_1_IRQ_Handler);

	while(1)
	{

	}
	return 0;
}
