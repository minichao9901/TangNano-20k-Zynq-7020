#include "RTC.h"

/**
  *****************************************************
  * @brief	初始化RTC设备
  * @usage	RTC_Init();	//初始化RTC设备
  * @tag	RTC在使用前要先初始化
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
  * @brief	设置当前时间
  * @param	hour	时
  * @param	min		分
  * @param	sec		秒
  * @usage	Set_Clock(11,22,33);	//设置时间为11:22:33
  * @tag	时间为24小时制，从00:00:00到23:59:59
  *****************************************************
**/
void Set_Clock(uint8_t hour,uint8_t min,uint8_t sec)
{
	uint8_t Clock_Data[3];

	/* 此处为十进制数转BCD码的计算 */
	Clock_Data[0] = ((sec/10)<<4) + (sec%10);
	Clock_Data[1] = ((min/10)<<4) + (min%10);
	Clock_Data[2] = ((hour/10)<<4) + (hour%10);

	PS_IIC_SeqWrite_Reg(&Iic,0x51,REG8, 0x02, Clock_Data, 3);
}

/**
  *****************************************************
  * @brief	设置当前日期
  * @param	year	年份，若为20xx年，此处填入xx
  * @param	month	月
  * @param	day		日
  * @param	week	星期
  * @usage	Set_Date(21,12,23,0);	//设置时间为2021-12-23，周日
  * @tag	星期从0到6，0表示星期日，1表示星期一，6表示星期六
  *****************************************************
**/
void Set_Date(uint8_t year,uint8_t month,uint8_t day,uint8_t week)
{
	uint8_t Date_Data[4];

	/* 此处为十进制数转BCD码的计算 */
	Date_Data[0] = ((day/10)<<4) + (day%10);
	Date_Data[1] = week;
	Date_Data[2] = ((month/10)<<4) + (month%10);
	Date_Data[3] = ((year/10)<<4) + (year%10);

	PS_IIC_SeqWrite_Reg(&Iic,0x51,REG8, 0x05, Date_Data, 4);
}

void Get_Time(Time *Time)
{
	uint8_t Data[7];//RTC原始数据
	PS_IIC_SeqRead_Reg(&Iic,0x51,REG8, 0x02, Data, 7);
	/* 此处为BCD码转十进制数的计算 */
	Time->second = ((Data[0]&0x7F)>>4)*10 + (Data[0] & 0x0F);//秒
	Time->minute = ((Data[1]&0x7F)>>4)*10 + (Data[1]&0x0F);	//分
	Time->hour = ((Data[2]&0x3F)>>4)*10 + (Data[2]&0x0F);	//时
	Time->day = ((Data[3]&0x3F)>>4)*10 + (Data[3]&0x0F);	//日
	Time->month = ((Data[5]&0x1F)>>4)*10 + (Data[5]&0x0F);	//月
	Time->year = ((Data[6]&0x7F)>>4)*10 + (Data[6]&0x0F) + BASE_YEARS;	//年
}
