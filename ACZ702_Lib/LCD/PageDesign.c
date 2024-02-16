#include "PageDesign.h"
#include "COMMON.h"

//绘制方框
void Draw_Box(Box_XY Box, uint32_t Color,int16_t Offset)
{
	LCD_DrawRectangle(Box.X1 + Offset, Box.Y1 + Offset,
			Box.X1 + Box.Width - Offset, Box.Y1 + Box.Height - Offset, Color);
}

//填充区域
void Fill_Box(Box_XY Box,uint32_t BackColor,int16_t Offset)
{
	LCD_Color_Fill(Box.X1+Offset,Box.Y1+Offset,
			Box.X1+Box.Width-Offset,Box.Y1+Box.Height-Offset,BackColor);
}

//通过Wave窗口的部分参数来自动补全其它参数
void Get_Waveform_Window_Parameters(Wave_Page *Wave)
{
	Wave->Wave_Area.X1 = Wave->Window.X1 + 40;			//计算波形区域的X1坐标
	Wave->Wave_Area.Y1 = Wave->Window.Y1 + 10;			//计算波形区域的Y1坐标
	Wave->Wave_Area.Width = Wave->Window.Width - 65;	//计算波形区域的宽度
	Wave->Wave_Area.Height = Wave->Window.Height - 35;	//计算波形区域的高度
	//波形范围除以高度得出每单位电压值代表的像素大小
	Wave->Precision = Wave->Wave_Area.Height / (float)((1 << Wave->ADC_Bits)*2);
}

//画出波形窗口和坐标
void Draw_Waveform_Windows(Wave_Page Wave, uint32_t Sample_Rate)
{
	Display_Time_Scale(Wave,Sample_Rate);	//显示X轴的时间刻度
	Draw_Grid_Background(Wave);				//显示网格背景
	Display_Voltage_Scale(Wave);			//显示Y轴的电压刻度
}

//绘制波形
void Draw_Waveform(Wave_Page Wave,int16_t *Pre_Wave_Data,int16_t *Wave_Data)
{
	uint16_t i,len;
	uint16_t XAxis_Y;

	len = Wave.Wave_Area.Width;
	XAxis_Y = Wave.Wave_Area.Y1 + Wave.Wave_Area.Height/2;

	//擦去旧波形并绘制新波形
	for(i=0;i<len-1;i++)
	{
		POINT_COLOR = PAGE_COLOR;
		LCD_DrawWaveLine(i + Wave.Wave_Area.X1, XAxis_Y - (int16_t)(Wave.Precision*Pre_Wave_Data[i]),
				i + 1 + Wave.Wave_Area.X1, XAxis_Y - (int16_t)(Wave.Precision*Pre_Wave_Data[i+1]));
		POINT_COLOR = LCD_YELLOW;
		LCD_DrawWaveLine(i + Wave.Wave_Area.X1, XAxis_Y - (int16_t)(Wave.Precision*Wave_Data[i]),
				i + 1 + Wave.Wave_Area.X1, XAxis_Y - (int16_t)(Wave.Precision*Wave_Data[i+1]));
	}

	LCD_Refresh();//刷新图像
}

//绘制网格背景
void Draw_Grid_Background(Wave_Page Wave)
{
	uint8_t i,j;
	//绘制网格背景
	for(i=0;i<11;i++) {
		for(j=0;j<51;j++) {
			LCD_DrawPoint(Wave.Wave_Area.X1 + j*Wave.Wave_Area.Width/50,
					Wave.Wave_Area.Y1 + i*Wave.Wave_Area.Height/10,LCD_WHITE);//绘制横网格
			LCD_DrawPoint(Wave.Wave_Area.X1 + i*Wave.Wave_Area.Width/10,
					Wave.Wave_Area.Y1 + j*Wave.Wave_Area.Height/50,LCD_WHITE);//绘制竖网格
			if(i==5) {
				LCD_DrawLine(Wave.Wave_Area.X1 + j*Wave.Wave_Area.Width/50,//绘制横中轴
						Wave.Wave_Area.Y1 + i*Wave.Wave_Area.Height/10 - 1,
						Wave.Wave_Area.X1 + j*Wave.Wave_Area.Width/50,
						Wave.Wave_Area.Y1 + i*Wave.Wave_Area.Height/10 + 1,LCD_WHITE);
				LCD_DrawLine(Wave.Wave_Area.X1 + i*Wave.Wave_Area.Width/10 - 1,//绘制竖中轴
						Wave.Wave_Area.Y1 + j*Wave.Wave_Area.Height/50,
						Wave.Wave_Area.X1 + i*Wave.Wave_Area.Width/10 + 1,
						Wave.Wave_Area.Y1 + j*Wave.Wave_Area.Height/50,LCD_WHITE);
			}
		}
	}

	//LCD_Refresh();//刷新图像
}

