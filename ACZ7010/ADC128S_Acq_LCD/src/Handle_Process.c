#include "Handle_Process.h"
#include "Create_Features.h"
#include "xiaomeige.h"
#include "QR_Code.h"

//定义各种数据变量
char str[30];	//缓存字符串
uint16_t *P_ADC_Data = (uint16_t *)ADC_DATA_BASE;//创建一个指针指向ADC数据区域
uint8_t Wave_Run = 1;		//波形采集标志
uint8_t Refresh_Flag = 1;		//波形刷新标志
uint8_t Sample_Set = 4;		//采样率档位，默认为20K档位,0~9共10档
uint8_t Sample_Set_Last = 0;//记录上一次采样率档位
uint8_t Trigger_Mode = 0;	//0表示自动触发，1表示手动触发，2表示单次触发
uint16_t Trigger_Value = 0;	//采样触发值
uint16_t Trigger_Value_Last = 0;//上一次采样触发值
//采样率设置（KHz）ADC128S102最大为1000KHz
uint32_t Sample_Rate[10] = {1,2,5,10,20,50,100,200,500,1000};
uint8_t CH_Set = 0;	//采样通道设置
uint16_t Measure_Point,Measure_Point_Last;
uint16_t Trigger_Point,Trigger_Point_Last;
uint16_t Wave_Data[ADC_DATA_LENGTH] = {0};//存储当前波形数据
uint16_t Pre_Wave_Data[ADC_DATA_LENGTH] = {0};//存储上一次波形数据
float Point_Voltage;	//标点的电压值
float Point_Time;		//标点的时间
uint8_t Single_TriggerFlag = 0;	//触发标志
uint8_t Cancel_Trigger = 0;	//取消触发
uint32_t ADC_Wave_Offset = 0;//波形数据地址偏移值
uint16_t Slider_Val = 0;
Time RTC_Time;

//定义各个进程的标志位
uint8_t Flag_DrawWave = 0;		//绘制波形，10ms一次
uint8_t Flag_DrawGrid = 0;		//绘制网格，20ms一次
uint8_t Flag_TouchScan = 0;		//触摸扫描，30ms一次
uint8_t Flag_Refresh_Val = 0;	//刷新电压和频率数值，500ms一次
uint8_t Flag_Refresh_Time = 0;	//刷新时间，500ms一次

