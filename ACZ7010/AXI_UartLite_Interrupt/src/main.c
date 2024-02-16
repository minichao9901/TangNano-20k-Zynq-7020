/**
  *****************************************************************************
  * 				使用PL端的AXI_UARTLite及其中断实现数据发送和接收处理
  * 				帧头'S' 帧尾'E' 中间有效数据长度为8字节，总长10字节
  *****************************************************************************
  *
  * @File   : main.c
  * @By     : Sun
  * @Version: V1.0
  * @Date   : 2022 / 06 / 07
  * @Shop	: https://xiaomeige.taobao.com/
  *
  *****************************************************************************
**/
#include "COMMON.h"

uint8_t Receive_Buffer[10];

int main(void)
{
	uint8_t Data[10];
	uint32_t Timeout;
	uint8_t i;

	//开启通用中断控制器
	ScuGic_Init();

	//初始化AXI_UART
	AXI_UartLite_Init(&AXI_UART0, XPAR_AXI_UARTLITE_0_DEVICE_ID);

	//初始化AXI_UART中断
	AXI_UARTLite_Intr_Init(&AXI_UART0, XPAR_FABRIC_AXI_UARTLITE_0_INTERRUPT_INTR,
			AXI_UART0_Send_IRQ_Handler,AXI_UART0_Recv_IRQ_Handler);

	//UART发送
	AXI_UARTLite_SendString(&AXI_UART0,"00 01 02 03 04 05 06 07 08 09 10 "
			"11 12 13 14 15 16 17 18 19 20 21 22 23 24 25\n");

	//等待上一轮发送完成
	while(!All_Send_Flag);
	All_Send_Flag = 0;

	AXI_UARTLite_SendString(&AXI_UART0,"UART send done!!!\n");

	//等待上一轮发送完成
	while(!All_Send_Flag);
	All_Send_Flag = 0;

	while(1) {
		//UART接收10个字符
		AXI_UARTLite_RecvData(&AXI_UART0, Receive_Buffer, 10);

		//等待10字节接收完成或接收超时
		Timeout = 10000;
		while((!All_Recv_Flag) && (Timeout)) {
			usleep(10);
			Timeout--;
		}

		if(All_Recv_Flag) {
			//清除接收完成标志
			All_Recv_Flag = 0;

			//处理接受到的数据，帧头'S' 帧尾'E'，中间有效数据长度为8字节
			if((Receive_Buffer[0] == 'S') && (Receive_Buffer[9] == 'E')) {
				for(i=0;i<8;i++) {
					Data[i] = Receive_Buffer[i+1];
				}

				//回发8位有效数据
				XUartLite_Send(&AXI_UART0,Data,8);

				//换行
				AXI_UARTLite_SendString(&AXI_UART0,"\n");
			}
		}
	}

	return 0;
}
