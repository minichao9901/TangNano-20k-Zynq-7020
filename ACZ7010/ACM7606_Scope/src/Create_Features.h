#ifndef _CREATE_FEATURES_H_
#define _CREATE_FEATURES_H_

#include "PageDesign.h"

//定义各个按键的属性
Button Button_RUN = {
		{15,395,105,60},	//按键坐标X1、Y1、宽度、高度
		LCD_BLACK,LCD_GREEN,//按键内的文本颜色和背景颜色
		32,{{"RUN"}}	//按键文本的字体大小和文本内容
};
Button Button_AUTO = {
		{15,315,105,60},
		LCD_BLACK,LCD_GREEN,
		32,{{"AUTO"}}
};
Button Button_CH_INC = {
		{715,60,70,60},
		LCD_WHITE,LCD_BLACK,
		32,{{'+'}}
};
Button Button_CH_DEC = {
		{715,175,70,60},
		LCD_WHITE,LCD_BLACK,
		32,{{'-'}}
};
Button Button_SA_INC = {
		{715,280,70,60},
		LCD_WHITE,LCD_BLACK,
		32,{{'+'}}
};
Button Button_SA_DEC = {
		{715,395,70,60},
		LCD_WHITE,LCD_BLACK,
		32,{{'-'}}
};

Button Button_Tri_Mode = {
		{15,160,105,65},
		LCD_WHITE,LCD_BLACK,
		24,{
				{"Auto"},
				{"Trigger"}
		}
};


Button Button_TriggerVal = {//当前电压触发值
		{15,240,105,60},
		LCD_WHITE,LCD_GRAY,
		24,{{"0.000V"}}
};

//定义波形滑块
Button Wave_Slider = {
		{450,10,240,40},
		LCD_PINK,LCD_BLACK,//按键内的文本颜色和背景颜色
		16,{{"  "}}	//按键文本的字体大小和文本内容
};

//定义滑块实体
Box_XY Slider = {
		450,10,40,40
};


//定义文本属性
Text Text_CHANNEL = {	//当前采样通道
		{705,125,90,45},
		LCD_WHITE,LCD_BLACK,
		24
};

Text Text_SAMPLE = {	//当前采样率
		{705,345,90,45},
		LCD_WHITE,LCD_BLACK,
		24
};

Text Text_Vmax = {		//当前电压的最大值
		{290,460,150,20},
		LCD_YELLOW,LCD_BLACK,
		16
};

Text Text_Vmin = {		//当前电压的最小值
		{125,460,150,20},
		LCD_YELLOW,LCD_BLACK,
		16
};

Text Text_Vpp = {		//当前电压的峰峰值
		{455,460,150,20},
		LCD_YELLOW,LCD_BLACK,
		16
};

Text Text_Freq = {		//当前波形的频率
		{620,460,150,20},
		LCD_YELLOW,LCD_BLACK,
		16
};

Text Text_Point_V = {		//当前标点电压值
		{145,20,150,20},
		LCD_CYAN,LCD_BLACK,
		16
};

Text Text_Point_T = {		//当前标点时间
		{295,20,150,20},
		LCD_CYAN,LCD_BLACK,
		16
};

Text Text_Date = {		//RTC时间
		{705,5,90,20},
		LCD_YELLOW,LCD_BLACK,
		12
};

Text Text_Clock = {		//RTC时间
		{705,25,90,20},
		LCD_YELLOW,LCD_BLACK,
		12
};

Text Text_TouchXY = {		//当前触摸坐标，调试时使用
		{290,5,150,20},
		LCD_YELLOW,LCD_BLACK,
		16
};

//定义波形界面的部分属性，剩下的使用Get_Waveform_Window_Parameters函数计算出来
Wave_Page ADC_Wave = {
		{135,60,565,395},	//波形窗口坐标X1、Y1、宽度、高度
		5,15				//电压量程和ADC位数
};

#endif /* _CREATE_FEATURES_H_ */
