/*****************************************************************************
*	ʵ�����̣�
*	1.��ʼ��AXI DMA
*	2.����Tx��Rx�ж�
*	3.�ύ����
*	4.�ȴ��������
*	5.��鴫��״̬��У������
*	6.��ӡ���Խ��
******************************************************************************/

#include "COMMON.h"


#define RX_INTR_ID		XPAR_FABRIC_AXIDMA_0_S2MM_INTROUT_VEC_ID	//RX���жϺ�
#define TX_INTR_ID		XPAR_FABRIC_AXIDMA_0_MM2S_INTROUT_VEC_ID	//TX���жϺ�
#define TX_BUFFER_BASE	0x1200000				//TX�������Ļ���ַ
#define RX_BUFFER_BASE	0x1400000				//RX�������Ļ���ַ

int main(void)
{
	int Index;					//��������
	uint16_t Error_Cnt = 0;		//�������
	uint8_t *TxBufferPtr;		//�������ݵ�ָ��
	uint8_t *RxBufferPtr;		//�������ݵ�ָ��

	//��ָ��TxBufferPtrָ��TX_BUFFER_BASE
	TxBufferPtr = (uint8_t *)TX_BUFFER_BASE;
	//��ָ��RxBufferPtrָ��RX_BUFFER_BASE
	RxBufferPtr = (uint8_t *)RX_BUFFER_BASE;

	//��ʼ��ͨ���жϿ�����
	ScuGic_Init();

	//��ʼ��AXI DMA
	AXI_DMA_Init(&AxiDma0, XPAR_AXIDMA_0_DEVICE_ID);

	//�����жϷ�����
	AXI_DMA_TxInt_Init(&AxiDma0, TX_INTR_ID, TxIntrHandler);
	AXI_DMA_RxInt_Init(&AxiDma0, RX_INTR_ID, RxIntrHandler);

	//�ڿ�ʼ�������֮ǰ����ж�״̬��־
	TxDone = 0;
	RxDone = 0;
	Error = 0;

	//��TxBufferPtr��ֵΪ0~255
	for(Index = 0; Index < 256; Index ++) {
		TxBufferPtr[Index] = Index;
	}

	//��DMA����֮ǰˢ��Buffer
	Xil_DCacheFlushRange((UINTPTR)TxBufferPtr, 256);


	//�������ݴ���
	XAxiDma_SimpleTransfer(&AxiDma0,(UINTPTR) TxBufferPtr,
				256, XAXIDMA_DMA_TO_DEVICE);

	XAxiDma_SimpleTransfer(&AxiDma0,(UINTPTR) RxBufferPtr,
				256, XAXIDMA_DEVICE_TO_DMA);

	//�ȴ�TX��ɡ�����RX��ɡ����ߴ�����󣬷���һֱ�ȴ�
	while (!TxDone && !RxDone && !Error) {
		/* �ȴ� */
	}

	//��������������ӡʧ����ʾ
	if (Error) {
		printf("DMA Transfer Failed!\n");
	} else {
		//������ɣ��������
		for(Index = 0; Index < 256; Index++)
		{
			if(RxBufferPtr[Index] != Index) {
				Error_Cnt++;
			}
		}
		if(Error_Cnt > 0)
			printf("Test Failed!\n");
		else
			printf("Test Successfully!\n");
	}

	return 0;
}
