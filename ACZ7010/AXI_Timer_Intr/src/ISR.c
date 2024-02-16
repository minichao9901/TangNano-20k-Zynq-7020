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

volatile uint16_t Cnt0 = 0;
volatile uint16_t Cnt1 = 0;

volatile uint16_t Cnt0_Flag = 0;
volatile uint16_t Cnt1_Flag = 0;

void AXI_Timer0_IRQ_Handler(void *CallBackRef, uint8_t TmrCtrNum)
{
	//Timer0�ļ�����0�ж�
	if (TmrCtrNum == TIMER_CNTR_0) {
	/* �������û���������� */
		Cnt0_Flag = 1;
		Cnt0++;

	/* ������������������� */
	}

	//Timer0�ļ�����1�ж�
	if(TmrCtrNum == TIMER_CNTR_1) {
	/* �������û���������� */
		Cnt1_Flag = 1;
		Cnt1++;

	/* ������������������� */

	}
}


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

