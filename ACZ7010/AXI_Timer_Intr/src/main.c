/**
  *****************************************************************************
  * 				����һ��AXI TIMER IP�˲�ʹ�����Ķ�ʱ���жϹ���
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
	//��ʼ��ͨ���жϿ�����
	ScuGic_Init();
	//��ʱ����ʼ��
	AXI_Timer_Init(&AXI_Timer0,XPAR_TMRCTR_0_DEVICE_ID);
	//��ʱ���жϳ�ʼ��
	AXI_Timer_Int_Init(&AXI_Timer0,TMRCTR_INTERRUPT_ID,AXI_Timer0_IRQ_Handler);
	//��ʱ��������0����Ϊ�ж�ģʽ���Զ���װ�������¼�������ʱ1s
	AXI_Timer_SetOption(&AXI_Timer0,TIMER_CNTR_0, XTC_INT_MODE_OPTION |
			XTC_AUTO_RELOAD_OPTION | XTC_DOWN_COUNT_OPTION,RELOAD_VAL_S(1));
	//��ʱ��������1����Ϊ�ж�ģʽ���Զ���װ�������¼�������ʱ0.5s
	AXI_Timer_SetOption(&AXI_Timer0,TIMER_CNTR_1, XTC_INT_MODE_OPTION |
			XTC_AUTO_RELOAD_OPTION | XTC_DOWN_COUNT_OPTION,RELOAD_VAL_S(0.5));

	while(1) {
		//�жϼ�����0�ļ�����־
		if(Cnt0_Flag) {
			Cnt0_Flag = 0;	//�����־

			printf("Timer0:%d\n",Cnt0);
		}
		//�жϼ�����1�ļ�����־
		if(Cnt1_Flag) {
			Cnt1_Flag = 0;	//�����־

			printf("Timer1:%d\n",Cnt1);
		}
	}
	return 0;
}