//显示X轴的时间刻度
void Display_Time_Scale(Wave_Page Wave,uint32_t Sample_Rate)
{
	char str[30];
	uint8_t i;
	uint16_t Byte;
	uint16_t X_Offset;
	uint16_t Y_Offset = 10;
	Box_XY Box_TimeScale;	//刻度区域
	Box_TimeScale.X1 = Wave.Window.X1 + 1;
	Box_TimeScale.Y1 = Wave.Wave_Area.Y1 + Wave.Wave_Area.Height + Y_Offset;
	Box_TimeScale.Width = Wave.Window.Width - 2;
	Box_TimeScale.Height = 12;
	POINT_COLOR = LCD_WHITE;//X轴标度颜色
	BACK_COLOR = PAGE_COLOR;//标度的背景颜色与页面一致
	//清除原有的刻度
	Fill_Box(Box_TimeScale,PAGE_COLOR, 0);
	//显示X轴的时间刻度
	for(i=0;i<11;i++){
		if(i <= 5)
			sprintf(str,"%gms",(float)(i-5)*Wave.Wave_Area.Width/(Sample_Rate*10));
		else
			sprintf(str,"+%gms",(float)(i-5)*Wave.Wave_Area.Width/(Sample_Rate*10));//加个正号

		//获取字符串长度，使刻度标注与竖网格居中对齐
		Byte = strlen(str);
		X_Offset = Byte*3;//一个字符宽度是6，居中需要偏移一半，半个就是3
		LCD_ShowString(Wave.Wave_Area.X1 + i * Wave.Wave_Area.Width/10 - X_Offset,
				Wave.Wave_Area.Y1 + Wave.Wave_Area.Height + Y_Offset,
				150, 30, 12, 0, (uint8_t*)str);
	}

	LCD_Refresh();//刷新图像
}
//显示Y轴的电压刻度
void Display_Voltage_Scale(Wave_Page Wave)
{
	uint8_t i;
	char str[30];

	//显示Y轴的电压刻度
	for (i=0;i<11;i++) {
		if (i < 5)
			sprintf(str,"+%.1fV",Wave.Range - i * Wave.Range/5);
		else if(i == 5)
			sprintf(str," %.1fV",Wave.Range - i * Wave.Range/5);
		else
			sprintf(str,"%.1fV",Wave.Range - i * Wave.Range/5);

		LCD_ShowString(Wave.Window.X1+5,
				Wave.Window.Y1+5 + i * Wave.Wave_Area.Height/10,
				150, 30, 12, 0, (uint8_t*)str);
	}

	LCD_Refresh();//刷新图像
}

//绘制普通按键
void Draw_Normal_Button(Button Button)
{
	uint8_t Byte = 0;
	uint8_t Rows = 0;
	uint8_t i;
	uint16_t X_Offset,Y_Offset;
	uint8_t Border_LineWidth = 2;//按键边框的线宽
	POINT_COLOR = Button.TextColor;
	BACK_COLOR = Button.BackColor;

	//画白色边框
	for(i=0;i<Border_LineWidth;i++)
		Draw_Box(Button.Box, LCD_WHITE, i);

	//画背景色边框
	for(i=Border_LineWidth;i<Border_LineWidth*2;i++)
		Draw_Box(Button.Box,PAGE_COLOR, i);


	//填充按键背景
	Fill_Box(Button.Box,Button.BackColor,Border_LineWidth*2);

	//计算Text的行数
	while (strlen(Button.Text[Rows]))
		Rows++;

	for(i=0;i<Rows;i++){
		Byte = strlen(Button.Text[i]);
		X_Offset = (Button.Box.Width - Byte*Button.TextSize/2)/2;
		Y_Offset = (Button.Box.Height - Button.TextSize*Rows)/(Rows+1) + Button.TextSize*i;
		LCD_ShowString(Button.Box.X1+X_Offset,Button.Box.Y1+Y_Offset,
				150,Button.TextSize+2,Button.TextSize,0,(uint8_t*)Button.Text[i]);
	}

	//LCD_Refresh();//刷新图像
}

