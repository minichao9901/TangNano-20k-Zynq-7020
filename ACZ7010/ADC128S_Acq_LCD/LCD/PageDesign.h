#ifndef __PAGEDESIGN_H__
#define __PAGEDESIGN_H__

#include "lcd.h"

//定义页面总体背景色
#define PAGE_COLOR	LCD_BLACK

//区域坐标相关结构体定义
typedef struct {
	uint16_t X1;	//区域左上坐标x1
	uint16_t Y1;	//区域左上坐标y1
	uint16_t Width;	//区域的宽度
	uint16_t Height;//区域的高度

} Box_XY;

//按钮坐标相关结构体定义
typedef struct {
	Box_XY Box;
	uint32_t TextColor;
	uint32_t BackColor;
	uint8_t TextSize;
	char Text[10][30];
} Button;

//文本坐标相关结构体定义
typedef struct {
	Box_XY Box;
	uint32_t TextColor;
	uint32_t BackColor;
	uint8_t TextSize;	//字体大小
} Text;

//点坐标相关结构体定义
typedef struct {
	float X;	//图片左上坐标x
	float Y;	//图片左上坐标y

} Point_XY;

//波形界面相关结构体定义
typedef struct {
	Box_XY Window;		//显示波形的窗口坐标
	float Range;		//显示波形范围（原始值）的绝对值
	uint8_t ADC_Bits;	//ADC位数
	Box_XY Wave_Area;	//波形区域
	float Precision;	//精度
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
