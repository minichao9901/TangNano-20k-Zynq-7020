#include "RTC.h"

extern RTC_TypeDef Time;
extern uint8_t Seg_Code[12];

//RTC��ʼ��
void RTC_Init()
{
	uint8_t Reset_Data[2] = {0x00,0x00};
	PS_IIC_SeqWrite_Reg(&IIC0,0x51,REG8, 0x00, Reset_Data, 2);
}

//��ȡRTC��ʱ������
void RTC_Get_Time()
{
	//��ȡRTCоƬ�ڵ�ʱ������
	PS_IIC_SeqRead_Reg(&IIC0,0x51,REG8, 0x02, (uint8_t *)&Time, 7);

	//����RTCԭʼ���ݣ������Чλ
	Time.second &= 0x7F;	//Bit(0~6)��Ч
	Time.minute &= 0x7F;	//Bit(0~6)��Ч
	Time.hour   &= 0x3F;	//Bit(0~5)��Ч
	Time.day    &= 0x3F;	//Bit(0~5)��Ч
	Time.month  &= 0x1F;	//Bit(0~4)��Ч
	Time.year   &= 0xFF;	//Bit(0~7)��Ч
}
//ʱ��Ϊ24Сʱ�ƣ���00:00:00��23:59:59
void Set_Clock(uint8_t hour,uint8_t min,uint8_t sec)
{
	uint8_t Clock_Data[3];

	/* �˴�Ϊʮ������תBCD��ļ��� */
	Clock_Data[0] = ((sec/10)<<4) | (sec%10);
	Clock_Data[1] = ((min/10)<<4) | (min%10);
	Clock_Data[2] = ((hour/10)<<4) | (hour%10);

	PS_IIC_SeqWrite_Reg(&IIC0,0x51,REG8, 0x02, Clock_Data, 3);
}

//���ڴ�0��6��0��ʾ������
void Set_Date(uint16_t year,uint8_t month,uint8_t day,uint8_t week)
{
	uint8_t Date_Data[4];

	/* �˴�Ϊʮ������תBCD��ļ��� */
	Date_Data[0] = ((day/10)<<4) | (day%10);
	Date_Data[1] = week;
	Date_Data[2] = ((month/10)<<4) | (month%10);
	Date_Data[3] = (((year-BASE_YEARS)/10)<<4) | (year%10);

	PS_IIC_SeqWrite_Reg(&IIC0,0x51,REG8, 0x05, Date_Data, 4);
}

void Time_Seg_Code(RTC_TypeDef *Data,uint8_t *Buffer)
{
	Buffer[0]  = Seg_Code[Data->second & 0x0F];
	Buffer[2]  = Seg_Code[Data->second >> 4];
	Buffer[4]  = Seg_Code[10];//��ʾ -
	Buffer[6]  = Seg_Code[Data->minute & 0x0F];
	Buffer[8]  = Seg_Code[Data->minute >> 4];
	Buffer[10] = Seg_Code[10];//��ʾ -
	Buffer[12] = Seg_Code[Data->hour & 0x0F];
	Buffer[14] = Seg_Code[Data->hour >> 4];
}

void Date_Seg_Code(RTC_TypeDef *Data,uint8_t *Buffer)
{
	Buffer[0]  = Seg_Code[Data->day & 0x0F]&0x7F;
	Buffer[2]  = Seg_Code[Data->day >> 4];
	Buffer[4]  = Seg_Code[Data->month & 0x0F]&0x7F;
	Buffer[6]  = Seg_Code[Data->month >> 4];
	Buffer[8]  = Seg_Code[Data->year & 0x0F]&0x7F;
	Buffer[10] = Seg_Code[Data->year >> 4];
	Buffer[12] = Seg_Code[0];
	Buffer[14] = Seg_Code[2];
}
