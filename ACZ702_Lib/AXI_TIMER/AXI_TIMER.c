/**
  *****************************************************************************
  * 					AXI TIMER IP�˵Ķ�ʱ���ж����Ӧ�ÿ�
  *****************************************************************************
  *
  * @File   : AXI_Timer.c
  * @By     : Sun
  * @Version: V1.0
  * @Date   : 2022 / 05 / 19
  * @Shop	: https://xiaomeige.taobao.com/
  *
  *****************************************************************************
**/

#include "AXI_TIMER.h"

XTmrCtr AXI_Timer0;		//��ʱ��ʵ��


/**
  *****************************************************
  * @brief	��ʼ��AXI_Timer�豸
  * @param	TmrCtrInstPtr	Timer����ָ��
  * @param	DevId			Timer�豸ID
  * @usage	//��ʱ����ʼ��
  * 		AXI_Timer_Init(&Timer0,TMRCTR_DEVICE_ID);
  *****************************************************
**/
void AXI_Timer_Init(XTmrCtr *TmrCtrInstPtr,uint16_t DevId)
{
	XTmrCtr_Initialize(TmrCtrInstPtr, DevId);
}


/**
  *****************************************************
  * @brief	��ʼ��AXI_Timer�ж�
  * @param	TmrCtrInstPtr	Timer����ָ��
  * @param	IntrId			�ж�ID
  * @param	Timer_Handler	Timer���жϷ�����
  * @usage	//��ʱ���жϳ�ʼ��,�жϷ�����ΪAXI_Timer0_Handler
  * 		AXI_Timer_Int_Init(&Timer0,TMRCTR_INTERRUPT_ID,AXI_Timer0_Handler);
  *****************************************************
**/
void AXI_Timer_Int_Init(XTmrCtr *TmrCtrInstPtr,uint16_t IntrId,XTmrCtr_Handler Timer_Handler)
{
	Set_ScuGic_Link(IntrId, 0xA8, HIGH_Level_Sensitive, XTmrCtr_InterruptHandler, TmrCtrInstPtr);
	XTmrCtr_SetHandler(TmrCtrInstPtr, Timer_Handler,TmrCtrInstPtr);
}

/**
  *****************************************************
  * @brief	����AXI_Timer����
  * @param	TmrCtrInstPtr	Timer����ָ��
  * @param	TmrCtrNum		AXI_Timer��������0Ϊ������0��1Ϊ������1
  * @param	Mode			Timerģʽ����
  * ģʽ�����¼��֣�	XTC_CASCADE_MODE_OPTION		���ü���ģʽ����������TCSRO
  *					XTC_ENABLE_ALL_OPTION		һ�����������ж�ʱ������
  *                 XTC_DOWN_COUNT_OPTION		����Ϊ���¼�����Ĭ��Ϊ���ϼ�����
  *                 XTC_CAPTURE_MODE_OPTION		����Ϊ���벶��ģʽ��Ĭ��Ϊ�Ƚ�ģʽ��
  *                 XTC_INT_MODE_OPTION			����ʱ���жϣ����¼�����0�����ϼ������ʱ����
  *                 XTC_AUTO_RELOAD_OPTION		�����Զ���װ��
  *                 XTC_EXT_COMPARE_OPTION		�����ⲿ�Ƚ�����ź�
  * @param	Reload_Val		װ��ֵ��ÿ��CLK�������ϻ��¼���1�������CLK��AXI_TIMER IP���ⲿ����
  * @usage	//��ʱ��������0����Ϊ�ж�ģʽ���Զ���װ�������¼���
  * 		AXI_Timer_SetOption(&Timer0,TIMER_CNTR_0, XTC_INT_MODE_OPTION |
  *				XTC_AUTO_RELOAD_OPTION | XTC_DOWN_COUNT_OPTION,RELOAD_VALUE);
  *****************************************************
**/
void AXI_Timer_SetOption(XTmrCtr *TmrCtrInstPtr,uint8_t TmrCtrNum,uint32_t Mode,uint32_t Reload_Val)
{
	XTmrCtr_SetOptions(TmrCtrInstPtr, TmrCtrNum, Mode);

	//������װ��ֵ�����ֵ�ڼ���������ʱ������
	XTmrCtr_SetResetValue(TmrCtrInstPtr, TmrCtrNum, Reload_Val);

	//����������
	XTmrCtr_Start(TmrCtrInstPtr, TmrCtrNum);
}
