#include"COMMON.h"
int main(void)
{
	uint32_t State;
	AXI_GPIO_Init(&AXI_GPIO0,GPIO_0_ID);	//��ʼ��AXI GPIO0
	AXI_GPIO_Set_Channel(&AXI_GPIO0, XGPIO_IR_CH1_MASK, 0xFF, 0);//����ͨ��1Ϊ����

	while(1){
		State = XGpio_DiscreteRead(&AXI_GPIO0,XGPIO_IR_CH1_MASK);//��ȡͨ��1�����ֵ
		AXI_GPIO_Set_Channel(&AXI_GPIO0, XGPIO_IR_CH2_MASK, 0, State);//��ͨ��2����Ϊ����������ͨ��1��ȡ��ֵ
	}
	return 0;
}
