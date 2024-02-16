/**
  *****************************************************************************
  * 						私有定时器应用库
  *****************************************************************************
  *
  * @File   : SCU_TIMER.c
  * @By     : Sun
  * @Version: V0.5
  * @Date   : 2022 / 05 / 30
  * @Shop	: https://xiaomeige.taobao.com/
  *
  *****************************************************************************
**/
#include "SCU_TIMER.h"

XScuTimer ScuTimer;		//私有定时器


/**
  *****************************************************
  * @brief	初始化私有定时器
  * @param	Time_us		定时器中断触发时间，单位为us
  * @usage	ScuTimer_Int_Init(20);	//初始化私有定时器，每20us触发一次
  * @tag	本函数开启私有定时器，并进行周期性中断，要停止私有定时器可使用XScuTimer_Stop(&ScuTimer);
  *****************************************************
**/
void ScuTimer_Int_Init(uint32_t Time_us)
{
	uint32_t Load_Val;
    //定时器初始化
    XScuTimer_Config *Config;
    Config = XScuTimer_LookupConfig(XPAR_XSCUTIMER_0_DEVICE_ID);
    XScuTimer_CfgInitialize(&ScuTimer, Config,Config->BaseAddr);

    //将us转换为定时器装载值
    Load_Val = ((float)CPU_CLK_HZ / 2 / 1000000 * Time_us) - 1;

	//设置自动装载值和自动装载模式
    XScuTimer_LoadTimer(&ScuTimer, Load_Val);
    XScuTimer_EnableAutoReload(&ScuTimer);

    //链接到中断控制器
    Set_ScuGic_Link(XPAR_SCUTIMER_INTR, 0xA8, HIGH_Level_Sensitive,ScuTimer_IRQ_Handler, (void *)&ScuTimer);

    //使能定时器中断
    XScuTimer_EnableInterrupt(&ScuTimer);

    //开启计数器
    XScuTimer_Start(&ScuTimer);
}
