#include "Timer.h"

//声明各个进程的标志位
extern uint8_t Flag_DrawWave;		//绘制波形，10ms一次
extern uint8_t Flag_DrawGrid;		//绘制网格，20ms一次
extern uint8_t Flag_TouchScan;		//触摸扫描，30ms一次
extern uint8_t Flag_Refresh_Val;	//刷新电压和频率数值，500ms一次
extern uint8_t Flag_Refresh_Time;	//刷新时间，500ms一次

XScuGic Intc;               //中断控制器驱动程序实例
XScuTimer Timer;            //定时器驱动程序实例

//各个时间标志位
static uint8_t Flag_20ms = 0;
static uint8_t Flag_30ms = 0;
static uint8_t Flag_50ms = 0;
static uint8_t Flag_100ms = 0;
static uint8_t Flag_200ms = 0;
static uint8_t Flag_500ms = 0;

//定时器中断初始化函数。仅在定时器初始化函数Timer_Init内部调用，外部无需调用。
void Timer_Intr_Init(XScuGic *Intc_Ptr,XScuTimer *Timer_Ptr);
void touch_button();

/**
  *****************************************************
  * @brief	定时器中断处理程序
  * @tag	本函数用来处理定时器中断，在内部加入用户程序即可
  *****************************************************
**/
void Timer_Intr_Handler(void *CallBackRef)
{
    XScuTimer *Timer_Ptr = (XScuTimer *) CallBackRef;


    /*****下面填入该中断发生后用户想要执行的操作*****/
    /*** 开始↓ ***/
    //以下通过计时器控制每个进程的触发间隔，启动对应的标志位
    Flag_DrawWave = 1;

    if(Flag_20ms >= 1) {
    	Flag_20ms = 0;
    	Flag_DrawGrid = 1;
    }
    else
    	Flag_20ms++;

    if(Flag_30ms >= 2) {
    	Flag_30ms = 0;
    	Flag_TouchScan = 1;
    }
    else
    	Flag_30ms++;

    if(Flag_50ms >= 4) {
    	Flag_50ms = 0;

    }
    else
    	Flag_50ms++;

    if(Flag_100ms >= 9) {
    	Flag_100ms = 0;

    }
    else
    	Flag_100ms++;

    if(Flag_200ms >= 19) {
    	Flag_200ms = 0;

    }
    else
    	Flag_200ms++;

    if(Flag_500ms >= 49) {
    	Flag_500ms = 0;
    	Flag_Refresh_Val = 1;
    	Flag_Refresh_Time = 1;
    }
    else
    	Flag_500ms++;

    /*** 结束↑ ***/


    //清除中断标志
    XScuTimer_ClearInterruptStatus(Timer_Ptr);
}

/**
  *****************************************************
  * @brief	初始化定时器
  * @param	*Intc_Ptr	指向Intc_Ptr实例的指针
  * @param	*Timer_Ptr	指向XScuTimer实例的指针
  * @param	Time_us		定时器中断触发时间，单位为us
  * @usage	Timer_Init(&Intc, &Timer, 2000);	//初始化定时器，每2000us触发一次
  * 		XScuTimer_Start (&Timer ); //启动定时器
  * @tag	本函数仅用来初始化定时器，要启动定时器需要使用XScuTimer_Start函数，停止则用XScuTimer_Stop
  *****************************************************
**/
void Timer_Init(XScuGic *Intc_Ptr,XScuTimer *Timer_Ptr, uint32_t Time_us)
{
    //定时器初始化
    XScuTimer_Config *Timer_Cfg_Ptr;
    Timer_Cfg_Ptr = XScuTimer_LookupConfig(XPAR_XSCUTIMER_0_DEVICE_ID);
    XScuTimer_CfgInitialize(Timer_Ptr, Timer_Cfg_Ptr,Timer_Cfg_Ptr->BaseAddr);
    XScuTimer_LoadTimer(Timer_Ptr, TIMER_LOAD_VALUE(Time_us));	//加载计数周期
    XScuTimer_EnableAutoReload(Timer_Ptr);	//设置自动装载模式
	Timer_Intr_Init(Intc_Ptr,Timer_Ptr);   //定时器中断初始化
}

/**
  *****************************************************
  * @brief	定时器中断初始化函数。
  * @tag	仅在定时器初始化函数Timer_Init内部调用，外部无需调用。
  *****************************************************
**/
void Timer_Intr_Init(XScuGic *Intc_Ptr,XScuTimer *Timer_Ptr)
{
    //初始化中断控制器
    XScuGic_Config *Intc_Cfg_Ptr;
    Intc_Cfg_Ptr = XScuGic_LookupConfig(XPAR_SCUGIC_SINGLE_DEVICE_ID);
    XScuGic_CfgInitialize(Intc_Ptr, Intc_Cfg_Ptr,Intc_Cfg_Ptr->CpuBaseAddress);
    //设置并打开中断异常处理功能
    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
            (Xil_ExceptionHandler)XScuGic_InterruptHandler, Intc_Ptr);
    Xil_ExceptionEnable();
    //设置定时器中断
    XScuGic_Connect(Intc_Ptr, XPAR_SCUTIMER_INTR,
    		(Xil_ExceptionHandler)Timer_Intr_Handler, (void *)Timer_Ptr);
    //使能GIC中的定时器中断
    XScuGic_Enable(Intc_Ptr, XPAR_SCUTIMER_INTR);
    //使能定时器中断
    XScuTimer_EnableInterrupt(Timer_Ptr);
}
