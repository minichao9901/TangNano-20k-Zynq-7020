#include "COMMON.h"


int main(void)
{
	//GICͨ���жϿ�������ʼ��
	ScuGic_Init();

	//LOG���Թ��ܳ�ʼ��
	Log_Debug_Init();

	//��ʼ��AXI GPIO ��������
	AXI_GPIO_Init(&AXI_GPIO0, XPAR_AXI_GPIO_0_DEVICE_ID);

	//����KEY_S1����
	AXI_GPIO_SetPin_Dir(&AXI_GPIO0,XGPIO_IR_CH1_MASK,KEY_S1,INPUT);

	//��ʼ��AXI GPIO�ж�
	AXI_GPIO_Intc_Init(&AXI_GPIO0,XPAR_FABRIC_AXI_GPIO_0_IP2INTC_IRPT_INTR,
			XGPIO_IR_CH1_MASK, AXI_GPIO_IRQ_Handler);

	//��ʼ��PS��GPIO
	PS_GPIO_Init();

	//����PS_KEY(MIO47)����Ϊ����
	PS_GPIO_SetMode(PS_KEY, INPUT, 0);

	//��ʼ��PS GPIO�ж�
	PS_GPIO_Int_Init(PS_GPIO_IRQ_Handler);

	//���ð���PS_KEY���ж�Ϊ��������
	PS_GPIO_SetInt(PS_KEY,XGPIOPS_IRQ_TYPE_EDGE_BOTH);

	//����TTC0_1Ϊ�δ�ʱ����������������ʱ�������жϴ������Ϊ10ms���ȴ�����
	TTC_Tick_Init(&TTC0_1_Timer, XPAR_XTTCPS_1_DEVICE_ID, XPS_TTC0_1_INT_ID,
			10000, TTC0_1_IRQ_Handler);

	while(1)
	{

	}
	return 0;
}
