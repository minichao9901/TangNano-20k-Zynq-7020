#include "RTC.h"

extern RTC_TypeDef Time;
extern uint8_t Seg_Code[12];

//RTC初始化
void RTC_Init()
{
	uint8_t Reset_Data[2] = {0x00,0x00};
	PS_IIC_SeqWrite_Reg(&IIC0,0x51,REG8, 0x00, Reset_Data, 2);
}

//获取RTC内时间数据
void RTC_Get_Time()
{
	//读取RTC芯片内的时间数据
	PS_IIC_SeqRead_Reg(&IIC0,0x51,REG8, 0x02, (uint8_t *)&Time, 7);

	//处理RTC原始数据，清除无效位
	Time.second &= 0x7F;	//Bit(0~6)有效
	Time.minute &= 0x7F;	//Bit(0~6)有效
	Time.hour   &= 0x3F;	//Bit(0~5)有效
	Time.day    &= 0x3F;	//Bit(0~5)有效
	Time.month  &= 0x1F;	//Bit(0~4)有效
	Time.year   &= 0xFF;	//Bit(0~7)有效
}
//时间为24小时制，从00:00:00到23:59:59
void Set_Clock(uint8_t hour,uint8_t min,uint8_t sec)
{
	uint8_t Clock_Data[3];

	/* 此处为十进制数转BCD码的计算 */
	Clock_Data[0] = ((sec/10)<<4) | (sec%10);
	Clock_Data[1] = ((min/10)<<4) | (min%10);
	Clock_Data[2] = ((hour/10)<<4) | (hour%10);

	PS_IIC_SeqWrite_Reg(&IIC0,0x51,REG8, 0x02, Clock_Data, 3);
}

//星期从0到6，0表示星期日
void Set_Date(uint16_t year,uint8_t month,uint8_t day,uint8_t week)
{
	uint8_t Date_Data[4];

	/* 此处为十进制数转BCD码的计算 */
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
	Buffer[4]  = Seg_Code[10];//显示 -
	Buffer[6]  = Seg_Code[Data->minute & 0x0F];
	Buffer[8]  = Seg_Code[Data->minute >> 4];
	Buffer[10] = Seg_Code[10];//显示 -
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