//初始化主页
void Init_Homepage()
{
	FRESULT rc;

	LCD_Init();							//LCD初始化
	Timer_Init(&Intc,&Timer,10000);		//定时器初始化，每10ms触发一次
	GT9147_Init();						//GT9147初始化
	PS_GPIO_SetMode(KEY_S1, INPUT, 0);	//初始化按键S1
	PS_GPIO_Int_Init(&Intc);			//初始化PS GPIO中断
	//设置按键S1为下降沿中断
	PS_GPIO_SetInt(&Intc,KEY_S1,XGPIOPS_IRQ_TYPE_EDGE_FALLING);

    rc = f_mount(&fatfs, "0:/", 0);		//挂载SD卡
    if (rc != FR_OK) {
    	printf("Mount Failed!\n");		//若挂载失败则发出警告
    }

	LCD_Clear(LCD_BLACK);	//设置整体背景为黑色
	POINT_COLOR = LCD_WHITE;//笔画设定为白色
	BACK_COLOR = LCD_BLACK;	//笔画背景设定为黑色

	//LCD_ShowString(3,20,180,16,16,0,(uint8_t*)"www.corecourse.cn");//显示网址
	LCD_DisplayPic(20,60,gImage_xiaomeige1);//画LOGO

	Draw_Box(ADC_Wave.Window, LCD_WHITE, 0);
	Get_Waveform_Window_Parameters(&ADC_Wave);
	Draw_Waveform_Windows(ADC_Wave, Sample_Rate[Sample_Set]);//KHz

	Draw_Normal_Button(Button_RUN);
	Draw_Normal_Button(Button_AUTO);
	Draw_Normal_Button(Button_CH_INC);
	Draw_Normal_Button(Button_CH_DEC);
	Draw_Normal_Button(Button_SA_INC);
	Draw_Normal_Button(Button_SA_DEC);
	Draw_Normal_Button(Button_Tri_Mode);
	Draw_Normal_Button(Wave_Slider);
	Draw_Normal_Button(Button_QR_Code);//显示help按键
	Fill_Box(Slider,LCD_BLUE,5);

	sprintf(str,"%.3fV",Get_ADC_Vmid());
	strcpy(Button_TriggerVal.Text[0],str);
	Draw_Normal_Button(Button_TriggerVal);

	sprintf(str,"CH%d",CH_Set);
	Draw_Normal_Text(Text_CHANNEL, str);
	sprintf(str,"%dKHz",(int)Sample_Rate[Sample_Set]);
	Draw_Normal_Text(Text_SAMPLE, str);

	//先把ADC数据所处的内存清零
	memset(P_ADC_Data,0,ADC_DATA_LENGTH*2);

	//先采集一次
	Set_ADC_Mode(CH_Set, Sample_Rate[Sample_Set], Trigger_Value, Trigger_Mode);
	XScuTimer_Start(&Timer);         	//启动定时器
	Measure_Point = ADC_Wave.Wave_Area.X1 + ADC_Wave.Wave_Area.Width/2;		//测量点为Y轴线
	Trigger_Point = ADC_Wave.Wave_Area.Y1 + ADC_Wave.Wave_Area.Height/2;	//触发点为X轴线

}
void Refresh_Measure_Val()
{
	sprintf(str,"Vmin = %.3fV",Get_ADC_Vmin());
	Draw_Normal_Text(Text_Vmin, str);
	sprintf(str,"Vmax = %.3fV",Get_ADC_Vmax());
	Draw_Normal_Text(Text_Vmax, str);
	sprintf(str,"Vpp = %.3fV",Get_ADC_Vmax()-Get_ADC_Vmin());
	Draw_Normal_Text(Text_Vpp, str);
	sprintf(str,"Freq = %luHz",Get_ADC_Freq());
	Draw_Normal_Text(Text_Freq, str);

	if(Trigger_Mode == 0) {	//如果是Auto_Trigger则刷新电压中值
		sprintf(str,"%.3fV",Get_ADC_Vmid());
		strcpy(Button_TriggerVal.Text[0],str);
		Draw_Normal_Button(Button_TriggerVal);
	}
}
void Refresh_Time()
{
	Get_Time(&RTC_Time);	//获取当前时间
	sprintf(str,"%02u:%02u:%02u",RTC_Time.hour,RTC_Time.minute,RTC_Time.second);
	Draw_Normal_Text(Text_Clock, str);
	sprintf(str,"%02u-%02u-%02u",RTC_Time.year,RTC_Time.month,RTC_Time.day);
	Draw_Normal_Text(Text_Date, str);
	LCD_Refresh();//刷新图像
}
void Touch_Scan()
{
	uint8_t i;
	Time Time;
	uint32_t Freq;	//频率
	uint8_t Wave_Density = 4;	//按下Auto后的页面最优波形密度
	GT9147_Scan(&Touch_LCD);	//触摸扫描
	if(Touch_LCD.Touched) {		//如果按键按下
		if(!Refresh_Flag) {		//只要触摸，就擦除二维码，启动波形刷新
			LCD_Color_Fill(300,140,500,340,LCD_BLACK);
			Refresh_Flag = 1;
		}

		if(Judge_TpXY(Touch_LCD,Button_QR_Code.Box)) {	//help按键
			Draw_Button_Effect(Button_QR_Code);
		} else {
			Draw_Normal_Button(Button_QR_Code);
		}

		if(Judge_TpXY(Touch_LCD,Button_AUTO.Box)) {		//AUTO按键
			Draw_Button_Effect(Button_AUTO);
		} else {
			Button_AUTO.BackColor = LCD_GREEN;
			Draw_Normal_Button(Button_AUTO);
		}

		if(Judge_TpXY(Touch_LCD,Button_AUTO.Box)) {		//AUTO按键
			Draw_Button_Effect(Button_AUTO);
		} else {
			Button_AUTO.BackColor = LCD_GREEN;
			Draw_Normal_Button(Button_AUTO);
		}

		if(Wave_Run && (!Single_TriggerFlag)) {
			if(Judge_TpXY(Touch_LCD,Button_CH_INC.Box)) {	//CH_INC按键
				Draw_Button_Effect(Button_CH_INC);
			} else {
				Draw_Normal_Button(Button_CH_INC);
			}

			if(Judge_TpXY(Touch_LCD,Button_CH_DEC.Box)) {	//CH_DEC按键
				Draw_Button_Effect(Button_CH_DEC);
			} else {
				Draw_Normal_Button(Button_CH_DEC);
			}

			if(Judge_TpXY(Touch_LCD,Button_SA_INC.Box)) {	//SA_INC按键
				Draw_Button_Effect(Button_SA_INC);
			} else {
				Draw_Normal_Button(Button_SA_INC);
			}

			if(Judge_TpXY(Touch_LCD,Button_SA_DEC.Box)) {	//SA_DEC按键
				Draw_Button_Effect(Button_SA_DEC);
			} else {
				Draw_Normal_Button(Button_SA_DEC);
			}

			if(Judge_TpXY(Touch_LCD,Button_Tri_Mode.Box)) {	//Tri_Mode按键
				Draw_Button_Effect(Button_Tri_Mode);
			} else {
				Draw_Normal_Button(Button_Tri_Mode);
			}

			if((Trigger_Mode==0x03)&&Judge_TpXY(Touch_LCD,Button_TriggerVal.Box)) {//TriggerVal按键
				Draw_Button_Effect(Button_TriggerVal);
			} else {

			}
		}

		if(Judge_TpXY(Touch_LCD,Wave_Slider.Box)) { //滑动波形的滑块
			if(Touch_LCD.Tp_X[0] <= Wave_Slider.Box.X1 + 20)
				Slider_Val = 0;
			else if(Touch_LCD.Tp_X[0] >= Wave_Slider.Box.X1 + Wave_Slider.Box.Width - 20)
				Slider_Val = 200;
			else {
				Slider_Val = Touch_LCD.Tp_X[0] - Wave_Slider.Box.X1 - 20;
			}
			Fill_Box(Slider,Wave_Slider.BackColor,5);
			Slider.X1 = Slider_Val + Wave_Slider.Box.X1;
			Fill_Box(Slider,LCD_BLUE,5);
			ADC_Wave_Offset = Slider_Val*2.5;
		}


		if(Judge_TpXY(Touch_LCD,ADC_Wave.Window) && (!Single_TriggerFlag)) {	//波形窗口
			if((1)&&(Wave_Run)&&(!Trigger_Mode)&&
					(Judge_Two_TpXY(Touch_LCD,ADC_Wave.Wave_Area)))//两指触摸
			{
				Sample_Set_Last = Sample_Set;
				if((Touch_LCD.Tp_X[1] > Touch_LCD.Tp_X[0]))
					Sample_Set = (Touch_LCD.Tp_X[1]-Touch_LCD.Tp_X[0])/40;
				else
					Sample_Set = (Touch_LCD.Tp_X[0]-Touch_LCD.Tp_X[1])/40;
				if(Sample_Set >= 9)
					Sample_Set = 9;


				//如果采样率改变，则修改采样参数
				if(Sample_Set_Last != Sample_Set)
					Set_ADC_Mode(CH_Set, Sample_Rate[Sample_Set], Trigger_Value, Trigger_Mode);
				sprintf(str,"%dKHz",(int)Sample_Rate[Sample_Set]);
				Draw_Normal_Text(Text_SAMPLE, str);
				Display_Time_Scale(ADC_Wave,Sample_Rate[Sample_Set]);//刷新X轴的时间刻度
			}
			if(Wave_Run == 0) {
				//出现竖标线，可拖动显示每一点的电压和时间
				Measure_Point_Last = Measure_Point;
				if(Touch_LCD.Tp_X[0] <= ADC_Wave.Wave_Area.X1)
					Measure_Point = ADC_Wave.Wave_Area.X1;
				else if(Touch_LCD.Tp_X[0] >= ADC_Wave.Wave_Area.X1 + ADC_Wave.Wave_Area.Width)
					Measure_Point = ADC_Wave.Wave_Area.X1 + ADC_Wave.Wave_Area.Width;
				else if(Touch_LCD.Tp_X[0] > ADC_Wave.Wave_Area.X1 &&
						Touch_LCD.Tp_X[0] < ADC_Wave.Wave_Area.X1 + ADC_Wave.Wave_Area.Width)
					Measure_Point = Touch_LCD.Tp_X[0]/5*5;	//将测量点的X坐标变为5的倍数值

				//擦掉上一次的标线
				if(Measure_Point_Last != Measure_Point)
				Draw_Mark_Line(1,Measure_Point_Last, ADC_Wave.Wave_Area.Y1,
						ADC_Wave.Wave_Area.Y1 + ADC_Wave.Wave_Area.Height,PAGE_COLOR);

			} else if(Trigger_Mode != 0){
				//绘制触发电压的光标
				Trigger_Point_Last = Trigger_Point;//记录上一个标线位置
				if(Touch_LCD.Tp_Y[0] <= ADC_Wave.Wave_Area.Y1)
					Trigger_Point = ADC_Wave.Wave_Area.Y1;
				else if(Touch_LCD.Tp_Y[0] >= ADC_Wave.Wave_Area.Y1 + ADC_Wave.Wave_Area.Height)
					Trigger_Point = ADC_Wave.Wave_Area.Y1 + ADC_Wave.Wave_Area.Height;
				else if(Touch_LCD.Tp_Y[0] > ADC_Wave.Wave_Area.Y1 &&
						Touch_LCD.Tp_Y[0] < ADC_Wave.Wave_Area.Y1 + ADC_Wave.Wave_Area.Height)
					Trigger_Point = Touch_LCD.Tp_Y[0]/1*1;	//将触发点的Y坐标变为2的倍数值

				//擦旧标线
				if(Trigger_Point_Last != Trigger_Point)
				Draw_Mark_Line(0,Trigger_Point_Last, ADC_Wave.Wave_Area.X1,
						ADC_Wave.Wave_Area.X1 + ADC_Wave.Wave_Area.Width,PAGE_COLOR);

				Trigger_Value_Last = Trigger_Value;
				//计算触发值
				Trigger_Value = ((ADC_Wave.Wave_Area.Y1 + ADC_Wave.Wave_Area.Height/2
						- Trigger_Point)/ADC_Wave.Precision);

				//如果触发值被修改，则重配置采样参数
				if(Trigger_Value_Last != Trigger_Value)
					Set_ADC_Mode(CH_Set, Sample_Rate[Sample_Set], Trigger_Value, Trigger_Mode);
				//刷新触发电压显示
				sprintf(str,"%.3fV",Trigger_Value*3.3/4096);
				strcpy(Button_TriggerVal.Text[0],str);
				Draw_Normal_Button(Button_TriggerVal);

			}
		} else {

		}

	} else {
		if(Touch_LCD.Touched_Last) {	//如果本次未按但是上次按了，表示松手
			if(Judge_TpXY(Touch_LCD,Button_QR_Code.Box)) {	//help按键
				if(Refresh_Flag) {
					Refresh_Flag = 0;
					LCD_DisplayPic(300,140,QR_Code);	//显示论坛二维码
				}
				Draw_Normal_Button(Button_QR_Code);
				LCD_Refresh();//刷新图像
			}

			if(Judge_TpXY(Touch_LCD,Text_Clock.Box) ||
					Judge_TpXY(Touch_LCD,Text_Date.Box))	//使用串口设置时间
			{
				printf("Please input Date :\n");
				scanf("%d %d %d",&Time.year,&Time.month,&Time.day);
				printf("%02d-%02d-%02d\n",Time.year,Time.month,Time.day);
				Set_Date(Time.year%100,Time.month,Time.day,0);	//设置日期
				printf("Please input Clock :\n");
				scanf("%d %d %d",&Time.hour,&Time.minute,&Time.second);
				printf("%02d:%02d:%02d\n",Time.hour,Time.minute,Time.second);
				Set_Clock(Time.hour,Time.minute,Time.second);	//设置时间
			}


			if(Judge_TpXY(Touch_LCD,Button_RUN.Box) && (!Single_TriggerFlag)) {		//RUN按键
				if(Wave_Run) {
					//进入STOP模式
					Perform_STOP();
				} else {
					Wave_Run = 1;
					Button_RUN.BackColor = LCD_GREEN;
					Button_RUN.TextColor = LCD_BLACK;
					strcpy(Button_RUN.Text[0],"RUN");
					//擦掉当前的标线
					Draw_Mark_Line(1,Measure_Point, ADC_Wave.Wave_Area.Y1,
							ADC_Wave.Wave_Area.Y1 + ADC_Wave.Wave_Area.Height,PAGE_COLOR);
					//擦掉文本
					Fill_Box(Text_Point_V.Box,Text_Point_V.BackColor,0);
					Fill_Box(Text_Point_T.Box,Text_Point_T.BackColor,0);

					STOP_Ensable_Press();//解除在STOP状态下被冻结的操作
				}
				Draw_Normal_Button(Button_RUN);
			}
			if(Judge_TpXY(Touch_LCD,Button_AUTO.Box)) {		//AUTO按键
				Set_ADC_Mode(CH_Set, Sample_Rate[9], 0, 0);//开启最大采样率并采集一次
				if(Single_TriggerFlag)
					Cancel_Trigger = 1;
				Button_AUTO.BackColor = LCD_RED;
				Draw_Button_Effect(Button_AUTO);
				sleep(1);
				Button_AUTO.BackColor = LCD_GREEN;
				Draw_Normal_Button(Button_AUTO);
				Freq = Get_ADC_Freq();
				if(Freq/(2*Sample_Rate[0]) < Wave_Density)
					Sample_Set = 0;
				else if(Freq/(2*Sample_Rate[9]) > Wave_Density)
					Sample_Set = 9;
				else {
					for(i=0;i<9;i++) {
						if((Freq/(2*Sample_Rate[i]) >= Wave_Density) &&
								(Freq/(2*Sample_Rate[i+1]) <= Wave_Density)) {
							if(((float)Freq/(2*Sample_Rate[i]) - Wave_Density) <=
									(Wave_Density - (float)Freq/(2*Sample_Rate[i+1])))
								Sample_Set = i;
							else
								Sample_Set = i + 1;
						}
					}
				}
				sprintf(str,"%dKHz",(int)Sample_Rate[Sample_Set]);
				Draw_Normal_Text(Text_SAMPLE, str);
				Display_Time_Scale(ADC_Wave,Sample_Rate[Sample_Set]);	//刷新X轴的时间刻度
				//启动RUN
				Wave_Run = 1;
				Button_RUN.BackColor = LCD_GREEN;
				Button_RUN.TextColor = LCD_BLACK;
				strcpy(Button_RUN.Text[0],"RUN");
				Draw_Normal_Button(Button_RUN);
				//擦掉竖标线
				Draw_Mark_Line(1,Measure_Point, ADC_Wave.Wave_Area.Y1,
						ADC_Wave.Wave_Area.Y1 + ADC_Wave.Wave_Area.Height,PAGE_COLOR);
				//擦除横标线
				Draw_Mark_Line(0,Trigger_Point, ADC_Wave.Wave_Area.X1,
						ADC_Wave.Wave_Area.X1 + ADC_Wave.Wave_Area.Width,PAGE_COLOR);
				//擦掉文本
				Fill_Box(Text_Point_V.Box,Text_Point_V.BackColor,0);
				Fill_Box(Text_Point_T.Box,Text_Point_T.BackColor,0);

				STOP_Ensable_Press();//解除在STOP状态下被冻结的操作

				//切换为自动触发模式
				Trigger_Mode = 0;
				strcpy(Button_Tri_Mode.Text[0],"Auto");
				Draw_Normal_Button(Button_Tri_Mode);
				Button_TriggerVal.TextColor = LCD_WHITE;
				Button_TriggerVal.BackColor = LCD_GRAY;
				Draw_Normal_Button(Button_TriggerVal);
			}

			if(Wave_Run) {
				if(!Single_TriggerFlag) {
					if(Judge_TpXY(Touch_LCD,Button_CH_INC.Box)) {	//CH_INC按键
						Draw_Normal_Button(Button_CH_INC);
						if(CH_Set < 7)
							CH_Set++;
						else
							CH_Set = 0;
						//修改采样参数
						Set_ADC_Mode(CH_Set, Sample_Rate[Sample_Set], Trigger_Value, Trigger_Mode);
						sprintf(str,"CH%d",CH_Set);
						Draw_Normal_Text(Text_CHANNEL, str);	//更新通道显示
					}
					if(Judge_TpXY(Touch_LCD,Button_CH_DEC.Box)) {	//CH_DEC按键
						Draw_Normal_Button(Button_CH_DEC);
						if(CH_Set > 0)
							CH_Set--;
						else
							CH_Set = 7;
						//修改采样参数
						Set_ADC_Mode(CH_Set, Sample_Rate[Sample_Set], Trigger_Value, Trigger_Mode);
						sprintf(str,"CH%d",CH_Set);
						Draw_Normal_Text(Text_CHANNEL, str);	//更新通道显示
					}
					if(Judge_TpXY(Touch_LCD,Button_SA_INC.Box)) {	//SA_INC按键
						Draw_Normal_Button(Button_SA_INC);
						if(Sample_Set < 9)
							Sample_Set++;
						//修改采样参数
						Set_ADC_Mode(CH_Set, Sample_Rate[Sample_Set], Trigger_Value, Trigger_Mode);
						sprintf(str,"%dKHz",(int)Sample_Rate[Sample_Set]);
						Draw_Normal_Text(Text_SAMPLE, str);
						Display_Time_Scale(ADC_Wave,Sample_Rate[Sample_Set]);//刷新X轴的时间刻度
					}
					if(Judge_TpXY(Touch_LCD,Button_SA_DEC.Box)) {	//SA_DEC按键
						Draw_Normal_Button(Button_SA_DEC);
						if(Sample_Set > 0)
							Sample_Set--;
						//修改采样参数
						Set_ADC_Mode(CH_Set, Sample_Rate[Sample_Set], Trigger_Value, Trigger_Mode);
						sprintf(str,"%dKHz",(int)Sample_Rate[Sample_Set]);
						Draw_Normal_Text(Text_SAMPLE, str);
						Display_Time_Scale(ADC_Wave,Sample_Rate[Sample_Set]);//刷新X轴的时间刻度
					}
					if(Judge_TpXY(Touch_LCD,Button_Tri_Mode.Box)) {	//Tri_Mode按键
						if(Trigger_Mode == 0x00) {
							Trigger_Mode = 0x01;	//0x01为手动触发
							strcpy(Button_Tri_Mode.Text[0],"Normal");
							Button_TriggerVal.TextColor = LCD_BLACK;
							Button_TriggerVal.BackColor = LCD_ORANGE;
						} else if(Trigger_Mode == 0x01) {
							Trigger_Mode = 0x03;	//0x03为单次触发
							strcpy(Button_Tri_Mode.Text[0],"Single");
							Button_TriggerVal.TextColor = LCD_BLACK;
							Button_TriggerVal.BackColor = LCD_GREEN;
						} else {
							Trigger_Mode = 0x00;	//0x00为自动触发
							strcpy(Button_Tri_Mode.Text[0],"Auto");
							Button_TriggerVal.TextColor = LCD_WHITE;
							Button_TriggerVal.BackColor = LCD_GRAY;
							//清除触发电压的光标
							Draw_Mark_Line(0,Trigger_Point, ADC_Wave.Wave_Area.X1,
									ADC_Wave.Wave_Area.X1 + ADC_Wave.Wave_Area.Width,PAGE_COLOR);
						}
						//修改采样参数
						Set_ADC_Mode(CH_Set, Sample_Rate[Sample_Set], Trigger_Value, Trigger_Mode);
						Draw_Normal_Button(Button_Tri_Mode);
						sprintf(str,"%.3fV",Trigger_Value*3.3/4096);
						strcpy(Button_TriggerVal.Text[0],str);
						Draw_Normal_Button(Button_TriggerVal);
					}
				}
				if((Trigger_Mode==0x03)&&Judge_TpXY(Touch_LCD,Button_TriggerVal.Box)) {//TriggerVal按键
					//修改采样参数
					Set_ADC_Mode(CH_Set, Sample_Rate[Sample_Set], Trigger_Value, Trigger_Mode);
					if(Single_TriggerFlag)
						Cancel_Trigger = 1;
					Single_TriggerFlag = 1;

					Button_TriggerVal.TextColor = LCD_WHITE;
					Button_TriggerVal.BackColor = LCD_RED;
					Draw_Normal_Button(Button_TriggerVal);
					Trigger_Disable_Press();
				}
			}
		}
	}
}

