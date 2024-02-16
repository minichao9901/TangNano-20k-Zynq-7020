#include "AXI_DMA.h"

XAxiDma AxiDma0;


/*****************************************************************************
* @brief	初始化AXI DMA
* @param	AxiDmaPtr	指向DMA实例的指针
* @param	DeviceId	AXI DMA的设备ID
* @Usage	AXI_DMA_Init(&AxiDma0, XPAR_AXIDMA_0_DEVICE_ID);
******************************************************************************/
void AXI_DMA_Init(XAxiDma *AxiDma, uint32_t DeviceId)
{
	XAxiDma_Config *Config;
	Config = XAxiDma_LookupConfig(DeviceId);
	XAxiDma_CfgInitialize(AxiDma, Config);

	//禁用所有中断
	XAxiDma_IntrDisable(AxiDma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);
	XAxiDma_IntrDisable(AxiDma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
}

/*****************************************************************************
* @brief	初始化DMA的Tx中断
* @param	AxiDmaPtr	指向DMA实例的指针
* @param	TxIntrId	TX通道中断ID
* @param	Handler		Tx通道中断处理函数
* @Usage	AXI_DMA_TxInt_Init(&AxiDma, TX_INTR_ID, TxIntrHandler);
******************************************************************************/
void AXI_DMA_TxInt_Init(XAxiDma *AxiDma, uint16_t TxIntrId, Xil_InterruptHandler Handler)
{
	//连接中断服务函数
	Set_ScuGic_Link(TxIntrId, 0xA0, Rising_Edge_Sensitive, Handler, (void *)AxiDma);

	//使能DMATx中断
	XAxiDma_IntrEnable(AxiDma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);
}

/*****************************************************************************
* @brief	初始化DMA的Rx中断
* @param	AxiDmaPtr	指向DMA实例的指针
* @param	RxIntrId	Rx通道中断ID
* @param	Handler		Rx通道中断处理函数
* @Usage	AXI_DMA_RxInt_Init(&AxiDma, RX_INTR_ID, RxIntrHandler);
******************************************************************************/
void AXI_DMA_RxInt_Init(XAxiDma *AxiDma, uint16_t RxIntrId, Xil_InterruptHandler Handler)
{
	//连接中断服务函数
	Set_ScuGic_Link(RxIntrId, 0xA0, Rising_Edge_Sensitive, Handler, (void *)AxiDma);

	//使能DMARx中断
	XAxiDma_IntrEnable(AxiDma, XAXIDMA_IRQ_ALL_MASK,XAXIDMA_DEVICE_TO_DMA);
}
