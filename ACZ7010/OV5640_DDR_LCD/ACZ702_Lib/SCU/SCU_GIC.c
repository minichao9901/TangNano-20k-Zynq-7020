/**
  *****************************************************************************
  * 						ͨ���жϿ�������
  *****************************************************************************
  *
  * @File   : SCU_GIC.c
  * @By     : Sun
  * @Version: V0.5
  * @Date   : 2022 / 05 / 30
  * @Shop	: https://xiaomeige.taobao.com/
  *
  *****************************************************************************
**/
#include "SCU_GIC.h"

XScuGic ScuGic;	//ͨ���жϿ���������

/**
  *****************************************************
  * @brief	��ʼ��ͨ���жϿ�����
  * @usage	ScuGic_Init();	//��ʼ��ͨ���жϿ�����
  * @tag	��ʼ��ͨ���жϿ������󣬲ſ������ж�
  *****************************************************
**/
void ScuGic_Init()
{
	XScuGic_Config *IntcConfig;
	IntcConfig = XScuGic_LookupConfig(XPAR_SCUGIC_SINGLE_DEVICE_ID);
	XScuGic_CfgInitialize(&ScuGic, IntcConfig,IntcConfig->CpuBaseAddress);

	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			(Xil_ExceptionHandler)XScuGic_InterruptHandler,&ScuGic);
	Xil_ExceptionEnable();
}

/**
  *****************************************************
  * @brief	���ж����ӵ�ͨ���жϿ�����
  * @param	IntrId		�ж�����ID
  * @param	Priority	�ж����ȼ�����5λ��Ч��Խ�����ȼ�Խ��
  * @param	Trigger		�жϴ���ģʽ��
  * 							�ߵ�ƽ���� -> HIGH_Level_Sensitive
  * 							�����ش��� -> Rising_Edge_Sensitive
  * @param	Handler		�жϷ�����
  * @param	CallBackRef	�ص����ã�ͨ�����������������ʵ��ָ��
  * @usage	//����XPAR_SCUTIMER_INTR���жϿ��������жϷ�����ΪScuTimer_IRQ_Handler
  * 		Set_ScuGic_Link(XPAR_SCUTIMER_INTR, 0xA8, HIGH_Level_Sensitive,
  * 			ScuTimer_IRQ_Handler, (void *)&ScuTimer);
  *****************************************************
**/
void Set_ScuGic_Link(uint16_t IntrId, uint8_t Priority, uint8_t Trigger,
		Xil_InterruptHandler Handler, void *CallBackRef)
{
	XScuGic_SetPriorityTriggerType(&ScuGic, IntrId, Priority, Trigger);
	XScuGic_Connect(&ScuGic, IntrId,
			(Xil_ExceptionHandler)Handler,CallBackRef);
	XScuGic_Enable(&ScuGic, IntrId);
}