uint8_t Judge_TpXY(Touch_Data Touch_LCD, Box_XY Box)
{
	if((Touch_LCD.Tp_X[0] >= Box.X1)&&(Touch_LCD.Tp_X[0] <= Box.X1 + Box.Width)
			&&(Touch_LCD.Tp_Y[0] >= Box.Y1)&&(Touch_LCD.Tp_Y[0] <= Box.Y1 + Box.Height))
		return 1;
	else
		return 0;
}

uint8_t Judge_Two_TpXY(Touch_Data Touch_LCD, Box_XY Box)
{
	if(Touch_LCD.Touch_Num == 2) {
		if((Touch_LCD.Tp_X[0] >= Box.X1)&&(Touch_LCD.Tp_X[0] <= Box.X1 + Box.Width)
				&&(Touch_LCD.Tp_Y[0] >= Box.Y1)&&(Touch_LCD.Tp_Y[0] <= Box.Y1 + Box.Height)) {
			if((Touch_LCD.Tp_X[1] >= Box.X1)&&(Touch_LCD.Tp_X[1] <= Box.X1 + Box.Width)
						&&(Touch_LCD.Tp_Y[1] >= Box.Y1)&&(Touch_LCD.Tp_Y[1] <= Box.Y1 + Box.Height)) {
				return 1;
			} else
				return 0;
		} else
			return 0;
	}
	else
		return 0;
}

