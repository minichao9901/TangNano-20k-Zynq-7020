#include "AXI_DMA.h"

XAxiDma AxiDma0;



//��ʼ��DMA	AXI_DMA_Init(&AxiDma0,XPAR_AXIDMA_0_DEVICE_ID);
void AXI_DMA_Init(XAxiDma *AxiDma, uint32_t DeviceId)
{
	XAxiDma_Config *Config;
	Config = XAxiDma_LookupConfig(DeviceId);
	XAxiDma_CfgInitialize(AxiDma, Config);
}
//��ʼ��DMA��Rx�ж�
void AXI_DMA_RxInt_Init(XAxiDma *AxiDma, uint16_t RxIntrId, Xil_InterruptHandler Handler)
{
	//�����жϷ�����
	Set_ScuGic_Link(RxIntrId, 0xA8, HIGH_Level_Sensitive, Handler, (void *)AxiDma);

	//����DMATx�ж�
	XAxiDma_IntrDisable(AxiDma, XAXIDMA_IRQ_ALL_MASK,XAXIDMA_DMA_TO_DEVICE);

	//ʹ��DMARx�ж�
	XAxiDma_IntrEnable(AxiDma, XAXIDMA_IRQ_ALL_MASK,XAXIDMA_DEVICE_TO_DMA);
}
