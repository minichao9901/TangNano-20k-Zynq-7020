#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "xil_cache.h"
#include "xparameters.h"
#include "xaxivdma.h"
#include "xaxivdma_i.h"
#include "display_ctrl.h"
#include "vdma_api.h"
#include "DDS_Module.h"
#include "lcd.h"
#include "Touch.h"
#include "dds.h"
#include "HomePage.h"
#include "logo.h"

//定义Button_XY类型的结构体
Button_XY BT_OCH1	= {17, 27, 7, 14};	//CH1选择输出按钮
Button_XY BT_OCH2	= {30, 40, 7, 14};	//CH2选择输出按钮
Button_XY BT_Wave1	= {17, 27, 17, 27};
Button_XY BT_Wave2	= {30, 40, 17, 27};
Button_XY BT_Freq	= {4, 14, 30, 36};
Button_XY BT_Phas	= {4, 14, 38, 44};
Button_XY BT_Width	= {4, 14, 46, 52};

Button_XY BT_KHz	= {46, 56, 15, 22};
Button_XY BT_MHz	= {59, 69, 15, 22};
Button_XY BT_ICH1	= {72, 82, 15, 22};	//CH1设置输入按钮
Button_XY BT_ICH2	= {85, 95, 15, 22};	//CH2设置输入按钮
Button_XY BT_NUM1	= {46, 56, 25, 32};
Button_XY BT_NUM2	= {59, 69, 25, 32};
Button_XY BT_NUM3	= {72, 82, 25, 32};
Button_XY BT_ICH12	= {85, 95, 25, 32};	//CH1+CH2设置输入按钮
Button_XY BT_NUM4	= {46, 56, 35, 42};
Button_XY BT_NUM5	= {59, 69, 35, 42};
Button_XY BT_NUM6	= {72, 82, 35, 42};
Button_XY BT_NUM0	= {85, 95, 35, 42};
Button_XY BT_NUM7	= {46, 56, 45, 52};
Button_XY BT_NUM8	= {59, 69, 45, 52};
Button_XY BT_NUM9	= {72, 82, 45, 52};
Button_XY BT_AC		= {85, 95, 45, 52};

//定义Text_XY类型的结构体
Text_XY TX_Wave		= {6, 20};
Text_XY TX_Freq1	= {16, 30};
Text_XY TX_Hz1		= {26, 32};//Freq1单位Hz显示
Text_XY TX_Freq2	= {29, 30};
Text_XY TX_Hz2		= {39, 32};//Freq2单位Hz显示
Text_XY TX_Phas1	= {16, 39.5};
Text_XY TX_Phas2	= {29, 39.5};
Text_XY TX_Width1	= {18, 47.5};
Text_XY TX_Width2	= {31, 47.5};
Text_XY TX_Units	= {91, 7.5};//输入单位显示

Text_XY TX_INum	= {74, 7.5};//输入数字显示

//定义按钮上的文本
Text_XY TX_OCH1	= {20, 9};
Text_XY TX_OCH2	= {33, 9};
Text_XY TX_Wave1_1= {19, 18};
Text_XY TX_Wave1_2= {19, 21};
Text_XY TX_Wave1_3= {19, 24};
Text_XY TX_Wave2_1= {32, 18};
Text_XY TX_Wave2_2= {32, 21};
Text_XY TX_Wave2_3= {32, 24};
Text_XY TX_Freq	= {6, 31.5};
Text_XY TX_Phas	= {6, 39.5};
Text_XY TX_Width= {5.5, 47.5};

Text_XY TX_KHz	= {49, 17};
Text_XY TX_MHz	= {62, 17};
Text_XY TX_ICH1	= {75, 17};
Text_XY TX_ICH2	= {88, 17};
Text_XY TX_NUM1	= {49, 27};
Text_XY TX_NUM2	= {62, 27};
Text_XY TX_NUM3	= {75, 27};
Text_XY TX_ICH12= {86, 27};
Text_XY TX_NUM4	= {49, 37};
Text_XY TX_NUM5	= {62, 37};
Text_XY TX_NUM6	= {75, 37};
Text_XY TX_NUM0	= {88, 37};
Text_XY TX_NUM7	= {49, 47};
Text_XY TX_NUM8	= {62, 47};
Text_XY TX_NUM9	= {75, 47};
Text_XY TX_AC	= {88.5, 47};

void touch_button();
void Button_effects();//按钮特效