//禁止在STOP状态执行的一些操作
void STOP_Disable_Press()
{
	Button_CH_INC.BackColor = LCD_GRAY;
	Button_CH_DEC.BackColor = LCD_GRAY;
	Button_SA_INC.BackColor = LCD_GRAY;
	Button_SA_DEC.BackColor = LCD_GRAY;
	Button_Tri_Mode.BackColor = LCD_GRAY;
	Button_TriggerVal.BackColor = LCD_GRAY;
	Draw_Normal_Button(Button_CH_INC);
	Draw_Normal_Button(Button_CH_DEC);
	Draw_Normal_Button(Button_SA_INC);
	Draw_Normal_Button(Button_SA_DEC);
	Draw_Normal_Button(Button_Tri_Mode);
	Draw_Normal_Button(Button_TriggerVal);
}

//解除在STOP状态下被冻结的操作
void STOP_Ensable_Press()
{
	Button_CH_INC.BackColor = LCD_BLACK;
	Button_CH_DEC.BackColor = LCD_BLACK;
	Button_SA_INC.BackColor = LCD_BLACK;
	Button_SA_DEC.BackColor = LCD_BLACK;
	Button_Tri_Mode.BackColor = LCD_BLACK;
	if(Trigger_Mode == 0)
		Button_TriggerVal.BackColor = LCD_GRAY;
	else if(Trigger_Mode == 1)
		Button_TriggerVal.BackColor = LCD_ORANGE;
	else
		Button_TriggerVal.BackColor = LCD_GREEN;
	Draw_Normal_Button(Button_CH_INC);
	Draw_Normal_Button(Button_CH_DEC);
	Draw_Normal_Button(Button_SA_INC);
	Draw_Normal_Button(Button_SA_DEC);
	Draw_Normal_Button(Button_Tri_Mode);
	Draw_Normal_Button(Button_TriggerVal);
}

