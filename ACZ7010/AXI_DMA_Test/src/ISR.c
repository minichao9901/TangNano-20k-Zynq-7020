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

//�жϴ����־
volatile int TxDone;
volatile int RxDone;
volatile int Error;

/*****************************************************************************
* @brief	DMA TX�жϴ�������
*			��Ӳ����ȡ�ж�״̬���������ȷ�ϣ�
*			��������κδ�����������Ӳ����
*			����ж���ɣ���TxDone��־��Ϊ1
*
* @param	Callback��ָ��DMA�����TXͨ����ָ�롣
******************************************************************************/
void TxIntrHandler(void *Callback)
{
	uint32_t IrqStatus;
	int TimeOut;
	XAxiDma *AxiDmaInst = (XAxiDma *)Callback;

	//��ȡ������жϣ���ȡ���������жϵ�λ����
	IrqStatus = XAxiDma_IntrGetIrq(AxiDmaInst, XAXIDMA_DMA_TO_DEVICE);

	//ȷ�Ϲ�����ж�
	XAxiDma_IntrAckIrq(AxiDmaInst, IrqStatus, XAXIDMA_DMA_TO_DEVICE);

	//���û���жϱ����ԣ�ֱ�ӷ���
	if (!(IrqStatus & XAXIDMA_IRQ_ALL_MASK))
	{
		return;
	}

	//��������жϴ��������ߴ����־λ����λӲ���ԴӴ����лָ���Ȼ��ֱ�ӷ���
	if ((IrqStatus & XAXIDMA_IRQ_ERROR_MASK))
	{
		Error = 1;//�������־��Ϊ1

		//��λDMAͨ��
		XAxiDma_Reset(AxiDmaInst);

		//װ��ѭ����������λ�ĳ�ʱ����
		TimeOut = RESET_TIMEOUT_COUNTER;

		//�ȴ�DMA��λ��ɣ�����λ��ʱ��Ҳ�������ȴ�����ʱ�����ɿ�ͷ�û��Զ���
		while (TimeOut)
		{
			if (XAxiDma_ResetIsDone(AxiDmaInst))//�����λ��ɣ���break����whileѭ��
			{
				break;
			}

			TimeOut --;//��ʱ������һ���ﵽ0ʱ��whileѭ��Ҳ������
		}

		return;
	}

	//����ж���ɣ���TxDone��־��Ϊ1
	if ((IrqStatus & XAXIDMA_IRQ_IOC_MASK))
	{
		TxDone = 1;//��������ɱ�־��Ϊ1
	}
}

/*****************************************************************************
* @brief	DMA RX�жϴ�����
* 			����Ӳ����ȡ�ж�״̬���������ȷ�ϣ�
* 			��������κδ�����������Ӳ����
* 			��������ж���ɣ���RxDone��־��Ϊ1��
*
* @param	Callback��ָ��DMA�����RXͨ����ָ�롣
******************************************************************************/
void RxIntrHandler(void *Callback)
{
	uint32_t IrqStatus;
	int TimeOut;
	XAxiDma *AxiDmaInst = (XAxiDma *)Callback;

	//��ȡ������ж�
	IrqStatus = XAxiDma_IntrGetIrq(AxiDmaInst, XAXIDMA_DEVICE_TO_DMA);

	//ȷ�Ϲ�����ж�
	XAxiDma_IntrAckIrq(AxiDmaInst, IrqStatus, XAXIDMA_DEVICE_TO_DMA);

	//���û���жϱ����ԣ�ֱ�ӷ���
	if (!(IrqStatus & XAXIDMA_IRQ_ALL_MASK))
	{
		return;
	}

	/*
	 * ��������жϴ����򽫴����־��1��
	 * ��λӲ���ԴӴ����лָ���Ȼ��ֱ�ӷ���
	 */
	if ((IrqStatus & XAXIDMA_IRQ_ERROR_MASK))
	{
		Error = 1;//�������־��Ϊ1

		XAxiDma_Reset(AxiDmaInst);

		TimeOut = RESET_TIMEOUT_COUNTER;

		//�ȴ�DMA��λ��ɣ�����λ��ʱҲ�������ȴ�����ʱ�����ɿ�ͷ�û��Զ���
		while (TimeOut)
		{
			if(XAxiDma_ResetIsDone(AxiDmaInst))//�����λ��ɣ���break����whileѭ��
			{
				break;
			}

			TimeOut -= 1;//��ʱ������һ���ﵽ0ʱ��whileѭ��Ҳ������
		}
		return;
	}

	//����ж���ɣ���RxDone��־��Ϊ1
	if ((IrqStatus & XAXIDMA_IRQ_IOC_MASK))
	{
		RxDone = 1;//��������ɱ�־��Ϊ1
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
