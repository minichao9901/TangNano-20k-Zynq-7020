/**
  *****************************************************************************
  * 				�洢Ҫ��ӡ����־��Ϣ������CPU����ʱ��ӡ����
  *****************************************************************************
  *
  * @File   : LOG.c
  * @By     : Sun
  * @Version: V1.0
  * @Date   : 2021 / 12 / 23
  * @Shop	: https://xiaomeige.taobao.com/
  * @Tag	: LOG��ӡ����ռ����PS UART1��TTC0�ļ�����2
  *****************************************************************************
**/
#include "LOG.h"


char log_text[4096] = {};
char *p_log;
uint32_t log_cnt = 0;
XUartPs Debug_UART;

void Debug_LOG_IRQ_Handler(void *CallBackRef)
{
	/* �������û���������� */

	LOGP();//LOG��ӡ

	/* ������������������� */

	//����жϱ�־λ����ȡ���Զ������
	XTtcPs_GetInterruptStatus(&TTC0_2_Timer);
}

//PS�����жϷ�����
void Debug_UART_IRQ_Handler(void *CallBackRef)
{
	uint32_t IsrStatus;

	IsrStatus = XUartPs_ReadReg(Debug_UART.Config.BaseAddress,XUARTPS_ISR_OFFSET);

	//TX FIFO�� ���ǿա� ��Ϊ ���ա� ʱ
	if(IsrStatus & XUARTPS_IXR_TXEMPTY) {
		if (Debug_UART.SendBuffer.RemainingBytes == 0) {
			//������ϣ�����TX FIFO�ж�
			XUartPs_WriteReg(Debug_UART.Config.BaseAddress,XUARTPS_IDR_OFFSET,XUARTPS_IXR_TXEMPTY);

		} else {
			Debug_UART_SendBuffer(&Debug_UART);
		}
	}

	/* ����ж�״̬*/
	XUartPs_WriteReg(Debug_UART.Config.BaseAddress, XUARTPS_ISR_OFFSET,IsrStatus);
}




/**
  *****************************************************
  * @brief	Debug��ʼ����������ʼ�����ڡ���ʼ����־��ʱ���ж�
  * @usage	Log_Debug_Init();	//��ʼ��Debug����
  * @tag	Debug��ӡ���ܻ���PS�˵Ĵ���1����TTC0
  * 		ʹ��֮ǰ��Ҫ����PS��UART1��TTC0
  *****************************************************
**/
void Log_Debug_Init()
{
	if(LOG_DEBUG) {

		//���Դ��ڳ�ʼ��
		Debug_UART_Init();

		//����TTC0_0Ϊ�δ�ʱ������ʱ��������ÿ10ms���һ����־
		TTC_Tick_Init(&TTC0_2_Timer, XPAR_XTTCPS_2_DEVICE_ID,
				XPS_TTC0_2_INT_ID, 10000, Debug_LOG_IRQ_Handler);
		TTC_Tick_Start(&TTC0_2_Timer);//������ʱ��
	}

}

void LOGP()
{
	if(log_cnt && LOG_DEBUG)
	{
		//ֹͣFifo�жϷ�ֹ����
		XUartPs_WriteReg(Debug_UART.Config.BaseAddress, XUARTPS_IDR_OFFSET,
				XUARTPS_IXR_TXEMPTY);

		//���贫�����
		Debug_UART.SendBuffer.RequestedBytes = log_cnt;
		Debug_UART.SendBuffer.RemainingBytes = log_cnt;
		Debug_UART.SendBuffer.NextBytePtr = (uint8_t*)log_text;

		Debug_UART_SendBuffer(&Debug_UART);

		log_cnt = 0;
	}
}

void Debug_UART_Init()
{
	XUartPs_Config *Config;
	//�����ñ��в������ã�Ȼ��Դ��ڽ��г�ʼ��
	Config = XUartPs_LookupConfig(DEBUG_UART_DEV_ID);
	XUartPs_CfgInitialize(&Debug_UART, Config, Config->BaseAddress);
	XUartPs_SetBaudRate(&Debug_UART, DEBUG_UART_BAUDRATE);
    //���ӵ��жϿ�����
    Set_ScuGic_Link(DEBUG_UART_INT_ID, 0xA0, HIGH_Level_Sensitive,
    		Debug_UART_IRQ_Handler, (void *)&Debug_UART);

}

void Debug_UART_SendBuffer(XUartPs *InstPtr)
{
	uint32_t Send_Cnt = 0;

	while ((!XUartPs_IsTransmitFull(InstPtr->Config.BaseAddress)) &&
		   (InstPtr->SendBuffer.RemainingBytes > Send_Cnt)) {

		/* ����������FIFO */
		XUartPs_WriteReg(InstPtr->Config.BaseAddress,XUARTPS_FIFO_OFFSET,
				((uint32_t)InstPtr->SendBuffer.NextBytePtr[Send_Cnt]));

		Send_Cnt++;
	}

	InstPtr->SendBuffer.NextBytePtr += Send_Cnt;
	InstPtr->SendBuffer.RemainingBytes -= Send_Cnt;

	//��TX FIFO���ж�
	XUartPs_WriteReg(InstPtr->Config.BaseAddress,
			XUARTPS_IER_OFFSET,(uint32_t)XUARTPS_IXR_TXEMPTY);
}