//定义辅助变量
uint8_t cnt;
uint8_t Input_Flag=0;
uint32_t color_save;
static s8 str[20]={0};
uint8_t last_touch = 0;
u64 Temp_Num = 0;
uint16_t last_tp_x;        //上一次触摸坐标x
uint16_t last_tp_y;		//上一次触摸坐标y
int tmp;

//定义信号输出相关变量
enum OUT_CHANNEL Out_Channel;//定义输出通道Out_Channel
enum OUT_WAVE Out1_Wave,Out2_Wave;//定义输出波形Out_Wave
enum SETTING_MODE Setting_Mode;//定义设置项
uint32_t Out1_Freq = 1000;		//定义输出频率为1kHz
uint32_t Out2_Freq = 1000;		//定义输出频率为1kHz
uint16_t Out1_Phas = 0;			//定义相位为0Deg
uint16_t Out2_Phas = 0;			//定义相位为0Deg
uint8_t Out1_Width = 50;				//定义占空比50%
uint8_t Out2_Width = 50;				//定义占空比50%

int main()
{
	Out1_Wave = SINE_OUT;		//初始化输出正弦波
	Out2_Wave = SINE_OUT;		//初始化输出正弦波
	Out_Channel = CH12_OUT;		//初始化通道1+2同时输出
	Setting_Mode = 1;

	dds_init();
	LCD_Init();
	GT9147_Init();

	LCD_Clear(LCD_BLACK);
	usleep(13000);
	POINT_COLOR=LCD_WHITE;
	BACK_COLOR=LCD_BLACK;

 	LCD_DisplayPic(18,68,gImage_logo);//画LOGO

	//画白框界面
	LCD_DrawRectangle(Left_Line1*8,Up_Line1*8,Right_Line1*8,Down_Line1*8,POINT_COLOR);
	LCD_DrawRectangle(Left_Line2*8,Up_Line2*8,Right_Line2*8,Down_Line2*8,POINT_COLOR);
	LCD_DrawRectangle(Left_Line3*8,Up_Line3*8,Right_Line3*8,Down_Line3*8,POINT_COLOR);

	//画按钮界面
	Draw_Button(BT_OCH1);
	Draw_Button(BT_OCH2);
	Draw_Button(BT_Wave1);
	Draw_Button(BT_Wave2);
	Draw_Button(BT_Freq);
	Draw_Button(BT_Phas);
	Draw_Button(BT_Width);

	Draw_Button(BT_KHz);
	Draw_Button(BT_MHz);
	Draw_Button(BT_ICH1);
	Draw_Button(BT_ICH2);
	Draw_Button(BT_NUM1);
	Draw_Button(BT_NUM2);
	Draw_Button(BT_NUM3);
	Draw_Button(BT_ICH12);
	Draw_Button(BT_NUM4);
	Draw_Button(BT_NUM5);
	Draw_Button(BT_NUM6);
	Draw_Button(BT_NUM0);
	Draw_Button(BT_NUM7);
	Draw_Button(BT_NUM8);
	Draw_Button(BT_NUM9);
	Draw_Button(BT_AC);

	LCD_ShowString(300, 0, 200, 30, 24, 1, "www.corecourse.cn");
	//写界面文本
	//WR_Text(TX_Wave, "Wave", 24, 1);
	WR_Text(TX_Units, "Hz ", 24, 1);

	LCD_ShowString(23*8, 33*8, 168, 24, 24, 0, "Hz");
	LCD_ShowString(36*8, 33*8, 168, 24, 24, 0, "Hz");

	LCD_ShowxNum(FREQ1_X*8,FREQ1_Y*8,0,3,24,0x80);
	LCD_ShowxNum((FREQ1_X+5)*8,FREQ1_Y*8,1,3,24,0x80);
	LCD_ShowxNum(FREQ1_X*8,(FREQ1_Y+3)*8,000,3,24,0x80);

	LCD_ShowxNum(FREQ2_X*8,FREQ2_Y*8,0,3,24,0x80);
	LCD_ShowxNum((FREQ2_X+5)*8,FREQ2_Y*8,1,3,24,0x80);
	LCD_ShowxNum(FREQ2_X*8,(FREQ2_Y+3)*8,000,3,24,0x80);

	sprintf((char*)str,"%3d Deg",Out1_Phas);
	WR_Text(TX_Phas1, str, 24,0);
	sprintf((char*)str,"%3d Deg",Out2_Phas);
	WR_Text(TX_Phas2, str, 24,0);

	sprintf((char*)str,"%3d %%",Out1_Width);
	WR_Text(TX_Width1, str, 24,0);
	sprintf((char*)str,"%3d %%",Out2_Width);
	WR_Text(TX_Width2, str, 24,0);

	//写按键文本
	WR_Text(TX_OCH1, "CH1", 24, 1);
	WR_Text(TX_OCH2, "CH2", 24, 1);
	WR_Text(TX_Wave1_1, " Sine ", 16, 1);
	WR_Text(TX_Wave1_2, " Squa ", 16, 1);
	WR_Text(TX_Wave1_3, " Tria ", 16, 1);
	WR_Text(TX_Wave2_1, " Sine ", 16, 1);
	WR_Text(TX_Wave2_2, " Squa ", 16, 1);
	WR_Text(TX_Wave2_3, " Tria ", 16, 1);
	WR_Text(TX_Freq, "Freq", 24, 1);
	WR_Text(TX_Phas, "Phas", 24, 1);
	WR_Text(TX_Width, "Width", 24, 1);

	WR_Text(TX_KHz, " K ", 24, 1);
	WR_Text(TX_MHz, " M ", 24, 1);
	WR_Text(TX_ICH1, "CH1", 24, 1);
	WR_Text(TX_ICH2, "CH2", 24, 1);
	WR_Text(TX_NUM1, " 1 ", 24, 1);
	WR_Text(TX_NUM2, " 2 ", 24, 1);
	WR_Text(TX_NUM3, " 3 ", 24, 1);
	WR_Text(TX_ICH12, "CH1+2", 24, 1);
	WR_Text(TX_NUM4, " 4 ", 24, 1);
	WR_Text(TX_NUM5, " 5 ", 24, 1);
	WR_Text(TX_NUM6, " 6 ", 24, 1);
	WR_Text(TX_NUM0, " 0 ", 24, 1);
	WR_Text(TX_NUM7, " 7 ", 24, 1);
	WR_Text(TX_NUM8, " 8 ", 24, 1);
	WR_Text(TX_NUM9, " 9 ", 24, 1);
	WR_Text(TX_AC, "AC", 24, 1);

	//初始化界面
	Fill_Button(BT_OCH1,LCD_GREEN);
	Fill_Button(BT_OCH2,LCD_GREEN);
	Fill_Button(BT_Freq,LCD_GREEN);
	POINT_COLOR=LCD_BLACK;
	BACK_COLOR=LCD_GREEN;
	WR_Text(TX_OCH1, "CH1", 24, 1);
	WR_Text(TX_OCH2, "CH2", 24, 1);
	WR_Text(TX_Wave1_1, " Sine ", 16, 0);
	WR_Text(TX_Wave2_1, " Sine ", 16, 0);
	WR_Text(TX_Freq, "Freq", 24, 1);

	BACK_COLOR=LCD_BLACK;
	POINT_COLOR=LCD_WHITE;
	//WR_Num(TX_INum, Temp_Num, 10);
	LCD_Refresh();
	while(1)
	{
		touch_button();
	}

	return 0;
}

