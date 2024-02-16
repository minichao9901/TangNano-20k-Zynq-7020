/*****************************************************************************
*	实验流程：
*	1.初始化AXI DMA
*	2.设置Tx和Rx中断
*	3.提交传输
*	4.等待传输完成
*	5.检查传输状态并校验数据
*	6.打印测试结果
******************************************************************************/

#include "COMMON.h"


#define RX_INTR_ID		XPAR_FABRIC_AXIDMA_0_S2MM_INTROUT_VEC_ID	//RX的中断号
#define TX_INTR_ID		XPAR_FABRIC_AXIDMA_0_MM2S_INTROUT_VEC_ID	//TX的中断号
#define TX_BUFFER_BASE	0x1200000				//TX缓冲区的基地址
#define RX_BUFFER_BASE	0x1400000				//RX缓冲区的基地址

int main(void)
{
	int Index;					//计数变量
	uint16_t Error_Cnt = 0;		//错误计数
	uint8_t *TxBufferPtr;		//传输数据的指针
	uint8_t *RxBufferPtr;		//接收数据的指针

	//将指针TxBufferPtr指向TX_BUFFER_BASE
	TxBufferPtr = (uint8_t *)TX_BUFFER_BASE;
	//将指针RxBufferPtr指向RX_BUFFER_BASE
	RxBufferPtr = (uint8_t *)RX_BUFFER_BASE;

	//初始化通用中断控制器
	ScuGic_Init();

	//初始化AXI DMA
	AXI_DMA_Init(&AxiDma0, XPAR_AXIDMA_0_DEVICE_ID);

	//设置中断服务函数
	AXI_DMA_TxInt_Init(&AxiDma0, TX_INTR_ID, TxIntrHandler);
	AXI_DMA_RxInt_Init(&AxiDma0, RX_INTR_ID, RxIntrHandler);

	//在开始传输测试之前清除中断状态标志
	TxDone = 0;
	RxDone = 0;
	Error = 0;

	//给TxBufferPtr赋值为0~255
	for(Index = 0; Index < 256; Index ++) {
		TxBufferPtr[Index] = Index;
	}

	//在DMA传输之前刷新Buffer
	Xil_DCacheFlushRange((UINTPTR)TxBufferPtr, 256);


	//开启数据传输
	XAxiDma_SimpleTransfer(&AxiDma0,(UINTPTR) TxBufferPtr,
				256, XAXIDMA_DMA_TO_DEVICE);

	XAxiDma_SimpleTransfer(&AxiDma0,(UINTPTR) RxBufferPtr,
				256, XAXIDMA_DEVICE_TO_DMA);

	//等待TX完成、或者RX完成、或者传输错误，否则一直等待
	while (!TxDone && !RxDone && !Error) {
		/* 等待 */
	}

	//如果发生错误，则打印失败提示
	if (Error) {
		printf("DMA Transfer Failed!\n");
	} else {
		//测试完成，检查数据
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
