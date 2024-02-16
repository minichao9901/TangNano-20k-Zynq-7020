/**
  *****************************************************************************
  * 					����û��жϴ�����������ͳһ����
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

//�ж���ʹ�õı�־λ
volatile int All_Send_Flag = 0;	//ȫ�����ͱ�־
volatile int All_Recv_Flag = 0;	//ȫ�����ձ�־

/**
  *****************************************************
  * @brief	˽�ж�ʱ���жϴ������
  * @tag	��������������˽�ж�ʱ���жϣ����ڲ������û����򼴿�
  *****************************************************
**/
void ScuTimer_IRQ_Handler(void *CallBackRef)
{
	/* �������û���������� */


	/* ������������������� */
    XScuTimer_ClearInterruptStatus(&ScuTimer);
}


void AXI_UART0_Send_IRQ_Handler(void *CallBackRef, unsigned int EventData)
{
	/* �������û���������� */
	All_Send_Flag = 1;
	/* ������������������� */
}

void AXI_UART0_Recv_IRQ_Handler(void *CallBackRef, unsigned int EventData)
{
	/* �������û���������� */
	All_Recv_Flag = 1;
	/* ������������������� */
}