//按键与触摸部分
void touch_button()
{
	GT9147_Scan();//触摸扫描
	Button_effects();//按键特效
	if ((last_touch==1)&&((tp_sta&1)==0))//检测到单次触摸并放开
	{
		if((Left_Line1*8 < last_tp_x)&&(last_tp_x < Right_Line1*8)&&(Up_Line1*8 < last_tp_y)&&(last_tp_y < Down_Line1*8))//如果触摸坐标在左侧窗口(1)
		{
			if(Judge_Button(BT_OCH1))
			{
				if((Out_Channel&1) == 0)//如果通道一关闭，则打开通道一
				{
					Out_Channel += CH1_OUT;
					Fill_Button(BT_OCH1,LCD_GREEN);
					POINT_COLOR=LCD_BLACK;
					WR_Text(TX_OCH1, "CH1", 24, 1);

					//波形对齐
					SET_CHAB_EN(0);
					SET_CHAB_EN(3);


				}
				else//如果通道一打开,则关闭通道一
				{
					Out_Channel -= CH1_OUT;
					Fill_Button(BT_OCH1,LCD_RED);
					POINT_COLOR=LCD_BLACK;
					WR_Text(TX_OCH1, "CH1", 24, 1);
				}
				SET_CHAB_EN(Out_Channel);
			}
			if(Judge_Button(BT_OCH2))
			{
				if((Out_Channel&2) == 0)//如果通道二关闭，则打开通道二
				{
					Out_Channel += CH2_OUT;
					Fill_Button(BT_OCH2,LCD_GREEN);
					POINT_COLOR=LCD_BLACK;
					WR_Text(TX_OCH2, "CH2", 24, 1);

					//波形对齐
					SET_CHAB_EN(0);
					SET_CHAB_EN(3);
				}
				else//如果通道二打开，则关闭通道二
				{
					Out_Channel -= CH2_OUT;
					Fill_Button(BT_OCH2,LCD_RED);
					POINT_COLOR=LCD_BLACK;
					WR_Text(TX_OCH2, "CH2", 24, 1);
				}
				SET_CHAB_EN(Out_Channel);

			}
			if(Judge_Button(BT_Wave1))
			{
				Release_Button(BT_Wave1);
				if(Out1_Wave<3)
					Out1_Wave++;
				else
					Out1_Wave = 1;

				if(Out1_Wave == 1)//如果波形为Sine
				{
					POINT_COLOR=LCD_BLACK;
					BACK_COLOR=LCD_GREEN;
					WR_Text(TX_Wave1_1, " Sine ", 16, 0);
					POINT_COLOR=LCD_WHITE;
					BACK_COLOR=LCD_BLACK;
					WR_Text(TX_Wave1_2, " Squa ", 16, 0);
					WR_Text(TX_Wave1_3, " Tria ", 16, 0);

					Init_DDS_ROM_sine(RAM_A_BASE);
				}
				else if(Out1_Wave == 2)//如果波形为Squa
				{
					POINT_COLOR=LCD_BLACK;
					BACK_COLOR=LCD_GREEN;
					WR_Text(TX_Wave1_2, " Squa ", 16, 0);
					POINT_COLOR=LCD_WHITE;
					BACK_COLOR=LCD_BLACK;
					WR_Text(TX_Wave1_1, " Sine ", 16, 0);
					WR_Text(TX_Wave1_3, " Tria ", 16, 0);

					Init_DDS_ROM_tra(RAM_A_BASE);
				}
				else if(Out1_Wave == 3)//如果波形为Tria
				{
					POINT_COLOR=LCD_BLACK;
					BACK_COLOR=LCD_GREEN;
					WR_Text(TX_Wave1_3, " Tria ", 16, 0);
					POINT_COLOR=LCD_WHITE;
					BACK_COLOR=LCD_BLACK;
					WR_Text(TX_Wave1_1, " Sine ", 16, 0);
					WR_Text(TX_Wave1_2, " Squa ", 16, 0);

					Init_DDS_ROM_tra(RAM_A_BASE);
				}
				SET_CHAB_Mode(CHAB_MODE);

				//波形对齐
				SET_CHAB_EN(0);
				SET_CHAB_EN(3);

			}
			if(Judge_Button(BT_Wave2))
			{
				Release_Button(BT_Wave2);
				if(Out2_Wave<3)
					Out2_Wave++;
				else
					Out2_Wave = 1;

				if(Out2_Wave == 1)//如果波形为Sine
				{
					POINT_COLOR=LCD_BLACK;
					BACK_COLOR=LCD_GREEN;
					WR_Text(TX_Wave2_1, " Sine ", 16, 0);
					POINT_COLOR=LCD_WHITE;
					BACK_COLOR=LCD_BLACK;
					WR_Text(TX_Wave2_2, " Squa ", 16, 0);
					WR_Text(TX_Wave2_3, " Tria ", 16, 0);

					Init_DDS_ROM_sine(RAM_B_BASE);
				}
				else if(Out2_Wave == 2)//如果波形为Squa
				{
					POINT_COLOR=LCD_BLACK;
					BACK_COLOR=LCD_GREEN;
					WR_Text(TX_Wave2_2, " Squa ", 16, 0);
					POINT_COLOR=LCD_WHITE;
					BACK_COLOR=LCD_BLACK;
					WR_Text(TX_Wave2_1, " Sine ", 16, 0);
					WR_Text(TX_Wave2_3, " Tria ", 16, 0);

					Init_DDS_ROM_tra(RAM_B_BASE);
				}
				else if(Out2_Wave == 3)//如果波形为Tria
				{
					POINT_COLOR=LCD_BLACK;
					BACK_COLOR=LCD_GREEN;
					WR_Text(TX_Wave2_3, " Tria ", 16, 0);
					POINT_COLOR=LCD_WHITE;
					BACK_COLOR=LCD_BLACK;
					WR_Text(TX_Wave2_1, " Sine ", 16, 0);
					WR_Text(TX_Wave2_2, " Squa ", 16, 0);

					Init_DDS_ROM_tra(RAM_B_BASE);
				}

				SET_CHAB_Mode(CHAB_MODE);

				//波形对齐
				SET_CHAB_EN(0);
				SET_CHAB_EN(3);

			}
			if(Judge_Button(BT_Freq))
			{
				if(Setting_Mode != Freq_Set)//设置项不为Freq_Set,若为Freq_Set则不做处理
				{
					Fill_Button(BT_Freq,LCD_GREEN);
					POINT_COLOR=LCD_BLACK;
					WR_Text(TX_Freq, "Freq", 24, 1);

					POINT_COLOR=LCD_WHITE;
					if(Setting_Mode == Phas_Set)
					{
						Fill_Button(BT_Phas,LCD_BLACK);
						WR_Text(TX_Phas, "Phas", 24, 1);
					}
					else if(Setting_Mode == Width_Set)
					{
						Fill_Button(BT_Width,LCD_BLACK);
						WR_Text(TX_Width, "Width", 24, 1);
					}

					Setting_Mode = Freq_Set;//项设置为Freq_Set
					Temp_Num = 0;
					Input_Flag=0;//输入标志为0
					//WR_Num(TX_INum, Temp_Num, 10);
					WR_Text(TX_Units, "Hz ", 24, 0);
					LCD_Color_Fill(Left_Line3*8+1,Up_Line3*8+1,Right_Line3*8-1,Down_Line3*8-1,LCD_BLACK);
				}
			}
			if(Judge_Button(BT_Phas))
			{
				if(Setting_Mode != Phas_Set)//设置项不为Phas_Set
				{
					Fill_Button(BT_Phas,LCD_GREEN);
					POINT_COLOR=LCD_BLACK;
					WR_Text(TX_Phas, "Phas", 24, 1);

					//清除痕迹
					POINT_COLOR=LCD_WHITE;
					if(Setting_Mode == Freq_Set)
					{
						Fill_Button(BT_Freq,LCD_BLACK);
						WR_Text(TX_Freq, "Freq", 24, 1);
					}
					else if(Setting_Mode == Width_Set)
					{
						Fill_Button(BT_Width,LCD_BLACK);
						WR_Text(TX_Width, "Width", 24, 1);
					}

					Setting_Mode = Phas_Set;
					Temp_Num = 0;
					Input_Flag=0;//输入标志为0
					//WR_Num(TX_INum, Temp_Num, 10);
					WR_Text(TX_Units, "Deg", 24, 0);
					LCD_Color_Fill(Left_Line3*8+1,Up_Line3*8+1,Right_Line3*8-1,Down_Line3*8-1,LCD_BLACK);
				}
			}
			if(Judge_Button(BT_Width))
			{
				if(Setting_Mode != Width_Set)//设置项不为Width_Set
				{
					Fill_Button(BT_Width,LCD_GREEN);
					POINT_COLOR=LCD_BLACK;
					WR_Text(TX_Width, "Width", 24, 1);

					//清除痕迹
					POINT_COLOR=LCD_WHITE;
					if(Setting_Mode == Freq_Set)
					{
						Fill_Button(BT_Freq,LCD_BLACK);
						WR_Text(TX_Freq, "Freq", 24, 1);
					}
					else if(Setting_Mode == Phas_Set)
					{
						Fill_Button(BT_Phas,LCD_BLACK);
						WR_Text(TX_Phas, "Phas", 24, 1);
					}
					Setting_Mode = Width_Set;
					Temp_Num = 0;
					Input_Flag=0;//输入标志为0
					//WR_Num(TX_INum, Temp_Num, 10);
					WR_Text(TX_Units, "%  ", 24, 0);
					LCD_Color_Fill(Left_Line3*8+1,Up_Line3*8+1,Right_Line3*8-1,Down_Line3*8-1,LCD_BLACK);
				}
			}
		}

		if((Left_Line2*8 < last_tp_x)&&(last_tp_x < Right_Line2*8)&&(Up_Line2*8 < last_tp_y)&&(last_tp_y < Down_Line2*8))//如果触摸坐标在右侧输入窗口(2)
		{
			POINT_COLOR=LCD_WHITE;//按键输入界面被触摸，画笔颜色变为白色
			BACK_COLOR=LCD_BLACK;//按键输入界面被触摸，文字背景颜色变为黑色
			if((BT_KHz.X1*8 < last_tp_x)&&(last_tp_x < BT_KHz.X2*8))
			{
				if((BT_KHz.Y1*8 < last_tp_y)&&(last_tp_y < BT_KHz.Y2*8))//KHz
				{
					Release_Button(BT_KHz);
					Temp_Num *= 1000;
				}
				if((BT_NUM1.Y1*8 < last_tp_y)&&(last_tp_y < BT_NUM1.Y2*8))//1
				{
					Release_Button(BT_NUM1);
					Temp_Num *= 10 ;
					Temp_Num += 1;
				}
				if((BT_NUM4.Y1*8 < last_tp_y)&&(last_tp_y < BT_NUM4.Y2*8))//4
				{
					Release_Button(BT_NUM4);
					Temp_Num *= 10 ;
					Temp_Num += 4;
				}
				if((BT_NUM7.Y1*8 < last_tp_y)&&(last_tp_y < BT_NUM7.Y2*8))//7
				{
					Release_Button(BT_NUM7);
					Temp_Num *= 10 ;
					Temp_Num += 7;
				}
				Input_Flag=1;//输入标志为1
			}

			if((BT_MHz.X1*8 < last_tp_x)&&(last_tp_x < BT_MHz.X2*8))
			{
				if((BT_KHz.Y1*8 < last_tp_y)&&(last_tp_y < BT_KHz.Y2*8))//MHz
				{
					Release_Button(BT_MHz);
					Temp_Num *= 1000000 ;
				}
				if((BT_NUM1.Y1*8 < last_tp_y)&&(last_tp_y < BT_NUM1.Y2*8))//2
				{
					Release_Button(BT_NUM2);
					Temp_Num *= 10 ;
					Temp_Num += 2;
				}
				if((BT_NUM4.Y1*8 < last_tp_y)&&(last_tp_y < BT_NUM4.Y2*8))//5
				{
					Release_Button(BT_NUM5);
					Temp_Num *= 10 ;
					Temp_Num += 5;
				}
				if((BT_NUM7.Y1*8 < last_tp_y)&&(last_tp_y < BT_NUM7.Y2*8))//8
				{
					Release_Button(BT_NUM8);
					Temp_Num *= 10 ;
					Temp_Num += 8;
				}
				Input_Flag=1;//输入标志为1
			}

			if((BT_ICH1.X1*8 < last_tp_x)&&(last_tp_x < BT_ICH1.X2*8))
			{
				if((BT_KHz.Y1*8 < last_tp_y)&&(last_tp_y < BT_KHz.Y2*8))//CH1
				{
					Release_Button(BT_ICH1);

					if(Input_Flag==1)//输入标志为1时更改数值
					{
						if(Setting_Mode == Freq_Set)
						{
							Out1_Freq = Temp_Num;

							LCD_ShowxNum(FREQ1_X*8,FREQ1_Y*8,Out1_Freq/1000000,3,24,0x80);
							LCD_ShowxNum((FREQ1_X+5)*8,FREQ1_Y*8,(Out1_Freq/1000)%1000,3,24,0x80);
							LCD_ShowxNum(FREQ1_X*8,(FREQ1_Y+3)*8,Out1_Freq%1000,3,24,0x80);

							SET_CHA_Freq(Calculate_Fword(CHA_FREQ));
						}
						if(Setting_Mode == Phas_Set)
						{
							Out1_Phas = Temp_Num;
							sprintf((char*)str,"%3d Deg",Out1_Phas);
							WR_Text(TX_Phas1, str, 24,0);

							SET_CHAB_Phas(CHAB_PHASE);
						}
						if(Setting_Mode == Width_Set)
						{
							Out1_Width = Temp_Num;
							sprintf((char*)str,"%3d %%",Out1_Width);
							WR_Text(TX_Width1, str, 24,0);

							SET_CHAB_Width(CHAB_PLUSE_WIDTH);
						}
						Temp_Num = 0;//赋值后清零

						//波形对齐
						SET_CHAB_EN(0);
						SET_CHAB_EN(3);

						Input_Flag=0;//输入标志为0
					}
				}
				if((BT_NUM1.Y1*8 < last_tp_y)&&(last_tp_y < BT_NUM1.Y2*8))//3
				{
					Release_Button(BT_NUM3);
					Temp_Num *= 10 ;
					Temp_Num += 3;

					Input_Flag=1;//输入标志为1
				}
				if((BT_NUM4.Y1*8 < last_tp_y)&&(last_tp_y < BT_NUM4.Y2*8))//6
				{
					Release_Button(BT_NUM6);
					Temp_Num *= 10 ;
					Temp_Num += 6;

					Input_Flag=1;//输入标志为1
				}
				if((BT_NUM7.Y1*8 < last_tp_y)&&(last_tp_y < BT_NUM7.Y2*8))//9
				{
					Release_Button(BT_NUM9);
					Temp_Num *= 10 ;
					Temp_Num += 9;

					Input_Flag=1;//输入标志为1
				}
			}
			if((BT_ICH2.X1*8 < last_tp_x)&&(last_tp_x < BT_ICH2.X2*8))
			{
				if((BT_KHz.Y1*8 < last_tp_y)&&(last_tp_y < BT_KHz.Y2*8))//CH2
				{
					Release_Button(BT_ICH2);
					if(Input_Flag==1)//输入标志为1时更改数值
					{
						if(Setting_Mode == Freq_Set)
						{
							Out2_Freq = Temp_Num;

							LCD_ShowxNum(FREQ2_X*8,FREQ2_Y*8,Out2_Freq/1000000,3,24,0x80);
							LCD_ShowxNum((FREQ2_X+5)*8,FREQ2_Y*8,(Out2_Freq/1000)%1000,3,24,0x80);
							LCD_ShowxNum(FREQ2_X*8,(FREQ2_Y+3)*8,Out2_Freq%1000,3,24,0x80);

							SET_CHB_Freq(Calculate_Fword(CHB_FREQ));
						}
						if(Setting_Mode == Phas_Set)
						{
							Out2_Phas = Temp_Num;
							sprintf((char*)str,"%3d Deg",Out2_Phas);
							WR_Text(TX_Phas2, str, 24,0);

							SET_CHAB_Phas(CHAB_PHASE);
						}
						if(Setting_Mode == Width_Set)
						{
							Out2_Width = Temp_Num;
							sprintf((char*)str,"%3d %%",Out2_Width);
							WR_Text(TX_Width2, str, 24,0);

							SET_CHAB_Width(CHAB_PLUSE_WIDTH);
						}
						Temp_Num = 0;//赋值后清零

						//波形对齐
						SET_CHAB_EN(0);
						SET_CHAB_EN(3);

						Input_Flag=0;//输入标志为0
					}

				}
				if((BT_NUM1.Y1*8 < last_tp_y)&&(last_tp_y < BT_NUM1.Y2*8))//CH1+2
				{
					Release_Button(BT_ICH12);
					if(Input_Flag==1)//输入标志为1时更改数值
					{
						if(Setting_Mode == Freq_Set)
						{
							Out1_Freq = Temp_Num;
							Out2_Freq = Temp_Num;

							LCD_ShowxNum(FREQ1_X*8,FREQ1_Y*8,Out1_Freq/1000000,3,24,0x80);
							LCD_ShowxNum((FREQ1_X+5)*8,FREQ1_Y*8,(Out1_Freq/1000)%1000,3,24,0x80);
							LCD_ShowxNum(FREQ1_X*8,(FREQ1_Y+3)*8,Out1_Freq%1000,3,24,0x80);

							LCD_ShowxNum(FREQ2_X*8,FREQ2_Y*8,Out2_Freq/1000000,3,24,0x80);
							LCD_ShowxNum((FREQ2_X+5)*8,FREQ2_Y*8,(Out2_Freq/1000)%1000,3,24,0x80);
							LCD_ShowxNum(FREQ2_X*8,(FREQ2_Y+3)*8,Out2_Freq%1000,3,24,0x80);

							SET_CHA_Freq(Calculate_Fword(CHA_FREQ));
							SET_CHB_Freq(Calculate_Fword(CHB_FREQ));

						}
						if(Setting_Mode == Phas_Set)
						{
							Out1_Phas = Temp_Num;
							Out2_Phas = Temp_Num;
							sprintf((char*)str,"%3d Deg",Out1_Phas);
							WR_Text(TX_Phas1, str, 24,0);
							sprintf((char*)str,"%3d Deg",Out2_Phas);
							WR_Text(TX_Phas2, str, 24,0);

							SET_CHAB_Phas(CHAB_PHASE);
						}
						if(Setting_Mode == Width_Set)
						{
							Out1_Width = Temp_Num;
							Out2_Width = Temp_Num;
							sprintf((char*)str,"%3d %%",Out1_Width);
							WR_Text(TX_Width1, str, 24,0);
							sprintf((char*)str,"%3d %%",Out2_Width);
							WR_Text(TX_Width2, str, 24,0);

							SET_CHAB_Width(CHAB_PLUSE_WIDTH);
						}

						Temp_Num = 0;//赋值后清零

						//波形对齐
						SET_CHAB_EN(0);
						SET_CHAB_EN(3);

						Input_Flag=0;//输入标志为0
					}

				}
				if((BT_NUM4.Y1*8 < last_tp_y)&&(last_tp_y < BT_NUM4.Y2*8))//0
				{
					Release_Button(BT_NUM0);
					Temp_Num *= 10 ;

					Input_Flag=1;//输入标志为1
				}
				if((BT_NUM7.Y1*8 < last_tp_y)&&(last_tp_y < BT_NUM7.Y2*8))//AC
				{
					Release_Button(BT_AC);
					Temp_Num = 0;

					Input_Flag=1;//输入标志为1
				}
			}
			//限制在不同模式下Temp_Num的最大输入值
			if(Setting_Mode == Freq_Set)
			{
				if(Temp_Num > 100000000)
					Temp_Num = 100000000;
			}
			if(Setting_Mode == Phas_Set)
			{
				if(Temp_Num > 360)
					Temp_Num = 360;
			}
			if(Setting_Mode == Width_Set)
			{
				if(Temp_Num > 100)
					Temp_Num = 100;
			}
			//刷新显示的Temp_Num的值
			POINT_COLOR=LCD_WHITE;
			if(Input_Flag==1)//输入标志为时显示数值
				WR_Num(TX_INum, Temp_Num, 10);
			else
				LCD_Color_Fill(Left_Line3*8+1,Up_Line3*8+1,Right_Line3*8-1,Down_Line3*8-1,LCD_BLACK);

		}
	}


	last_touch = tp_sta&1;//记录上一次的触摸情况
	last_tp_x = tp_x[0];//记录上一次的触摸坐标
	last_tp_y = tp_y[0];
}

