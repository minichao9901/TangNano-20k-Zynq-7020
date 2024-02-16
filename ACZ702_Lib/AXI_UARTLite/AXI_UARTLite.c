/**
  *****************************************************************************
  * 						AXI UARTLite库
  *****************************************************************************
  *
  * @File   : AXI_UARTLite.c
  * @By     : Sun
  * @Version: V1.4
  * @Date   : 2022 / 06 / 07
  * @Shop	: https://xiaomeige.taobao.com/
  *
  *****************************************************************************
**/


#include "AXI_UARTLite.h"

//AXI UARTLite实例
XUartLite AXI_UART0;


/***********************************************************************************************************
 * @brief:	初始化AXI UARTLite设备
 * @param:	UartInstPtr		UART对象
 * @param:	DeviceId		UART设备ID
 * Sample:	AXI_UartLite_Init(&AXI_UART0, XPAR_AXI_UARTLITE_0_DEVICE_ID);
***********************************************************************************************************/
void AXI_UartLite_Init(XUartLite *UartInstPtr, uint16_t DeviceId)
{
	XUartLite_Initialize(UartInstPtr, DeviceId );
}

/***********************************************************************************************************
 * @brief:	初始化AXI UARTLite中断（初始化AXI UARTLite中断前，需先初始化AXI UARTLite）
 * @param:	UartInstPtr		UART对象
 * @param:	UartIntrId		UART中断ID
 * @param:	SendHandler		UART发送完成中断处理函数
 * @param:	RecvHandler		UART接收完成中断处理函数
 * @return:	无
 * Sample:	//初始化AXI_UART中断，并配置它的中断处理函数
			AXI_UARTLite_Intr_Init(&AXI_UART0, XPAR_FABRIC_AXI_UARTLITE_0_INTERRUPT_INTR,
					AXI_UART0_Send_IRQ_Handler,AXI_UART0_Recv_IRQ_Handler);
***********************************************************************************************************/
void AXI_UARTLite_Intr_Init(XUartLite *UartInstPtr,uint16_t UartIntrId,
		XUartLite_Handler SendHandler,XUartLite_Handler RecvHandler)
{
	Set_ScuGic_Link(UartIntrId, 0xA8, Rising_Edge_Sensitive,
			(Xil_InterruptHandler)XUartLite_InterruptHandler, (void *)UartInstPtr);

	XUartLite_SetSendHandler(UartInstPtr, (XUartLite_Handler)SendHandler, UartInstPtr);
	XUartLite_SetRecvHandler(UartInstPtr, (XUartLite_Handler)RecvHandler, UartInstPtr);

	XUartLite_EnableInterrupt(UartInstPtr);
}

/***********************************************************************************************************
 * @brief:	使用AXI UARTLite发送数据，数据需在0~65535之间
 * @param:	UartInstPtr		UART对象
 * @param:	data			数据
 * @return:	无
 * Sample:	AXI_UARTLite_SendData(&UartPs,12345); //发送数据12345
***********************************************************************************************************/
void AXI_UARTLite_SendData(XUartLite *UartInstPtr,uint32_t data)
{
	uint8_t SendData[2];
	int data_H,data_L;
	data_H = data>>8;
	data_L = data&0x00ff;
	if(data_H == 0)//如果高位为0
	{
		SendData[0] = data;
		XUartLite_Send(UartInstPtr, SendData, 1);
	}
	else
	{
		SendData[0] = data_H;
		SendData[1] = data_L;
		XUartLite_Send(UartInstPtr, SendData, 2);
	}
}

/***********************************************************************************************************
 * @brief:	使用AXI UARTLite发送字符串
 * @param:	UartInstPtr		UART对象
 * @param:	str				字符串地址
 * @return:	无
 * Sample:	AXI_UARTLite_SendString(&UartPs,"1234567"); //发送了7个字节
***********************************************************************************************************/
void AXI_UARTLite_SendString(XUartLite *UartInstPtr, char *str)
{
	XUartLite_Send(UartInstPtr, (uint8_t *)str, strlen(str));
}

/***********************************************************************************************************
 * @brief:	使用AXI UARTLite接收数据
 * @param:	UartInstPtr		UART对象
 * @param:	Buffer			接收数据存储区
 * @param:	Len				接收数据的长度
 * @return:	无
 * Sample:	AXI_UARTLite_RecvData(&UartPs, RecvBuffer,2);接收两个数据，并将数据存入RecvBuffer数组
***********************************************************************************************************/
void AXI_UARTLite_RecvData(XUartLite *UartInstPtr, uint8_t *Buffer,uint32_t Len)
{
	XUartLite_Recv(UartInstPtr, Buffer, Len);
}
