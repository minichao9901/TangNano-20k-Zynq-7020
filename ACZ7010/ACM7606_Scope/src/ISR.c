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

//����ʱ���־λ
static uint8_t Flag_20ms = 0;
static uint8_t Flag_30ms = 0;
static uint8_t Flag_50ms = 0;
static uint8_t Flag_100ms = 0;
static uint8_t Flag_200ms = 0;
static uint8_t Flag_500ms = 0;

//����������̵ı�־λ
uint8_t Flag_DrawWave = 0;		//���Ʋ��Σ�10msһ��
uint8_t Flag_DrawGrid = 0;		//��������20msһ��
uint8_t Flag_TouchScan = 0;		//����ɨ�裬30msһ��
uint8_t Flag_Refresh_Val = 0;	//ˢ�µ�ѹ��Ƶ����ֵ��500msһ��

/**
  *****************************************************
  * @brief	˽�ж�ʱ���жϴ������
  * @tag	��������������˽�ж�ʱ���жϣ����ڲ������û����򼴿�
  *****************************************************
**/
void ScuTimer_IRQ_Handler(void *CallBackRef)
{
	/* �������û���������� */
	//����ͨ����ʱ������ÿ�����̵Ĵ��������������Ӧ�ı�־λ
	Flag_DrawWave = 1;

	if(Flag_20ms >= 1) {
		Flag_20ms = 0;
		Flag_DrawGrid = 1;
	}
	else
		Flag_20ms++;

	if(Flag_30ms >= 2) {
		Flag_30ms = 0;
		Flag_TouchScan = 1;
	}
	else
		Flag_30ms++;

	if(Flag_50ms >= 4) {
		Flag_50ms = 0;

	}
	else
		Flag_50ms++;

	if(Flag_100ms >= 9) {
		Flag_100ms = 0;

	}
	else
		Flag_100ms++;

	if(Flag_200ms >= 19) {
		Flag_200ms = 0;

	}
	else
		Flag_200ms++;

	if(Flag_500ms >= 49) {
		Flag_500ms = 0;
		Flag_Refresh_Val = 1;
	}
	else
		Flag_500ms++;


	/* ������������������� */
    XScuTimer_ClearInterruptStatus(&ScuTimer);
}

//��־�жϴ���������ڽ��¼�֪ͨӦ�ó���������
volatile int RxDone;
volatile int Error;

//DMA��Rx������жϷ�����
void AXI_DMARx_IRQHandler(void *Callback)
{
	uint32_t IrqStatus;

	//��ȡ������ж�
	IrqStatus = XAxiDma_IntrGetIrq(&AxiDma0, XAXIDMA_DEVICE_TO_DMA);

	//ȷ�Ϲ�����ж�
	XAxiDma_IntrAckIrq(&AxiDma0, IrqStatus, XAXIDMA_DEVICE_TO_DMA);

	//���û�з����жϣ���ִ���κβ���
	if (!(IrqStatus & XAXIDMA_IRQ_ALL_MASK)) {
		return;
	}

	//�������������λӲ������������������
	if ((IrqStatus & XAXIDMA_IRQ_ERROR_MASK)) {

		Error = 1;
		//printf("RX Error!\n");

		//��λ
		XAxiDma_Reset(&AxiDma0);
		//�ȴ���λ���
		while(!XAxiDma_ResetIsDone(&AxiDma0));
		//ʹ��DMA��Rx�ж�
		XAxiDma_IntrEnable(&AxiDma0, XAXIDMA_IRQ_ALL_MASK,XAXIDMA_DEVICE_TO_DMA);
		//������һ�ִ���
		XAxiDma_SimpleTransfer(&AxiDma0,(UINTPTR) P_ADC_Data,
				ADC_DATA_LENGTH*2, XAXIDMA_DEVICE_TO_DMA);

		return;
	}

	//�������жϱ����ԣ���RxDone��1
	if ((IrqStatus & XAXIDMA_IRQ_IOC_MASK)) {

		RxDone = 1;
		//printf("RX Done!\n");
	}
}
