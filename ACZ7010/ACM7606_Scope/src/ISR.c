/**
  *****************************************************************************
  * 					存放用户中断处理函数，方便统一处理
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

//各个时间标志位
static uint8_t Flag_20ms = 0;
static uint8_t Flag_30ms = 0;
static uint8_t Flag_50ms = 0;
static uint8_t Flag_100ms = 0;
static uint8_t Flag_200ms = 0;
static uint8_t Flag_500ms = 0;

//定义各个进程的标志位
uint8_t Flag_DrawWave = 0;		//绘制波形，10ms一次
uint8_t Flag_DrawGrid = 0;		//绘制网格，20ms一次
uint8_t Flag_TouchScan = 0;		//触摸扫描，30ms一次
uint8_t Flag_Refresh_Val = 0;	//刷新电压和频率数值，500ms一次

/**
  *****************************************************
  * @brief	私有定时器中断处理程序
  * @tag	本函数用来处理私有定时器中断，在内部加入用户程序即可
  *****************************************************
**/
void ScuTimer_IRQ_Handler(void *CallBackRef)
{
	/* ↓↓↓用户处理↓↓↓ */
	//以下通过计时器控制每个进程的触发间隔，启动对应的标志位
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


	/* ↑↑↑结束处理↑↑↑ */
    XScuTimer_ClearInterruptStatus(&ScuTimer);
}

//标志中断处理程序用于将事件通知应用程序上下文
volatile int RxDone;
volatile int Error;

//DMA的Rx传输的中断服务函数
void AXI_DMARx_IRQHandler(void *Callback)
{
	uint32_t IrqStatus;

	//读取挂起的中断
	IrqStatus = XAxiDma_IntrGetIrq(&AxiDma0, XAXIDMA_DEVICE_TO_DMA);

	//确认挂起的中断
	XAxiDma_IntrAckIrq(&AxiDma0, IrqStatus, XAXIDMA_DEVICE_TO_DMA);

	//如果没有发生中断，则不执行任何操作
	if (!(IrqStatus & XAXIDMA_IRQ_ALL_MASK)) {
		return;
	}

	//如果发生错误则复位硬件，并重新启动传输
	if ((IrqStatus & XAXIDMA_IRQ_ERROR_MASK)) {

		Error = 1;
		//printf("RX Error!\n");

		//复位
		XAxiDma_Reset(&AxiDma0);
		//等待复位完成
		while(!XAxiDma_ResetIsDone(&AxiDma0));
		//使能DMA的Rx中断
		XAxiDma_IntrEnable(&AxiDma0, XAXIDMA_IRQ_ALL_MASK,XAXIDMA_DEVICE_TO_DMA);
		//启动新一轮传输
		XAxiDma_SimpleTransfer(&AxiDma0,(UINTPTR) P_ADC_Data,
				ADC_DATA_LENGTH*2, XAXIDMA_DEVICE_TO_DMA);

		return;
	}

	//如果完成中断被断言，将RxDone置1
	if ((IrqStatus & XAXIDMA_IRQ_IOC_MASK)) {

		RxDone = 1;
		//printf("RX Done!\n");
	}
}
