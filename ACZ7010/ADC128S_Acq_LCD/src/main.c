/**
  *****************************************************************************
  * 			基于ADC128S102制作一个简易示波器，采样率最高为1MHz
  *****************************************************************************
  *
  * @File   : main.c
  * @By     : 小梅哥团队
  * @Version: V1.2
  * @Date   : 2021 / 12 / 17
  * @Shop	: https://xiaomeige.taobao.com/
  * @Forum  : http://www.corecourse.cn/
  *****************************************************************************
**/

#include "Handle_Process.h"

//主函数
int main(void)
{
	Init_Homepage();//初始化主页和控制器

	//循环处理事件
	while(1) {
		//如果开启了单次触发，触发成功则STOP，取消触发则恢复原状
		if(Single_TriggerFlag && (ADC_ROUND_DONE || Cancel_Trigger)) {
			Single_TriggerFlag = 0;	//清标志
			Handle_Single_Trigger();//处理单次触发事件
		}
		//每轮传输完成则刷新Flash并开启下一轮传输
		if(ADC_ROUND_DONE && Wave_Run && (!Single_TriggerFlag)) {
			Handle_Round_Done();
		}
		//定时刷新波形窗口，10ms一次
		if(Flag_DrawWave && Refresh_Flag) {
			Flag_DrawWave = 0;
			Refresh_WaveWindow();	//刷新波形窗口
		}
		//定时检测触摸，30ms一次
		if(Flag_TouchScan) {
			Flag_TouchScan = 0;
			Touch_Scan();					//触摸扫描
		}
		//定时刷网格背景，20ms一次
		if(Flag_DrawGrid && Refresh_Flag) {
			Flag_DrawGrid = 0;
			Draw_Grid_Background(ADC_Wave);	//绘制网格背景
		}
		//定时刷测量值，500ms一次
		if(Flag_Refresh_Val && Wave_Run) {
			Flag_Refresh_Val = 0;
			Refresh_Measure_Val();			//刷新测量值
		}
		//当前时间刷新，500ms一次
		if(Flag_Refresh_Time) {
			Flag_Refresh_Time = 0;
			Refresh_Time();			//刷新RTC时间
		}
	}
	return 0;
}

