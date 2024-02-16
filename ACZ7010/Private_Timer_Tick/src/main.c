#include "COMMON.h"


int main(void)
{
	uint32_t Tick_Cnt = 0;

	//��ʼ��ͨ���жϿ�����
	ScuGic_Init();

	//��ʼ��˽�ж�ʱ����ÿ1s����һ��
	ScuTimer_Int_Init(1000000);

	while(1) {
		//Tick��־��˽�ж�ʱ���жϿ��ƣ��жϷ�������ISR.c��
		if(Tick_Flag) {
			Tick_Flag = 0;

			//��ӡTick_Cnt����ֵ
			printf("Tick Cnt: %lu\n", Tick_Cnt);

			//Tick_Cnt��Χ0~59
			if(Tick_Cnt >= 59)
				Tick_Cnt = 0;
			else
				Tick_Cnt++;
		}
	}

	return 0;
}
