/**
  *****************************************************************************
  * 						AXI_GPIO��ؿ⣨����ACZ702�����壩
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

//AXI GPIO����
XGpio AXI_GPIO0;

/*****************************************************************************
**  @brief	��ʼ��AXI_GPIO
**  @param	InstPtr		GPIOʵ��ָ��
**  @param	DeviceId	GPIO�豸ID
**	Sample:	AXI_GPIO_Init(&AXI_GPIO0,GPIO_0_ID);	//��ʼ��AXI GPIO0
*****************************************************************************/
void AXI_GPIO_Init(XGpio *InstPtr, uint16_t DeviceId)
{
	XGpio_Initialize(InstPtr, DeviceId);
}

/**
  *****************************************************
  * @brief	��ʼ��AXI_GPIO�ж�
  * @param	InstPtr		AXI_GPIO����ָ��
  * @param	IntrId		�ж�ID
  * @param	Mask		ͨ������
  * 					XGPIO_IR_CH1_MASK -> ͨ��1
  * 					XGPIO_IR_CH2_MASK -> ͨ��2
  * 					XGPIO_IR_MASK -> ͨ��1��ͨ��2
  * @param	Handler		�жϷ�����
  * @usage	//AXI_GPIO�жϳ�ʼ��,ֻ����ͨ��1���жϣ��жϷ�����ΪAXI_GPIO_IRQ_Handler
  * 		AXI_GPIO_Intc_Init(&AXI_GPIO0,XPAR_FABRIC_AXI_GPIO_0_IP2INTC_IRPT_INTR,
  * 				XGPIO_IR_CH1_MASK,AXI_GPIO_IRQ_Handler);
  *****************************************************
**/
void AXI_GPIO_Intc_Init(XGpio *InstPtr,uint16_t IntrId, uint32_t Mask,
		Xil_InterruptHandler Handler)
{
	Set_ScuGic_Link(IntrId, 0xA0, HIGH_Level_Sensitive,
			Handler, (void *)InstPtr);

	//����GPIOͨ���жϣ�������GPIO�豸���ж�
	XGpio_InterruptEnable(InstPtr, Mask);
	XGpio_InterruptGlobalEnable(InstPtr);
}

/*****************************************************************************
**  @brief	�޸�AXI GPIOĳͨ����ĳPIN���������ģʽ
**  @param	InstPtr		GPIOʵ��ָ��
**  @param	Channel		GPIOͨ����1��2
**  @param	GPIO_Num	GPIO�ű�ţ�0~31
**  @param	Dir			����/�����OUTPUT(0)Ϊ�����INPUT(1)Ϊ����
**  Sample:	//����Gpio0��ͨ��1�ĵ�0�Ŷ˿�Ϊ���ģʽ
**  		AXI_GPIO_SetPin_Dir(&AXI_GPIO0, XGPIO_IR_CH1_MASK, 0, OUTPUT);
*****************************************************************************/
void AXI_GPIO_SetPin_Dir(XGpio *InstPtr, uint8_t Channel, uint8_t GPIO_Num, uint8_t Dir)
{
	uint32_t Mode;
	Mode = XGpio_GetDataDirection(InstPtr, Channel);
	if(Dir == INPUT)
		Mode = Mode | (1 << GPIO_Num);//�Ѷ�ӦPinλ��Ϊ1
	else if(Dir == OUTPUT)
		Mode = Mode & (~(1 << GPIO_Num));//�Ѷ�ӦPinλ��Ϊ0
	XGpio_SetDataDirection(InstPtr, Channel, Mode);
}

/*****************************************************************************
**  @brief	�����ض�GPIO�������/�͵�ƽ,ʹ��ǰ���������ø�GPIO��Ϊ"���"ģʽ
**  @param	InstPtr		GPIOʵ��ָ��
**  @param	Channel		GPIOͨ����1��2
**  @param	GPIO_Num	GPIO�ŵı�ţ�0~31
**  @param	Data		�����ƽ�ߵͣ�0Ϊ�ͣ�1Ϊ��
**  Sample:	//����GPIO0��ͨ��1��1�Ž�����ߵ�ƽ
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
**  @brief	��ȡ�ض�GPIO�ڵ������ƽ,ʹ��ǰ�������ø�GPIO��Ϊ"����"ģʽ
**  @param	InstPtr		GPIOʵ��ָ��
**  @param	Channel		GPIOͨ����1��2
**  @param	GPIO_Num	GPIO�ű�ţ���0��ʼ
**  @return ���زɼ���GPIO��ƽֵ��0Ϊ�ͣ�1Ϊ��
**  Sample:	//�ɼ�GPIO0��ͨ��1��1�Žŵĵ�ƽ�������state��
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
**  @brief	�趨AXI GPIOĳͨ����ģʽ��״̬
**  @param	InstPtr		GPIOʵ��ָ��
**  @param	Channel		GPIOͨ����1��2
**  @param	Dir			����/�����0Ϊ�����1Ϊ���룬ÿһλ��ʾһ������
**  @param	Data		�����ƽ�ߵͣ�0Ϊ�ͣ�1Ϊ�ߣ�ÿһλ��ʾһ������
**  Sample:	//����Gpio0��ͨ��1ȫΪ���ģʽ����ȫ���Ϊ�͵�ƽ
**  		AXI_GPIO_Set_Channel(&AXI_GPIO0, XGPIO_IR_CH1_MASK, 0, 0);
*****************************************************************************/
void AXI_GPIO_Set_Channel(XGpio *InstPtr, uint8_t Channel, uint32_t Dri, uint32_t Data)
{
	XGpio_SetDataDirection(InstPtr, Channel, Dri);
	XGpio_DiscreteWrite(InstPtr, Channel, Data);
}
