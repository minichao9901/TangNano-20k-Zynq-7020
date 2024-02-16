/**
  *****************************************************************************
  * 						AXI_GPIO相关库（基于ACZ702开发板）
  *****************************************************************************
  *
  * @File   : AXI_GPIO.c
  * @By     : Sun
  * @Version: V1.3
  * @Date   : 2021 / 12 / 23
  * @Shop	: https://xiaomeige.taobao.com/
  *
  *****************************************************************************
**/

#include "AXI_GPIO.h"

//AXI GPIO对象
XGpio AXI_GPIO0;

/*****************************************************************************
**  @brief	初始化AXI_GPIO
**  @param	InstPtr		GPIO实例指针
**  @param	DeviceId	GPIO设备ID
**	Sample:	AXI_GPIO_Init(&AXI_GPIO0,GPIO_0_ID);	//初始化AXI GPIO0
*****************************************************************************/
void AXI_GPIO_Init(XGpio *InstPtr, uint16_t DeviceId)
{
	XGpio_Initialize(InstPtr, DeviceId);
}

/**
  *****************************************************
  * @brief	初始化AXI_GPIO中断
  * @param	InstPtr		AXI_GPIO对象指针
  * @param	IntrId		中断ID
  * @param	Mask		通道掩码
  * 					XGPIO_IR_CH1_MASK -> 通道1
  * 					XGPIO_IR_CH2_MASK -> 通道2
  * 					XGPIO_IR_MASK -> 通道1和通道2
  * @param	Handler		中断服务函数
  * @usage	//AXI_GPIO中断初始化,只开启通道1的中断，中断服务函数为AXI_GPIO_IRQ_Handler
  * 		AXI_GPIO_Intc_Init(&AXI_GPIO0,XPAR_FABRIC_AXI_GPIO_0_IP2INTC_IRPT_INTR,
  * 				XGPIO_IR_CH1_MASK,AXI_GPIO_IRQ_Handler);
  *****************************************************
**/
void AXI_GPIO_Intc_Init(XGpio *InstPtr,uint16_t IntrId, uint32_t Mask,
		Xil_InterruptHandler Handler)
{
	Set_ScuGic_Link(IntrId, 0xA0, HIGH_Level_Sensitive,
			Handler, (void *)InstPtr);

	//启用GPIO通道中断，并启用GPIO设备的中断
	XGpio_InterruptEnable(InstPtr, Mask);
	XGpio_InterruptGlobalEnable(InstPtr);
}

/*****************************************************************************
**  @brief	修改AXI GPIO某通道的某PIN的输入输出模式
**  @param	InstPtr		GPIO实例指针
**  @param	Channel		GPIO通道，1或2
**  @param	GPIO_Num	GPIO脚编号，0~31
**  @param	Dir			输入/输出：OUTPUT(0)为输出，INPUT(1)为输入
**  Sample:	//设置Gpio0的通道1的第0号端口为输出模式
**  		AXI_GPIO_SetPin_Dir(&AXI_GPIO0, XGPIO_IR_CH1_MASK, 0, OUTPUT);
*****************************************************************************/
void AXI_GPIO_SetPin_Dir(XGpio *InstPtr, uint8_t Channel, uint8_t GPIO_Num, uint8_t Dir)
{
	uint32_t Mode;
	Mode = XGpio_GetDataDirection(InstPtr, Channel);
	if(Dir == INPUT)
		Mode = Mode | (1 << GPIO_Num);//把对应Pin位设为1
	else if(Dir == OUTPUT)
		Mode = Mode & (~(1 << GPIO_Num));//把对应Pin位设为0
	XGpio_SetDataDirection(InstPtr, Channel, Mode);
}

/*****************************************************************************
**  @brief	控制特定GPIO口输出高/低电平,使用前必须先设置该GPIO口为"输出"模式
**  @param	InstPtr		GPIO实例指针
**  @param	Channel		GPIO通道，1或2
**  @param	GPIO_Num	GPIO脚的编号，0~31
**  @param	Data		输出电平高低：0为低，1为高
**  Sample:	//控制GPIO0的通道1的1号脚输出高电平
**  		AXI_GPIO_SetPort(&AXI_GPIO0, XGPIO_IR_CH1_MASK, 1, 1);
*****************************************************************************/
void AXI_GPIO_SetPin(XGpio *InstPtr, uint8_t Channel, uint8_t GPIO_Num, uint32_t Data)
{
	if(Data)
		XGpio_DiscreteSet(InstPtr,Channel,1 << GPIO_Num);
	else
		XGpio_DiscreteClear(InstPtr,Channel,1 << GPIO_Num);
}
/*****************************************************************************
**  @brief	读取特定GPIO口的输入电平,使用前必须设置该GPIO口为"输入"模式
**  @param	InstPtr		GPIO实例指针
**  @param	Channel		GPIO通道，1或2
**  @param	GPIO_Num	GPIO脚编号，从0开始
**  @return 返回采集的GPIO电平值，0为低，1为高
**  Sample:	//采集GPIO0的通道1的1号脚的电平，存放在state里
**  		state = AXI_GPIO_GetPort(&Gpio0, XGPIO_IR_CH1_MASK, 1);
*****************************************************************************/
uint8_t AXI_GPIO_GetPin(XGpio *InstPtr, uint8_t Channel, uint8_t GPIO_Num)
{
	uint32_t State;
	State = XGpio_DiscreteRead(InstPtr,Channel);
	State = (State >> GPIO_Num) & 0x1;
	return State;
}

/*****************************************************************************
**  @brief	设定AXI GPIO某通道的模式与状态
**  @param	InstPtr		GPIO实例指针
**  @param	Channel		GPIO通道，1或2
**  @param	Dir			输入/输出：0为输出，1为输入，每一位表示一个引脚
**  @param	Data		输出电平高低：0为低，1为高，每一位表示一个引脚
**  Sample:	//设置Gpio0的通道1全为输出模式，且全输出为低电平
**  		AXI_GPIO_Set_Channel(&AXI_GPIO0, XGPIO_IR_CH1_MASK, 0, 0);
*****************************************************************************/
void AXI_GPIO_Set_Channel(XGpio *InstPtr, uint8_t Channel, uint32_t Dri, uint32_t Data)
{
	XGpio_SetDataDirection(InstPtr, Channel, Dri);
	XGpio_DiscreteWrite(InstPtr, Channel, Data);
}