//执行STOP操作
void Perform_STOP()
{
	Wave_Run = 0;

	//按键变为STOP模式
	Button_RUN.BackColor = LCD_RED;
	Button_RUN.TextColor = LCD_WHITE;
	strcpy(Button_RUN.Text[0],"STOP");
	Draw_Normal_Button(Button_RUN);

	//擦横标线
	Draw_Mark_Line(0,Trigger_Point, ADC_Wave.Wave_Area.X1,
			ADC_Wave.Wave_Area.X1 + ADC_Wave.Wave_Area.Width,PAGE_COLOR);

	//使STOP禁止的操作按钮为灰色不可触
	STOP_Disable_Press();
}

//从DDR中读取波形数据
void Read_Wave_Data()
{
	memcpy(Pre_Wave_Data,Wave_Data,ADC_DATA_LENGTH*2);	//保存上次的波形值
	memcpy(Wave_Data,P_ADC_Data + ADC_Wave_Offset,
			(ADC_DATA_LENGTH - ADC_Wave_Offset)*2);//从DDR读取需要的波形值
}

//使单次触发时禁止的操作按钮为灰色不可触
void Trigger_Disable_Press()
{
	Button_RUN.BackColor = LCD_GRAY;
	Button_CH_INC.BackColor = LCD_GRAY;
	Button_CH_DEC.BackColor = LCD_GRAY;
	Button_SA_INC.BackColor = LCD_GRAY;
	Button_SA_DEC.BackColor = LCD_GRAY;
	Button_Tri_Mode.BackColor = LCD_GRAY;

	Draw_Normal_Button(Button_RUN);
	Draw_Normal_Button(Button_CH_INC);
	Draw_Normal_Button(Button_CH_DEC);
	Draw_Normal_Button(Button_SA_INC);
	Draw_Normal_Button(Button_SA_DEC);
	Draw_Normal_Button(Button_Tri_Mode);
}

