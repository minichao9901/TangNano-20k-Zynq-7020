#include "HomePage.h"

extern uint16_t last_tp_x;        //上一次触摸坐标x
extern uint16_t last_tp_y;		//上一次触摸坐标y


//定义画按键方框的函数
void Draw_Button(Button_XY Button_XY)
{
	LCD_DrawRectangle(Button_XY.X1*8,Button_XY.Y1*8,Button_XY.X2*8,Button_XY.Y2*8,POINT_COLOR);
	LCD_Refresh();
}
//定义画按键小方框的函数
void Draw_Small_Button(Button_XY Button_XY)
{
	LCD_DrawRectangle(Button_XY.X1*8+5,Button_XY.Y1*8+5,Button_XY.X2*8-5,Button_XY.Y2*8-5,POINT_COLOR);
	LCD_Refresh();
}

//定义填充按键的函数
void Fill_Button(Button_XY Button_XY,uint32_t Color)
{
	LCD_Color_Fill(Button_XY.X1*8+2,Button_XY.Y1*8+2,Button_XY.X2*8-2,Button_XY.Y2*8-2,Color);
	LCD_Refresh();
}
//定义填写文本的函数
void WR_Text(Text_XY Text_XY, uint8_t *TEXT, uint8_t SIZE, uint8_t MODE)
{
	LCD_ShowString(Text_XY.X*8,Text_XY.Y*8,84,30,SIZE,MODE,TEXT);
	LCD_Refresh();
}
//定义填写数字变量的函数
void WR_Num(Text_XY Text_XY, uint32_t NUM, uint8_t LEN)
{
	LCD_ShowNum(Text_XY.X*8,Text_XY.Y*8,NUM,10,24);
	LCD_Refresh();
}
//定义按钮按下判断函数
uint8_t Judge_Button(Button_XY Button_XY)
{
	if((Button_XY.X1*8 < last_tp_x)&&(last_tp_x < Button_XY.X2*8)&&(Button_XY.Y1*8 < last_tp_y)&&(last_tp_y < Button_XY.Y2*8))
		return 1;
	else
		return 0;
}
//定义按钮按下画框特效
void Press_Button(Button_XY Button_XY)
{
	POINT_COLOR=LCD_BLACK;
	Draw_Button(Button_XY);
	POINT_COLOR=LCD_WHITE;
	Draw_Small_Button(Button_XY);
	LCD_Refresh();
}
//定义按钮松开画框恢复
void Release_Button(Button_XY Button_XY)
{
	POINT_COLOR=LCD_BLACK;
	Draw_Small_Button(Button_XY);
	POINT_COLOR=LCD_WHITE;
	Draw_Button(Button_XY);
	LCD_Refresh();
}
