#ifndef LCD_HOMEPAGE_H_
#define LCD_HOMEPAGE_H_

#include "Touch.h"
#include "lcd.h"

#define Up_Line1		4		//白框上边界Y坐标
#define Down_Line1		54		//白框下边界Y坐标
#define Left_Line1		2		//白框左边界X坐标
#define Right_Line1		42		//白框右边界X坐标

#define Up_Line2		4		//白框上边界Y坐标
#define Down_Line2		54		//白框下边界Y坐标
#define Left_Line2		45		//白框左边界X坐标
#define Right_Line2		96		//白框右边界X坐标

#define Up_Line3		6		//白框上边界Y坐标
#define Down_Line3		12		//白框下边界Y坐标
#define Left_Line3		47		//白框左边界X坐标
#define Right_Line3		90		//白框右边界X坐标

#define FREQ1_X			17
#define FREQ1_Y			30
#define FREQ2_X			30
#define FREQ2_Y			30

//按钮坐标相关结构体定义
typedef struct {
	uint16_t X1;	//按键坐标x1
	uint16_t X2;	//按键坐标x2
	uint16_t Y1;	//按键坐标y1
	uint16_t Y2;	//按键坐标y2

} Button_XY;

//文本坐标相关结构体定义
typedef struct {
	float X;	//文本坐标x
	float Y;	//文本坐标y

} Text_XY;

void Draw_Button(Button_XY Button_XY);
void Draw_Small_Button(Button_XY Button_XY);
void Fill_Button(Button_XY Button_XY,uint32_t Color);
void WR_Text(Text_XY Text_XY, uint8_t *TEXT, uint8_t SIZE, uint8_t MODE);
void WR_Num(Text_XY Text_XY, uint32_t NUM, uint8_t LEN);
uint8_t Judge_Button(Button_XY Button_XY);
void Press_Button(Button_XY Button_XY);
void Release_Button(Button_XY Button_XY);

#endif /* LCD_HOMEPAGE_H_ */
