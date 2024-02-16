/**
  *****************************************************************************
  * 						AXI UARTLite��
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

//AXI UARTLiteʵ��
XUartLite AXI_UART0;


/***********************************************************************************************************
 * @brief:	��ʼ��AXI UARTLite�豸
 * @param:	UartInstPtr		UART����
 * @param:	DeviceId		UART�豸ID
 * Sample:	AXI_UartLite_Init(&AXI_UART0, XPAR_AXI_UARTLITE_0_DEVICE_ID);
***********************************************************************************************************/
void AXI_UartLite_Init(XUartLite *UartInstPtr, uint16_t DeviceId)
{
	XUartLite_Initialize(UartInstPtr, DeviceId );
}

/***********************************************************************************************************
 * @brief:	��ʼ��AXI UARTLite�жϣ���ʼ��AXI UARTLite�ж�ǰ�����ȳ�ʼ��AXI UARTLite��
 * @param:	UartInstPtr		UART����
 * @param:	UartIntrId		UART�ж�ID
 * @param:	SendHandler		UART��������жϴ�����
 * @param:	RecvHandler		UART��������жϴ�����
 * @return:	��
 * Sample:	//��ʼ��AXI_UART�жϣ������������жϴ�����
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
 * @brief:	ʹ��AXI UARTLite�������ݣ���������0~65535֮��
 * @param:	UartInstPtr		UART����
 * @param:	data			����
 * @return:	��
 * Sample:	AXI_UARTLite_SendData(&UartPs,12345); //��������12345
***********************************************************************************************************/
void AXI_UARTLite_SendData(XUartLite *UartInstPtr,uint32_t data)
{
	uint8_t SendData[2];
	int data_H,data_L;
	data_H = data>>8;
	data_L = data&0x00ff;
	if(data_H == 0)//�����λΪ0
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
 * @brief:	ʹ��AXI UARTLite�����ַ���
 * @param:	UartInstPtr		UART����
 * @param:	str				�ַ�����ַ
 * @return:	��
 * Sample:	AXI_UARTLite_SendString(&UartPs,"1234567"); //������7���ֽ�
***********************************************************************************************************/
void AXI_UARTLite_SendString(XUartLite *UartInstPtr, char *str)
{
	XUartLite_Send(UartInstPtr, (uint8_t *)str, strlen(str));
}

/***********************************************************************************************************
 * @brief:	ʹ��AXI UARTLite��������
 * @param:	UartInstPtr		UART����
 * @param:	Buffer			�������ݴ洢��
 * @param:	Len				�������ݵĳ���
 * @return:	��
 * Sample:	AXI_UARTLite_RecvData(&UartPs, RecvBuffer,2);�����������ݣ��������ݴ���RecvBuffer����
***********************************************************************************************************/
void AXI_UARTLite_RecvData(XUartLite *UartInstPtr, uint8_t *Buffer,uint32_t Len)
{
	XUartLite_Recv(UartInstPtr, Buffer, Len);
}
