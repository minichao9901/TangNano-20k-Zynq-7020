#include"COMMON.h"
int main(void)
{
	uint32_t State;
	AXI_GPIO_Init(&AXI_GPIO0,GPIO_0_ID);	//初始化AXI GPIO0
	AXI_GPIO_Set_Channel(&AXI_GPIO0, XGPIO_IR_CH1_MASK, 0xFF, 0);//设置通道1为输入

	while(1){
		State = XGpio_DiscreteRead(&AXI_GPIO0,XGPIO_IR_CH1_MASK);//读取通道1输入的值
		AXI_GPIO_Set_Channel(&AXI_GPIO0, XGPIO_IR_CH2_MASK, 0, State);//将通道2设置为输出，输出从通道1读取的值
	}
	return 0;
}