void Button_effects()//按钮特效
{
	if(tp_sta&1)//当按钮按下
	{
		if(Judge_Button(BT_KHz))//KHz
			Press_Button(BT_KHz);//显示按下特效
		else
			Release_Button(BT_KHz);//显示松开特效
		if(Judge_Button(BT_NUM1))//NUM1
			Press_Button(BT_NUM1);
		else
			Release_Button(BT_NUM1);
		if(Judge_Button(BT_NUM4))//NUM4
			Press_Button(BT_NUM4);
		else
			Release_Button(BT_NUM4);
		if(Judge_Button(BT_NUM7))//NUM7
			Press_Button(BT_NUM7);
		else
			Release_Button(BT_NUM7);
		if(Judge_Button(BT_MHz))//MHz
			Press_Button(BT_MHz);
		else
			Release_Button(BT_MHz);
		if(Judge_Button(BT_NUM2))//NUM2
			Press_Button(BT_NUM2);
		else
			Release_Button(BT_NUM2);
		if(Judge_Button(BT_NUM5))//NUM5
			Press_Button(BT_NUM5);
		else
			Release_Button(BT_NUM5);
		if(Judge_Button(BT_NUM8))//NUM8
			Press_Button(BT_NUM8);
		else
			Release_Button(BT_NUM8);
		if(Judge_Button(BT_ICH1))//ICH1
			Press_Button(BT_ICH1);
		else
			Release_Button(BT_ICH1);
		if(Judge_Button(BT_NUM3))//NUM3
			Press_Button(BT_NUM3);
		else
			Release_Button(BT_NUM3);
		if(Judge_Button(BT_NUM6))//NUM6
			Press_Button(BT_NUM6);
		else
			Release_Button(BT_NUM6);
		if(Judge_Button(BT_NUM9))//NUM9
			Press_Button(BT_NUM9);
		else
			Release_Button(BT_NUM9);
		if(Judge_Button(BT_ICH2))//ICH2
			Press_Button(BT_ICH2);
		else
			Release_Button(BT_ICH2);
		if(Judge_Button(BT_ICH12))//ICH12
			Press_Button(BT_ICH12);
		else
			Release_Button(BT_ICH12);
		if(Judge_Button(BT_NUM0))//NUM0
			Press_Button(BT_NUM0);
		else
			Release_Button(BT_NUM0);
		if(Judge_Button(BT_AC))//AC
			Press_Button(BT_AC);
		else
			Release_Button(BT_AC);
		if(Judge_Button(BT_Wave1))//Wave1
			Press_Button(BT_Wave1);
		else
			Release_Button(BT_Wave1);
		if(Judge_Button(BT_Wave2))//Wave2
			Press_Button(BT_Wave2);
		else
			Release_Button(BT_Wave2);
	}

}
