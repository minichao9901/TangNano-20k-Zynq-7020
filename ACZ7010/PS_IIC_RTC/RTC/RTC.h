#ifndef APP_RTC_H_
#define APP_RTC_H_

#include "COMMON.h"

#define BASE_YEARS		2000				//基础年份设置为2000年

//用于存储时间的结构体，与RTC的2~8号寄存器一一对齐
typedef struct
{
	uint8_t second;	//秒
	uint8_t minute;	//分
	uint8_t hour; 	//时
	uint8_t day; 	//日
	uint8_t week; 	//星期
	uint8_t month; 	//月
	uint8_t year; 	//年
} RTC_TypeDef;

void RTC_Init();
void RTC_Get_Time();
void Set_Clock(uint8_t hour,uint8_t min,uint8_t sec);
void Set_Date(uint16_t year,uint8_t month,uint8_t day,uint8_t week);
void Time_Seg_Code(RTC_TypeDef *Data,uint8_t *Buffer);
void Date_Seg_Code(RTC_TypeDef *Data,uint8_t *Buffer);

#endif /* APP_RTC_H_ */