//绘制普通文本
void Draw_Normal_Text(Text Text, char *str)

{
	uint16_t Byte;
	uint16_t X_Offset,Y_Offset;
	Box_XY Box = Text.Box;

	Byte = strlen(str);
	X_Offset = (Text.Box.Width - Byte*Text.TextSize/2)/2;
	Y_Offset = (Text.Box.Height - Text.TextSize)/2;

	POINT_COLOR = Text.TextColor;
	BACK_COLOR = Text.BackColor;
	Box.Width = X_Offset;
	Fill_Box(Box,Text.BackColor,0);//先擦除
	Box.X1 = Text.Box.X1 + Text.Box.Width - X_Offset;
	Fill_Box(Box,Text.BackColor,0);//先擦除
	LCD_ShowString(Text.Box.X1 + X_Offset,Text.Box.Y1 + Y_Offset,
			Text.Box.Width,Text.TextSize+2,Text.TextSize,0,(uint8_t*)str);

	//LCD_Refresh();//刷新图像
}

//绘制按键特效
void Draw_Button_Effect(Button Button)
{
	uint8_t Byte = 0;
	uint8_t Rows = 0;
	uint8_t i;
	uint16_t X_Offset,Y_Offset;
	uint8_t Border_LineWidth = 2;	//按键边框的线宽，值越大线越粗
	uint8_t Effect_Width = 3;	//按键特效的效果，值越大特效越明显
	POINT_COLOR = Button.TextColor;
	BACK_COLOR = Button.BackColor;

	//清除外框
	for(i=0;i<Border_LineWidth*4;i++)
		Draw_Box(Button.Box, PAGE_COLOR, i);

	//画白色边框
	for(i=0;i<Border_LineWidth;i++)
		Draw_Box(Button.Box, LCD_WHITE, i+Effect_Width);

	//填充按键背景
	Fill_Box(Button.Box,Button.BackColor,Border_LineWidth*2+Effect_Width);

	//计算Text的行数
	while (strlen(Button.Text[Rows]))
		Rows++;

	for(i=0;i<Rows;i++){
		Byte = strlen(Button.Text[i]);
		X_Offset = (Button.Box.Width - Byte*Button.TextSize/2)/2;
		Y_Offset = (Button.Box.Height - Button.TextSize*Rows)/(Rows+1) + Button.TextSize*i;
		LCD_ShowString(Button.Box.X1+X_Offset,Button.Box.Y1+Y_Offset,
				150,Button.TextSize+2,Button.TextSize,0,(uint8_t*)Button.Text[i]);
	}

	LCD_Refresh();//刷新图像
}

//绘制特制的标线，Direction为0表示横线，为1表示竖线
void Draw_Mark_Line(uint8_t Direction,uint16_t Point, uint16_t Start,uint16_t End,uint32_t Color)
{
	uint8_t i;
	uint8_t Size = 8;


	if(Direction) {
		LCD_DrawLine(Point,Start,Point,End,Color);//画竖线
		//画三角标
		for(i=0;i<Size;i++) {
			LCD_DrawLine(Point-i,Start-i,Point+i,Start-i,Color);//画“——”
			LCD_DrawLine(Point-i,Start-i,Point,Start,Color);//画“\”
			LCD_DrawLine(Point,Start,Point+i,Start-i,Color);//画“/”
		}
	}
	else {
		LCD_DrawLine(Start,Point,End,Point,Color);//画横线
		//画三角标
		for(i=0;i<Size;i++) {
			LCD_DrawLine(End+i,Point-i,End+i,Point+i,Color);//画“|”
			LCD_DrawLine(End+i,Point-i,End,Point,Color);//画“/”
			LCD_DrawLine(End,Point,End+i,Point+i,Color);//画“\”
		}
	}
}
