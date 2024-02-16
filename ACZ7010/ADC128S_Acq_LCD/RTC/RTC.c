#include "RTC.h"

/**
  *****************************************************
  * @brief	��ʼ��RTC�豸
  * @usage	RTC_Init();	//��ʼ��RTC�豸
  * @tag	RTC��ʹ��ǰҪ�ȳ�ʼ��
  *****************************************************
**/
void RTC_Init()
{
	uint8_t Reset_Data[2] = {0x00,0x00};
	//PS_IIC_Init(&Iic,IIC_DEVICE_ID);
	PS_IIC_SeqWrite_Reg(&Iic,0x51,REG8, 0x00, Reset_Data, 2);
}


/**
  *****************************************************
  * @brief	���õ�ǰʱ��
  * @param	hour	ʱ
  * @param	min		��
  * @param	sec		��
  * @usage	Set_Clock(11,22,33);	//����ʱ��Ϊ11:22:33
  * @tag	ʱ��Ϊ24Сʱ�ƣ���00:00:00��23:59:59
  *****************************************************
**/
void Set_Clock(uint8_t hour,uint8_t min,uint8_t sec)
{
	uint8_t Clock_Data[3];

	/* �˴�Ϊʮ������תBCD��ļ��� */
	Clock_Data[0] = ((sec/10)<<4) + (sec%10);
	Clock_Data[1] = ((min/10)<<4) + (min%10);
	Clock_Data[2] = ((hour/10)<<4) + (hour%10);

	PS_IIC_SeqWrite_Reg(&Iic,0x51,REG8, 0x02, Clock_Data, 3);
}

/**
  *****************************************************
  * @brief	���õ�ǰ����
  * @param	year	��ݣ���Ϊ20xx�꣬�˴�����xx
  * @param	month	��
  * @param	day		��
  * @param	week	����
  * @usage	Set_Date(21,12,23,0);	//����ʱ��Ϊ2021-12-23������
  * @tag	���ڴ�0��6��0��ʾ�����գ�1��ʾ����һ��6��ʾ������
  *****************************************************
**/
void Set_Date(uint8_t year,uint8_t month,uint8_t day,uint8_t week)
{
	uint8_t Date_Data[4];

	/* �˴�Ϊʮ������תBCD��ļ��� */
	Date_Data[0] = ((day/10)<<4) + (day%10);
	Date_Data[1] = week;
	Date_Data[2] = ((month/10)<<4) + (month%10);
	Date_Data[3] = ((year/10)<<4) + (year%10);

	PS_IIC_SeqWrite_Reg(&Iic,0x51,REG8, 0x05, Date_Data, 4);
}

void Get_Time(Time *Time)
{
	uint8_t Data[7];//RTCԭʼ����
	PS_IIC_SeqRead_Reg(&Iic,0x51,REG8, 0x02, Data, 7);
	/* �˴�ΪBCD��תʮ�������ļ��� */
	Time->second = ((Data[0]&0x7F)>>4)*10 + (Data[0] & 0x0F);//��
	Time->minute = ((Data[1]&0x7F)>>4)*10 + (Data[1]&0x0F);	//��
	Time->hour = ((Data[2]&0x3F)>>4)*10 + (Data[2]&0x0F);	//ʱ
	Time->day = ((Data[3]&0x3F)>>4)*10 + (Data[3]&0x0F);	//��
	Time->month = ((Data[5]&0x1F)>>4)*10 + (Data[5]&0x0F);	//��
	Time->year = ((Data[6]&0x7F)>>4)*10 + (Data[6]&0x0F) + BASE_YEARS;	//��
}