//恢复单次触发禁止的操作按钮
void Trigger_Enable_Press()
{
	Button_RUN.BackColor = LCD_GREEN;
	Button_CH_INC.BackColor = LCD_BLACK;
	Button_CH_DEC.BackColor = LCD_BLACK;
	Button_SA_INC.BackColor = LCD_BLACK;
	Button_SA_DEC.BackColor = LCD_BLACK;
	Button_Tri_Mode.BackColor = LCD_BLACK;


	Draw_Normal_Button(Button_RUN);
	Draw_Normal_Button(Button_CH_INC);
	Draw_Normal_Button(Button_CH_DEC);
	Draw_Normal_Button(Button_SA_INC);
	Draw_Normal_Button(Button_SA_DEC);
	Draw_Normal_Button(Button_Tri_Mode);
}
void Refresh_WaveWindow()
{
	Read_Wave_Data();//读波形数据
	Draw_Waveform(ADC_Wave,Pre_Wave_Data,Wave_Data);	//画波形
	if(Wave_Run) {
		if(Trigger_Mode)
			//画横标线
			Draw_Mark_Line(0,Trigger_Point, ADC_Wave.Wave_Area.X1,
					ADC_Wave.Wave_Area.X1 + ADC_Wave.Wave_Area.Width,LCD_ORANGE);
	} else if(!Wave_Run) {
		//画竖标线
		Draw_Mark_Line(1,Measure_Point, ADC_Wave.Wave_Area.Y1,
				ADC_Wave.Wave_Area.Y1 + ADC_Wave.Wave_Area.Height,LCD_CYAN);

		//刷新电压显示
		Point_Voltage = Wave_Data[Measure_Point - ADC_Wave.Wave_Area.X1]*3.3/4096;
		if(Point_Voltage >= 0)
			sprintf(str,"Voltage = +%.3fV",Point_Voltage);
		else
			sprintf(str,"Voltage = %.3fV",Point_Voltage);
		Draw_Normal_Text(Text_Point_V, str);

		//刷新时间显示
		Point_Time = (float)((Measure_Point - ADC_Wave.Wave_Area.X1)-250)/Sample_Rate[Sample_Set];
		if(Point_Time >= 0)
			sprintf(str,"Time = +%.3fms",Point_Time);
		else
			sprintf(str,"Time = %.3fms",Point_Time);
		Draw_Normal_Text(Text_Point_T, str);
	}
}

void Handle_Single_Trigger()
{
	Xil_DCacheFlushRange((UINTPTR)P_ADC_Data,ADC_DATA_LENGTH*2);//刷新DDR里的ADC数据

	//按钮恢复原色
	if(Trigger_Mode == 0x03) {
		Button_TriggerVal.TextColor = LCD_BLACK;
		Button_TriggerVal.BackColor = LCD_GREEN;
	} else {
		Button_TriggerVal.TextColor = LCD_WHITE;
		Button_TriggerVal.BackColor = LCD_GRAY;
	}
	Draw_Normal_Button(Button_TriggerVal);

	Trigger_Enable_Press();

	if(!Cancel_Trigger)//触发取消则不再STOP
		Perform_STOP();//执行STOP操作
	Cancel_Trigger = 0;		//清标志
}
void Handle_Round_Done()
{
	Xil_DCacheFlushRange((UINTPTR)P_ADC_Data,ADC_DATA_LENGTH*2);//刷新DDR里的ADC数据
	//开启下一次传输
	Set_ADC_Mode(CH_Set, Sample_Rate[Sample_Set], Trigger_Value, Trigger_Mode);
}
