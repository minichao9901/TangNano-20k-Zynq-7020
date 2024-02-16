#ifndef APP_RTC_H_
#define APP_RTC_H_

#include "COMMON.h"

#define BASE_YEARS		2000				//�����������Ϊ2000��

//���ڴ洢ʱ��Ľṹ�壬��RTC��2~8�żĴ���һһ����
typedef struct
{
	uint8_t second;	//��
	uint8_t minute;	//��
	uint8_t hour; 	//ʱ
	uint8_t day; 	//��
	uint8_t week; 	//����
	uint8_t month; 	//��
	uint8_t year; 	//��
} RTC_TypeDef;

void RTC_Init();
void RTC_Get_Time();
void Set_Clock(uint8_t hour,uint8_t min,uint8_t sec);
void Set_Date(uint16_t year,uint8_t month,uint8_t day,uint8_t week);
void Time_Seg_Code(RTC_TypeDef *Data,uint8_t *Buffer);
void Date_Seg_Code(RTC_TypeDef *Data,uint8_t *Buffer);

#endif /* APP_RTC_H_ */
