/**
  *****************************************************************************
  * 				ʹ��PL�˵�AXI_UARTLite�����ж�ʵ�����ݷ��ͺͽ��մ���
  * 				֡ͷ'S' ֡β'E' �м���Ч���ݳ���Ϊ8�ֽڣ��ܳ�10�ֽ�
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

	//����ͨ���жϿ�����
	ScuGic_Init();

	//��ʼ��AXI_UART
	AXI_UartLite_Init(&AXI_UART0, XPAR_AXI_UARTLITE_0_DEVICE_ID);

	//��ʼ��AXI_UART�ж�
	AXI_UARTLite_Intr_Init(&AXI_UART0, XPAR_FABRIC_AXI_UARTLITE_0_INTERRUPT_INTR,
			AXI_UART0_Send_IRQ_Handler,AXI_UART0_Recv_IRQ_Handler);

	//UART����
	AXI_UARTLite_SendString(&AXI_UART0,"00 01 02 03 04 05 06 07 08 09 10 "
			"11 12 13 14 15 16 17 18 19 20 21 22 23 24 25\n");

	//�ȴ���һ�ַ������
	while(!All_Send_Flag);
	All_Send_Flag = 0;

	AXI_UARTLite_SendString(&AXI_UART0,"UART send done!!!\n");

	//�ȴ���һ�ַ������
	while(!All_Send_Flag);
	All_Send_Flag = 0;

	while(1) {
		//UART����10���ַ�
		AXI_UARTLite_RecvData(&AXI_UART0, Receive_Buffer, 10);

		//�ȴ�10�ֽڽ�����ɻ���ճ�ʱ
		Timeout = 10000;
		while((!All_Recv_Flag) && (Timeout)) {
			usleep(10);
			Timeout--;
		}

		if(All_Recv_Flag) {
			//���������ɱ�־
			All_Recv_Flag = 0;

			//������ܵ������ݣ�֡ͷ'S' ֡β'E'���м���Ч���ݳ���Ϊ8�ֽ�
			if((Receive_Buffer[0] == 'S') && (Receive_Buffer[9] == 'E')) {
				for(i=0;i<8;i++) {
					Data[i] = Receive_Buffer[i+1];
				}

				//�ط�8λ��Ч����
				XUartLite_Send(&AXI_UART0,Data,8);

				//����
				AXI_UARTLite_SendString(&AXI_UART0,"\n");
			}
		}
	}

	return 0;
}
