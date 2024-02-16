/**
  *****************************************************************************
  * ʹ��PS�˵�IIC����ʵ��RTC����,ʹ���������ʾʱ������ڣ����뿪���л���ʾģʽ
  *****************************************************************************
  *
  * @File   : main.c
  * @By     : Sun
  * @Version: V1.1
  * @Date   : 2022 / 06 / 09
  * @Shop	: https://xiaomeige.taobao.com/
  *
  *****************************************************************************
**/

#include "COMMON.h"

//���ڴ洢����ܵĶ�ѡ����ֵ������Ϊ0,1,2,3,4,5,6,7,8,9,-,.
uint8_t Seg_Code[12] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xBF,0x7F};

//�������ʾ��Ӧ������
uint8_t TubeBuffer[8][2] = {{0,0x01}, 	//����ܵ�0λ
							{0,0x02}, 	//����ܵ�1λ
							{0,0x04}, 	//����ܵ�2λ
							{0,0x08}, 	//����ܵ�3λ
							{0,0x10}, 	//����ܵ�4λ
							{0,0x20}, 	//����ܵ�5λ
							{0,0x40}, 	//����ܵ�6λ
							{0,0x80}};	//����ܵ�7λ

RTC_TypeDef Time;	//�洢ʱ������

int main(void)
{
	uint8_t Sel_Num = 0;	//�����λѡ
	uint8_t Mode;			//��ʾģʽ

	//��ʼ��ͨ���жϿ�����
	ScuGic_Init();

	//��ʼ��IIC0
	PS_IIC_Init(&IIC0,XPAR_XIICPS_0_DEVICE_ID,100000);

	//��ʼ��SPI0
	PS_SPI_Init(&SPI0,XPAR_XSPIPS_0_DEVICE_ID,
			XSPIPS_CLK_PRESCALE_64,XSPIPS_MASTER_OPTION);

	//��ʼ��PS��GPIO
	PS_GPIO_Init();

	//��ʼ��RTC
	RTC_Init();

	//����ʱ��Ϊ23��59��55��
	Set_Clock(23,59,55);

	//��������Ϊ2021��12��31�գ�������
	Set_Date(2021,12,31,5);

	//��GPIO54����Ϊ����
	PS_GPIO_SetMode(54, INPUT, 0);

	//��ʼ��˽�ж�ʱ���жϣ���ʱ���1ms
	ScuTimer_Int_Init(1000);

	while(1)
	{
		//RTC��ȡˢ�¼����˽�ж�ʱ���жϿ��ƣ��жϷ�������ISR.c��
		if(RTC_Refresh_Flag || (DISPLAY_MODE != Mode)) {
			//���RTCˢ�±�־
			RTC_Refresh_Flag = 0;
			//��ȡʱ��
			RTC_Get_Time();
			//ģʽ���
			Mode = DISPLAY_MODE;

			if (Mode) {
				//ʹ����ܿ���ʾ��ǰʱ�䡣��ʽΪ XX-XX-XX
				Time_Seg_Code(&Time,TubeBuffer[0]);
			} else {
				//ʹ����ܿ���ʾ��ǰ���ڡ���ʽΪ 20XX.XX.XX.
				Date_Seg_Code(&Time,TubeBuffer[0]);
			}
		}

		//ˢ�¼����˽�ж�ʱ���жϿ��ƣ��жϷ�������ISR.c��
		if(Tube_Refresh_Flag) {
			//��������ˢ�±�־
			Tube_Refresh_Flag = 0;

			//SPI���䣬���Ƶ�Sel_Numλ�������ʾ����
			PS_SPI_Transfer(&SPI0, 0, NULL, TubeBuffer[Sel_Num], 2);

			//λѡѭ��(0~7)
			if(Sel_Num >= 7)
				Sel_Num = 0;
			else
				Sel_Num++;
		}
	}

	return 0;
}
