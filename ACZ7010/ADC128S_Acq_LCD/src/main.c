/**
  *****************************************************************************
  * 			����ADC128S102����һ������ʾ���������������Ϊ1MHz
  *****************************************************************************
  *
  * @File   : main.c
  * @By     : С÷���Ŷ�
  * @Version: V1.2
  * @Date   : 2021 / 12 / 17
  * @Shop	: https://xiaomeige.taobao.com/
  * @Forum  : http://www.corecourse.cn/
  *****************************************************************************
**/

#include "Handle_Process.h"

//������
int main(void)
{
	Init_Homepage();//��ʼ����ҳ�Ϳ�����

	//ѭ�������¼�
	while(1) {
		//��������˵��δ����������ɹ���STOP��ȡ��������ָ�ԭ״
		if(Single_TriggerFlag && (ADC_ROUND_DONE || Cancel_Trigger)) {
			Single_TriggerFlag = 0;	//���־
			Handle_Single_Trigger();//�����δ����¼�
		}
		//ÿ�ִ��������ˢ��Flash��������һ�ִ���
		if(ADC_ROUND_DONE && Wave_Run && (!Single_TriggerFlag)) {
			Handle_Round_Done();
		}
		//��ʱˢ�²��δ��ڣ�10msһ��
		if(Flag_DrawWave && Refresh_Flag) {
			Flag_DrawWave = 0;
			Refresh_WaveWindow();	//ˢ�²��δ���
		}
		//��ʱ��ⴥ����30msһ��
		if(Flag_TouchScan) {
			Flag_TouchScan = 0;
			Touch_Scan();					//����ɨ��
		}
		//��ʱˢ���񱳾���20msһ��
		if(Flag_DrawGrid && Refresh_Flag) {
			Flag_DrawGrid = 0;
			Draw_Grid_Background(ADC_Wave);	//�������񱳾�
		}
		//��ʱˢ����ֵ��500msһ��
		if(Flag_Refresh_Val && Wave_Run) {
			Flag_Refresh_Val = 0;
			Refresh_Measure_Val();			//ˢ�²���ֵ
		}
		//��ǰʱ��ˢ�£�500msһ��
		if(Flag_Refresh_Time) {
			Flag_Refresh_Time = 0;
			Refresh_Time();			//ˢ��RTCʱ��
		}
	}
	return 0;
}

