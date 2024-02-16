#ifndef __PAGEDESIGN_H__
#define __PAGEDESIGN_H__

#include "lcd.h"

//����ҳ�����屳��ɫ
#define PAGE_COLOR	LCD_BLACK

//����������ؽṹ�嶨��
typedef struct {
	uint16_t X1;	//������������x1
	uint16_t Y1;	//������������y1
	uint16_t Width;	//����Ŀ��
	uint16_t Height;//����ĸ߶�

} Box_XY;

//��ť������ؽṹ�嶨��
typedef struct {
	Box_XY Box;
	uint32_t TextColor;
	uint32_t BackColor;
	uint8_t TextSize;
	char Text[10][30];
} Button;

//�ı�������ؽṹ�嶨��
typedef struct {
	Box_XY Box;
	uint32_t TextColor;
	uint32_t BackColor;
	uint8_t TextSize;	//�����С
} Text;

//��������ؽṹ�嶨��
typedef struct {
	float X;	//ͼƬ��������x
	float Y;	//ͼƬ��������y

} Point_XY;

//���ν�����ؽṹ�嶨��
typedef struct {
	Box_XY Window;		//��ʾ���εĴ�������
	float Range;		//��ʾ���η�Χ��ԭʼֵ���ľ���ֵ
	uint8_t ADC_Bits;	//ADCλ��
	Box_XY Wave_Area;	//��������
	float Precision;	//����
} Wave_Page;



extern Wave_Page ADC_Wave;

void Draw_Box(Box_XY Box, uint32_t Color,int16_t Offset);
void Fill_Box(Box_XY Box,uint32_t BackColor,int16_t Offset);
void Get_Waveform_Window_Parameters(Wave_Page *Wave);
void Draw_Waveform_Windows(Wave_Page Wave, uint32_t Sample_Rate);
void Draw_Waveform(Wave_Page Wave,uint16_t *Pre_Wave_Data,uint16_t *Wave_Data);
void Clear_Waveform(Wave_Page Wave,uint16_t *Wave_Data);
void Draw_Grid_Background(Wave_Page Wave);
void Display_Time_Scale(Wave_Page Wave,uint32_t Sample_Rate);
void Display_Voltage_Scale(Wave_Page Wave);
void Draw_Normal_Button(Button Button);
void Draw_Normal_Text(Text Text, char *str);
void Draw_Button_Effect(Button Button);
void Draw_Mark_Line(uint8_t Direction,uint16_t Point, uint16_t Start,uint16_t End,uint32_t Color);

#endif
