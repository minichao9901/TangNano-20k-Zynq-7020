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

//GPIO相关的结构体，使用GPIO时必须添加

XGpioPs Gpio;

/************************************************************************************************************
**  @brief	初始化PS端MIO和EMIO
**	Sample:	PS_GPIO_Init();	//初始化PS端GPIO
************************************************************************************************************/
void PS_GPIO_Init()
{
	XGpioPs_Config *ConfigPtr;
	ConfigPtr = XGpioPs_LookupConfig(GPIO_DEVICE_ID);
	XGpioPs_CfgInitialize(&Gpio, ConfigPtr, ConfigPtr->BaseAddr);
}

/************************************************************************************************************
**  @brief	初始化特定GPIO口的模式与状态
**  @param	GPIO_Num	GPIO编号（MIO为0~53，EMIO从54开始）
**  @param	Dir			输入/输出：OUTPUT为输出，INPUT为输入
**  @param	Data		输出电平高低：0为低，1为高（若设置为输入则此处数据无影响）
**  Sample:	GPIO_SetMode(46, OUTPUT, 1);//设置GPIO46为输出模式，输出为高电平
************************************************************************************************************/
void GPIO_SetMode(uint8_t GPIO_Num, uint8_t Dri, uint8_t Data)
{
	XGpioPs_SetDirectionPin(&Gpio, GPIO_Num, Dri);		//设置LED输出方向，0为输入，1为输出
	XGpioPs_SetOutputEnablePin(&Gpio, GPIO_Num, Dri);	//输出使能，0为禁用，1为使能
	XGpioPs_WritePin(&Gpio, GPIO_Num, Data);			//输出高/低电平，0为低，1为高
}

/************************************************************************************************************
**  @brief	控制特定GPIO口的输出高/低电平,使用前必须先用GPIO_SetMode设置该GPIO口为"输出"模式
**  @param	GPIO_Num	GPIO编号（MIO为0~53，EMIO从54开始）
**  @param	Data		输出电平高低：0为低，1为高
**  Sample:	GPIO_SetPort(46, 1);//控制GPIO46输出高电平
************************************************************************************************************/
void GPIO_SetPort(uint8_t GPIO_Num, uint8_t Data)
{
	XGpioPs_WritePin(&Gpio, GPIO_Num, Data);	//输出高/低电平，0为低，1为高
}
/************************************************************************************************************
**  @brief	读取特定GPIO口的输入电平,使用前必须先用GPIO_SetMode设置该GPIO口为"输入"模式
**  @param	GPIO_Num	GPIO编号（MIO为0~53，EMIO从54开始）
**  @return 返回采集的GPIO电平值，0为低，1为高
**  Sample:	state = GPIO_GetPort(46, state);	//采集GPIO46的电平，存放在state里
************************************************************************************************************/
uint8_t GPIO_GetPort(uint8_t GPIO_Num)
{
	return XGpioPs_ReadPin(&Gpio, GPIO_Num);		//输出高/低电平，0为低，1为高
}
