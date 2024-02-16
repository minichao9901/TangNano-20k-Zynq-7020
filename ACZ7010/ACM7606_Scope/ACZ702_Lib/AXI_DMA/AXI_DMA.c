#include "AXI_DMA.h"

XAxiDma AxiDma0;



//初始化DMA	AXI_DMA_Init(&AxiDma0,XPAR_AXIDMA_0_DEVICE_ID);
void AXI_DMA_Init(XAxiDma *AxiDma, uint32_t DeviceId)
{
	XAxiDma_Config *Config;
	Config = XAxiDma_LookupConfig(DeviceId);
	XAxiDma_CfgInitialize(AxiDma, Config);
}
//初始化DMA的Rx中断
void AXI_DMA_RxInt_Init(XAxiDma *AxiDma, uint16_t RxIntrId, Xil_InterruptHandler Handler)
{
	//连接中断服务函数
	Set_ScuGic_Link(RxIntrId, 0xA8, HIGH_Level_Sensitive, Handler, (void *)AxiDma);

	//禁用DMATx中断
	XAxiDma_IntrDisable(AxiDma, XAXIDMA_IRQ_ALL_MASK,XAXIDMA_DMA_TO_DEVICE);

	//使能DMARx中断
	XAxiDma_IntrEnable(AxiDma, XAXIDMA_IRQ_ALL_MASK,XAXIDMA_DEVICE_TO_DMA);
}
