#ifndef _RTC_H_
#define _RTC_H_

#include "PS_IIC.h"

#define BASE_YEARS		2000				//�����������Ϊ2000��

/*
 * ���ڴ洢ʱ��Ľṹ�壬�����롢�֡�ʱ���ա����ڡ��¡���
 * ʮλ�͸�λ�ֱ�洢������[0]������[1]��
 */
typedef struct {
	int second;	//��
	int minute;	//��
	int hour; 	//ʱ
	int day; 	//��
	int week; 	//����
	int month; 	//��
	int year; 	//��
} Time;

void RTC_Init();
void Set_Clock(uint8_t hour,uint8_t min,uint8_t sec);
void Set_Date(uint8_t year,uint8_t month,uint8_t day,uint8_t week);
void Get_Time(Time *Time);

#endif /* _RTC_H_ */
