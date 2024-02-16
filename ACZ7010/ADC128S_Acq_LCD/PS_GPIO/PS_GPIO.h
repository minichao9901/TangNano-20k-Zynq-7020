#ifndef __PS_GPIO_H__
#define __PS_GPIO_H__

#include "xgpiops.h"
#include "xparameters.h"
#include "xscugic.h"

#define GPIO_DEVICE_ID  	XPAR_XGPIOPS_0_DEVICE_ID
#define INPUT				0
#define OUTPUT				1

#define KEY_S1 47

//GPIO相关的结构体，使用GPIO时必须添加
extern XGpioPs Gpio;

void PS_GPIO_Init();
void PS_GPIO_SetMode(uint8_t GPIO_Num, uint8_t Dri, uint8_t Data);
void PS_GPIO_SetPort(uint8_t GPIO_Num, uint8_t Data);
uint8_t PS_GPIO_GetPort(uint8_t GPIO_Num);
void PS_GPIO_Int_Init(XScuGic *IntInstPtr);
void PS_GPIO_SetInt(XScuGic *IntInstPtr, uint8_t GPIO_Num,uint8_t IntcMode);



#endif
