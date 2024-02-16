#ifndef _RTC_H_
#define _RTC_H_

#include "PS_IIC.h"

#define BASE_YEARS		2000				//基础年份设置为2000年

/*
 * 用于存储时间的结构体，包含秒、分、时、日、星期、月、年
 * 十位和个位分别存储在数组[0]和数组[1]中
 */
typedef struct {
	int second;	//秒
	int minute;	//分
	int hour; 	//时
	int day; 	//日
	int week; 	//星期
	int month; 	//月
	int year; 	//年
} Time;

void RTC_Init();
void Set_Clock(uint8_t hour,uint8_t min,uint8_t sec);
void Set_Date(uint8_t year,uint8_t month,uint8_t day,uint8_t week);
void Get_Time(Time *Time);

#endif /* _RTC_H_ */
