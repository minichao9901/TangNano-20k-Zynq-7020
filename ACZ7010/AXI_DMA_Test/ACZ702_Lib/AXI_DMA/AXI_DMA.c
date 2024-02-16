#include "AXI_DMA.h"

XAxiDma AxiDma0;


/*****************************************************************************
* @brief	��ʼ��AXI DMA
* @param	AxiDmaPtr	ָ��DMAʵ����ָ��
* @param	DeviceId	AXI DMA���豸ID
* @Usage	AXI_DMA_Init(&AxiDma0, XPAR_AXIDMA_0_DEVICE_ID);
******************************************************************************/
void AXI_DMA_Init(XAxiDma *AxiDma, uint32_t DeviceId)
{
	XAxiDma_Config *Config;
	Config = XAxiDma_LookupConfig(DeviceId);
	XAxiDma_CfgInitialize(AxiDma, Config);

	//���������ж�
	XAxiDma_IntrDisable(AxiDma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);
	XAxiDma_IntrDisable(AxiDma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
}

/*****************************************************************************
* @brief	��ʼ��DMA��Tx�ж�
* @param	AxiDmaPtr	ָ��DMAʵ����ָ��
* @param	TxIntrId	TXͨ���ж�ID
* @param	Handler		Txͨ���жϴ�����
* @Usage	AXI_DMA_TxInt_Init(&AxiDma, TX_INTR_ID, TxIntrHandler);
******************************************************************************/
void AXI_DMA_TxInt_Init(XAxiDma *AxiDma, uint16_t TxIntrId, Xil_InterruptHandler Handler)
{
	//�����жϷ�����
	Set_ScuGic_Link(TxIntrId, 0xA0, Rising_Edge_Sensitive, Handler, (void *)AxiDma);

	//ʹ��DMATx�ж�
	XAxiDma_IntrEnable(AxiDma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);
}

/*****************************************************************************
* @brief	��ʼ��DMA��Rx�ж�
* @param	AxiDmaPtr	ָ��DMAʵ����ָ��
* @param	RxIntrId	Rxͨ���ж�ID
* @param	Handler		Rxͨ���жϴ�����
* @Usage	AXI_DMA_RxInt_Init(&AxiDma, RX_INTR_ID, RxIntrHandler);
******************************************************************************/
void AXI_DMA_RxInt_Init(XAxiDma *AxiDma, uint16_t RxIntrId, Xil_InterruptHandler Handler)
{
	//�����жϷ�����
	Set_ScuGic_Link(RxIntrId, 0xA0, Rising_Edge_Sensitive, Handler, (void *)AxiDma);

	//ʹ��DMARx�ж�
	XAxiDma_IntrEnable(AxiDma, XAXIDMA_IRQ_ALL_MASK,XAXIDMA_DEVICE_TO_DMA);
}
