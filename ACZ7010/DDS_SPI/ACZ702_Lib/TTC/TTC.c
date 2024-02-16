/**
  *****************************************************************************
  * 						TTC�δ�ʱ��Ӧ��
  *****************************************************************************
  *
  * @File   : TTC.c
  * @By     : Sun
  * @Version: V0.5
  * @Date   : 2022 / 05 / 30
  * @Shop	: https://xiaomeige.taobao.com/
  *
  *****************************************************************************
**/

#include "TTC.h"

XTtcPs TTC0_0_Timer,TTC0_1_Timer,TTC0_2_Timer;

/**
  *****************************************************
  * @brief	TTC��ʼ���������δ�ʱ�ж�
  * @param	TTC_Inst	TTC����ָ��
  * @param	DeviceID	TTC�豸ID
  * @param	IntrId		�ж�ID
  * @param	Time_Us		��ʱ��ʱ�䣬��λus
  * @param	Handler		�жϷ�����
  * @usage	//��ʼ��TTC0_0��Ϊ�δ�ʱ����ÿ10us����һ���ж�
  * 		TTC_Tick_Init(&TTC0_0_Timer, XPAR_XTTCPS_0_DEVICE_ID, XPS_TTC0_0_INT_ID,
  * 				10, TTC0_0_IRQ_Handler);
  * @tag	��ʼ������Ҫʹ��TTC_Tick_Start��������
  *****************************************************
**/
void TTC_Tick_Init(XTtcPs *TTC_Inst,int DeviceID,uint16_t IntrId,
		uint32_t Time_Us, Xil_InterruptHandler Handler)
{
	XInterval Interval;
	uint8_t Prescaler = 0;
	uint32_t Load_Val;
	uint32_t Tmp_Presc;

	XTtcPs_Config *Config;
	Config = XTtcPs_LookupConfig(DeviceID);
	XTtcPs_CfgInitialize(TTC_Inst, Config, Config->BaseAddress);

	XTtcPs_SetOptions(TTC_Inst, XTTCPS_OPTION_INTERVAL_MODE | XTTCPS_OPTION_WAVE_DISABLE);

	//�������Ƶ�ʵļ��ֵ�ͷ�Ƶֵ

	Load_Val = (((float)TTC_Inst->Config.InputClockHz / 1000000) * Time_Us - 1);

	//�Ƿ���Ҫ��Ƶ
	if(Load_Val > XTTCPS_MAX_INTERVAL_COUNT) {
		//���������Ƶϵ��
		Tmp_Presc = Load_Val / XTTCPS_MAX_INTERVAL_COUNT;
		if(Load_Val % XTTCPS_MAX_INTERVAL_COUNT) {
			Tmp_Presc++;
		}

		//����ʵ�ʷ�Ƶϵ��
		while((2 << Prescaler) < Tmp_Presc) {
			Prescaler++;
		}
		Tmp_Presc = (2 << Prescaler);

		//����ʵ�ʷ�Ƶϵ������ʵ�ʼ��
		Interval = Load_Val / Tmp_Presc;

	} else {
		//����Ƶ
		Prescaler = XTTCPS_CLK_CNTRL_PS_DISABLE;
		Interval = Load_Val;
	}

	//���ü��ֵ����Ƶֵ
	XTtcPs_SetInterval(TTC_Inst, Interval);
	XTtcPs_SetPrescaler(TTC_Inst, Prescaler);

	//�����жϷ��������ж����ȼ�����Ϊ0xB0
	Set_ScuGic_Link(IntrId, 0xB0, HIGH_Level_Sensitive,
			(Xil_ExceptionHandler)Handler, (void *)TTC_Inst);

	//ʹ�ܼ���жϣ��������ʱ����
	XTtcPs_EnableInterrupts(TTC_Inst, XTTCPS_IXR_INTERVAL_MASK);
}

/**
  *****************************************************
  * @brief	TTC��ʱ����ʼ����
  * @param	TTC_Inst	TTC����ָ��
  * @usage	//TTC0_0��ʼ����
  * 		TTC_Tick_Start(&TTC0_0_Timer);
  *****************************************************
**/
void TTC_Tick_Start(XTtcPs *TTC_Inst)
{
	//������ʱ/����
	XTtcPs_Start(TTC_Inst);
}

/**
  *****************************************************
  * @brief	TTC��ʱ��ֹͣ����
  * @param	TTC_Inst	TTC����ָ��
  * @usage	//TTC0_0ֹͣ����
  * 		TTC_Tick_Stop(&TTC0_0_Timer);
  *****************************************************
**/
void TTC_Tick_Stop(XTtcPs *TTC_Inst)
{
	//�رն�ʱ/����
	XTtcPs_Stop(TTC_Inst);

	//��λ����ֵ
	XTtcPs_ResetCounterValue(TTC_Inst);
}
