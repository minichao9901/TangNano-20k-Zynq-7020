/**
  *****************************************************************************
  * 						PS部分GPIO相关库
  *****************************************************************************
  *
  * @File   : PS_GPIO.c
  * @By     : Sun
  * @Version: V1.2
  * @Date   : 2021 / 11 / 04
  * @Shop	: https://xiaomeige.taobao.com/
  *
  *****************************************************************************
**/


#include "PS_GPIO.h"
#include "stdio.h"
#include "sleep.h"
#include "BMP_WR.h"
//GPIO相关的结构体，使用GPIO时必须添加

XGpioPs Gpio;
volatile uint8_t Cnt = 0;
char Pic_Name[30];
static void PS_GPIO_IntrHandler(void *CallBackRef, uint32_t Bank, uint32_t Status)
{
	uint32_t Key_State;
	XGpioPs *Gpio = (XGpioPs *)CallBackRef;

	//添加判断来消除其它Bank的干扰
	if(Bank == 1) {
		usleep(10000);	//按键消抖10ms
		Key_State = XGpioPs_ReadPin(Gpio, KEY_S1);
		if(Key_State == 0) {
			//截图，存入SD卡
			sprintf(Pic_Name,"Screenshot(%02d).bmp",Cnt);
		    bmp_write(Pic_Name, (char *)&BMODE_800x480, (char *)frame);
		    Cnt++;
		}
	}
}

/***************************************************************************
**  @brief	初始化PS端MIO和EMIO
**	Sample:	PS_GPIO_Init();	//初始化PS端GPIO
*************************************************************************/
void PS_GPIO_Init()
{
	XGpioPs_Config *ConfigPtr;
	ConfigPtr = XGpioPs_LookupConfig(GPIO_DEVICE_ID);
	XGpioPs_CfgInitialize(&Gpio, ConfigPtr, ConfigPtr->BaseAddr);
}

/****************************************************************************
**  @brief	初始化特定GPIO口的模式与状态
**  @param	GPIO_Num	GPIO编号（MIO为0~53，EMIO从54开始）
**  @param	Dir			输入/输出：OUTPUT为输出，INPUT为输入
**  @param	Data		输出电平高低：0为低，1为高（若设置为输入则此处数据无影响）
**  Sample:	GPIO_SetMode(46, OUTPUT, 1);//设置GPIO46为输出模式，输出为高电平
*****************************************************************************/
void PS_GPIO_SetMode(uint8_t GPIO_Num, uint8_t Dri, uint8_t Data)
{
	XGpioPs_SetDirectionPin(&Gpio, GPIO_Num, Dri);		//设置LED输出方向，0为输入，1为输出
	XGpioPs_SetOutputEnablePin(&Gpio, GPIO_Num, Dri);	//输出使能，0为禁用，1为使能
	XGpioPs_WritePin(&Gpio, GPIO_Num, Data);			//输出高/低电平，0为低，1为高
}

/****************************************************************************
**  @brief	控制特定GPIO口的输出高/低电平,使用前必须先用GPIO_SetMode设置该GPIO口为"输出"模式
**  @param	GPIO_Num	GPIO编号（MIO为0~53，EMIO从54开始）
**  @param	Data		输出电平高低：0为低，1为高
**  Sample:	GPIO_SetPort(46, 1);//控制GPIO46输出高电平
*****************************************************************************/
void PS_GPIO_SetPort(uint8_t GPIO_Num, uint8_t Data)
{
	XGpioPs_WritePin(&Gpio, GPIO_Num, Data);	//输出高/低电平，0为低，1为高
}
/*****************************************************************************
**  @brief	读取特定GPIO口的输入电平,使用前必须先用GPIO_SetMode设置该GPIO口为"输入"模式
**  @param	GPIO_Num	GPIO编号（MIO为0~53，EMIO从54开始）
**  @return 返回采集的GPIO电平值，0为低，1为高
**  Sample:	state = GPIO_GetPort(46, state);	//采集GPIO46的电平，存放在state里
*****************************************************************************/
uint8_t PS_GPIO_GetPort(uint8_t GPIO_Num)
{
	return XGpioPs_ReadPin(&Gpio, GPIO_Num);		//输出高/低电平，0为低，1为高
}
/****************************************************************************
**  @brief	初始化PS GPIO的中断功能
**  @param	IntInstPtr	中断实例
**  Sample:	PS_GPIO_Int_Init(&IntInstPtr);//初始化PS GPIO的中断功能
*****************************************************************************/
void PS_GPIO_Int_Init(XScuGic *IntInstPtr)
{
	XScuGic_Config *IntcConfig;
	IntcConfig = XScuGic_LookupConfig(XPAR_SCUGIC_SINGLE_DEVICE_ID);
	XScuGic_CfgInitialize(IntInstPtr, IntcConfig,
					IntcConfig->CpuBaseAddress);

	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
				(Xil_ExceptionHandler)XScuGic_InterruptHandler,IntInstPtr);
	Xil_ExceptionEnableMask(XIL_EXCEPTION_IRQ);

	XScuGic_Connect(IntInstPtr, XPAR_XGPIOPS_0_INTR,
				(Xil_ExceptionHandler)XGpioPs_IntrHandler,(void *)&Gpio);
}
/****************************************************************************
**  @brief	启用GPIO某Pin的中断功能
**  @param	InstPtr
**  @param	GPIO_Num	GPIO编号（MIO为0~53，EMIO从54开始）
**  @param	IntcMode	GPIO中断触发模式
**  					XGPIOPS_IRQ_TYPE_EDGE_RISING	上升沿触发
**  					XGPIOPS_IRQ_TYPE_EDGE_FALLING	下降沿触发
**  					XGPIOPS_IRQ_TYPE_EDGE_BOTH		上升或下降沿都触发
**  					XGPIOPS_IRQ_TYPE_LEVEL_HIGH		高电平触发
**  					XGPIOPS_IRQ_TYPE_LEVEL_LOW		低电平触发
**  Sample:	PS_GPIO_SetInt(&Intc,47,XGPIOPS_IRQ_TYPE_EDGE_FALLING);	//设置MIO47为下降沿中断
*****************************************************************************/
void PS_GPIO_SetInt(XScuGic *IntInstPtr, uint8_t GPIO_Num,uint8_t IntcMode)
{
	//设置引脚中断模式
	XGpioPs_SetIntrTypePin(&Gpio, GPIO_Num, IntcMode);

	//为 gpio中断设置处理程序
	XGpioPs_SetCallbackHandler(&Gpio, (void *)&Gpio, PS_GPIO_IntrHandler);

	//使能Bank的GPIO Pin中断
	XGpioPs_IntrEnablePin(&Gpio, GPIO_Num);

	//启用中断
	XScuGic_Enable(IntInstPtr, XPAR_XGPIOPS_0_INTR);
}
