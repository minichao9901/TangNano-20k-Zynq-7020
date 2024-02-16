#ifndef __HANDLE_PROCESS_H__
#define __HANDLE_PROCESS_H__

#include "Touch.h"
#include "ADC128S102.h"
#include "sleep.h"
#include "lcd.h"
#include "Touch.h"
#include "string.h"
#include "Timer.h"
#include "PageDesign.h"
#include "RTC.h"
#include "BMP_WR.h"

//������־
extern uint8_t Refresh_Flag;		//����ˢ�±�־
extern uint8_t Wave_Run;			//���βɼ���־
extern uint8_t Single_TriggerFlag;	//������־
extern uint8_t Cancel_Trigger;		//ȡ������

//�����������̵ı�־
extern uint8_t Flag_DrawWave;		//���Ʋ��Σ�10msһ��
extern uint8_t Flag_DrawGrid;		//��������20msһ��
extern uint8_t Flag_TouchScan;		//����ɨ�裬30msһ��
extern uint8_t Flag_Refresh_Val;	//ˢ�µ�ѹ��Ƶ����ֵ��500msһ��
extern uint8_t Flag_Refresh_Time;	//ˢ��ʱ�䣬500msһ��


//������������
void Read_Wave_Data();		//��DDR��ȡ��������
void Init_Homepage();		//��ʼ��������
void Refresh_Measure_Val();	//ˢ�²���ֵ
void Refresh_Time();		//ˢ��ʱ��
void Refresh_WaveWindow();	//ˢ�²��δ���
void Touch_Scan();			//����ɨ��
void STOP_Disable_Press();		//��STOP״̬�¶���ĳЩ����
void STOP_Ensable_Press();		//�����STOP״̬�±�����Ĳ���
void Trigger_Disable_Press();	//�ڵ��δ���״̬�¶���ĳЩ����
void Trigger_Enable_Press();	//����ڵ��δ���״̬�±�����Ĳ���
void Perform_STOP();		//ִ��STOP����
uint8_t Judge_TpXY(Touch_Data Touch_LCD, Box_XY Box);//�жϴ�������
uint8_t Judge_Two_TpXY(Touch_Data Touch_LCD, Box_XY Box);
void Handle_Single_Trigger();
void Handle_Round_Done();

#endif /* HANDLE_PROCESS_H */
