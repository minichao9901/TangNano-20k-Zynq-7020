/**
  *****************************************************************************
  * 						PS����GPIO���Ӧ�ÿ�
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

XGpioPs GpioPs;	//GPIOʵ������


/**
  *****************************************************
  * @brief	��ʼ��PS��MIO��EMIO
  * @usage	PS_GPIO_Init();	//��ʼ��PS��GPIO
  *****************************************************
**/
void PS_GPIO_Init()
{
	XGpioPs_Config *ConfigPtr;
	ConfigPtr = XGpioPs_LookupConfig(XPAR_XGPIOPS_0_DEVICE_ID);
	XGpioPs_CfgInitialize(&GpioPs, ConfigPtr, ConfigPtr->BaseAddr);
}


/**
  *****************************************************
  * @brief	��ʼ��PS GPIO���жϹ���
  * @usage	//��ʼ��PS GPIO���жϹ���
  * 		PS_GPIO_Int_Init();
  *****************************************************
**/
void PS_GPIO_Int_Init()
{
    //���ӵ��жϿ�����
    Set_ScuGic_Link(XPAR_XGPIOPS_0_INTR, 0xA0, HIGH_Level_Sensitive,
    		(Xil_InterruptHandler)XGpioPs_IntrHandler, (void *)&GpioPs);

	//Ϊ gpio�ж����ô������
	XGpioPs_SetCallbackHandler(&GpioPs, (void *)&GpioPs, PS_GPIO_IRQ_Handler);
}


/**
  *****************************************************
  * @brief	����GPIOĳPin���жϹ���
  * @param	GPIO_Num	GPIO��ţ�MIOΪ0~53��EMIO��54��ʼ��
  * @param	IntcMode	GPIO�жϴ���ģʽ
  * 					XGPIOPS_IRQ_TYPE_EDGE_RISING	�����ش���
  * 					XGPIOPS_IRQ_TYPE_EDGE_FALLING	�½��ش���
  * 					XGPIOPS_IRQ_TYPE_EDGE_BOTH		�������½��ض�����
  * 					XGPIOPS_IRQ_TYPE_LEVEL_HIGH		�ߵ�ƽ����
  * 					XGPIOPS_IRQ_TYPE_LEVEL_LOW		�͵�ƽ����
  * @usage	//����MIO47Ϊ�½����ж�
  * 		PS_GPIO_SetInt(47,XGPIOPS_IRQ_TYPE_EDGE_FALLING);
  *****************************************************
**/
void PS_GPIO_SetInt(uint8_t GPIO_Num,uint8_t IntcMode)
{
	//���������ж�ģʽ
	XGpioPs_SetIntrTypePin(&GpioPs, GPIO_Num, IntcMode);

	//ʹ��Bank��GPIO Pin�ж�
	XGpioPs_IntrEnablePin(&GpioPs, GPIO_Num);
}

/**
  *****************************************************
  * @brief	��ʼ���ض�GPIO�ڵ�ģʽ��״̬
  * @param	GPIO_Num	GPIO��ţ�MIOΪ0~53��EMIO��54��ʼ��
  * @param	Dir			����/�����OUTPUTΪ�����INPUTΪ����
  * @param	Data		�����ƽ�ߵͣ�0Ϊ�ͣ�1Ϊ�ߣ�������Ϊ������˴�������Ӱ�죩
  * @usage	//����GPIO46Ϊ���ģʽ�����Ϊ�ߵ�ƽ
  * 		GPIO_SetMode(46, OUTPUT, 1);
  *****************************************************
**/
void PS_GPIO_SetMode(uint8_t GPIO_Num, uint8_t Dir, uint8_t Data)
{
	if(Dir == INPUT) {
		XGpioPs_SetDirectionPin(&GpioPs, GPIO_Num, 0);
	} else if(Dir == OUTPUT){
		XGpioPs_SetDirectionPin(&GpioPs, GPIO_Num, 1);
		XGpioPs_SetOutputEnablePin(&GpioPs, GPIO_Num, 1);
		XGpioPs_WritePin(&GpioPs, GPIO_Num, Data);
	}
}

/**
  *****************************************************
  * @brief	�����ض�GPIO�ڵ������/�͵�ƽ,ʹ��ǰ��������GPIO_SetMode���ø�GPIO��Ϊ"���"ģʽ
  * @param	GPIO_Num	GPIO��ţ�MIOΪ0~53��EMIO��54��ʼ��
  * @param	Data		�����ƽ�ߵͣ�0Ϊ�ͣ�1Ϊ��
  * @usage	//����GPIO46����ߵ�ƽ
  * 		GPIO_SetPort(46, 1);
  *****************************************************
**/
void PS_GPIO_SetPort(uint8_t GPIO_Num, uint8_t Data)
{
	XGpioPs_WritePin(&GpioPs, GPIO_Num, Data);	//�����/�͵�ƽ��0Ϊ�ͣ�1Ϊ��
}

/**
  *****************************************************
  * @brief	��ȡ�ض�GPIO�ڵ������ƽ,ʹ��ǰ��������GPIO_SetMode���ø�GPIO��Ϊ"����"ģʽ
  * @param	GPIO_Num	GPIO��ţ�MIOΪ0~53��EMIO��54��ʼ��
  * @return	���زɼ���GPIO��ƽֵ��0Ϊ�ͣ�1Ϊ��
  * @usage	//�ɼ�GPIO46�ĵ�ƽ�������state
  * 		state = GPIO_GetPort(46, state);
  *****************************************************
**/
uint8_t PS_GPIO_GetPort(uint8_t GPIO_Num)
{
	return XGpioPs_ReadPin(&GpioPs, GPIO_Num);		//�����/�͵�ƽ��0Ϊ�ͣ�1Ϊ��
}

