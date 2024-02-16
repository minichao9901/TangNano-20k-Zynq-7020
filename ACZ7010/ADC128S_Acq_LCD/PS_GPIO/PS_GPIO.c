/**
  *****************************************************************************
  * 						PS����GPIO��ؿ�
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
//GPIO��صĽṹ�壬ʹ��GPIOʱ�������

XGpioPs Gpio;
volatile uint8_t Cnt = 0;
char Pic_Name[30];
static void PS_GPIO_IntrHandler(void *CallBackRef, uint32_t Bank, uint32_t Status)
{
	uint32_t Key_State;
	XGpioPs *Gpio = (XGpioPs *)CallBackRef;

	//����ж�����������Bank�ĸ���
	if(Bank == 1) {
		usleep(10000);	//��������10ms
		Key_State = XGpioPs_ReadPin(Gpio, KEY_S1);
		if(Key_State == 0) {
			//��ͼ������SD��
			sprintf(Pic_Name,"Screenshot(%02d).bmp",Cnt);
		    bmp_write(Pic_Name, (char *)&BMODE_800x480, (char *)frame);
		    Cnt++;
		}
	}
}

/***************************************************************************
**  @brief	��ʼ��PS��MIO��EMIO
**	Sample:	PS_GPIO_Init();	//��ʼ��PS��GPIO
*************************************************************************/
void PS_GPIO_Init()
{
	XGpioPs_Config *ConfigPtr;
	ConfigPtr = XGpioPs_LookupConfig(GPIO_DEVICE_ID);
	XGpioPs_CfgInitialize(&Gpio, ConfigPtr, ConfigPtr->BaseAddr);
}

/****************************************************************************
**  @brief	��ʼ���ض�GPIO�ڵ�ģʽ��״̬
**  @param	GPIO_Num	GPIO��ţ�MIOΪ0~53��EMIO��54��ʼ��
**  @param	Dir			����/�����OUTPUTΪ�����INPUTΪ����
**  @param	Data		�����ƽ�ߵͣ�0Ϊ�ͣ�1Ϊ�ߣ�������Ϊ������˴�������Ӱ�죩
**  Sample:	GPIO_SetMode(46, OUTPUT, 1);//����GPIO46Ϊ���ģʽ�����Ϊ�ߵ�ƽ
*****************************************************************************/
void PS_GPIO_SetMode(uint8_t GPIO_Num, uint8_t Dri, uint8_t Data)
{
	XGpioPs_SetDirectionPin(&Gpio, GPIO_Num, Dri);		//����LED�������0Ϊ���룬1Ϊ���
	XGpioPs_SetOutputEnablePin(&Gpio, GPIO_Num, Dri);	//���ʹ�ܣ�0Ϊ���ã�1Ϊʹ��
	XGpioPs_WritePin(&Gpio, GPIO_Num, Data);			//�����/�͵�ƽ��0Ϊ�ͣ�1Ϊ��
}

/****************************************************************************
**  @brief	�����ض�GPIO�ڵ������/�͵�ƽ,ʹ��ǰ��������GPIO_SetMode���ø�GPIO��Ϊ"���"ģʽ
**  @param	GPIO_Num	GPIO��ţ�MIOΪ0~53��EMIO��54��ʼ��
**  @param	Data		�����ƽ�ߵͣ�0Ϊ�ͣ�1Ϊ��
**  Sample:	GPIO_SetPort(46, 1);//����GPIO46����ߵ�ƽ
*****************************************************************************/
void PS_GPIO_SetPort(uint8_t GPIO_Num, uint8_t Data)
{
	XGpioPs_WritePin(&Gpio, GPIO_Num, Data);	//�����/�͵�ƽ��0Ϊ�ͣ�1Ϊ��
}
/*****************************************************************************
**  @brief	��ȡ�ض�GPIO�ڵ������ƽ,ʹ��ǰ��������GPIO_SetMode���ø�GPIO��Ϊ"����"ģʽ
**  @param	GPIO_Num	GPIO��ţ�MIOΪ0~53��EMIO��54��ʼ��
**  @return ���زɼ���GPIO��ƽֵ��0Ϊ�ͣ�1Ϊ��
**  Sample:	state = GPIO_GetPort(46, state);	//�ɼ�GPIO46�ĵ�ƽ�������state��
*****************************************************************************/
uint8_t PS_GPIO_GetPort(uint8_t GPIO_Num)
{
	return XGpioPs_ReadPin(&Gpio, GPIO_Num);		//�����/�͵�ƽ��0Ϊ�ͣ�1Ϊ��
}
/****************************************************************************
**  @brief	��ʼ��PS GPIO���жϹ���
**  @param	IntInstPtr	�ж�ʵ��
**  Sample:	PS_GPIO_Int_Init(&IntInstPtr);//��ʼ��PS GPIO���жϹ���
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
**  @brief	����GPIOĳPin���жϹ���
**  @param	InstPtr
**  @param	GPIO_Num	GPIO��ţ�MIOΪ0~53��EMIO��54��ʼ��
**  @param	IntcMode	GPIO�жϴ���ģʽ
**  					XGPIOPS_IRQ_TYPE_EDGE_RISING	�����ش���
**  					XGPIOPS_IRQ_TYPE_EDGE_FALLING	�½��ش���
**  					XGPIOPS_IRQ_TYPE_EDGE_BOTH		�������½��ض�����
**  					XGPIOPS_IRQ_TYPE_LEVEL_HIGH		�ߵ�ƽ����
**  					XGPIOPS_IRQ_TYPE_LEVEL_LOW		�͵�ƽ����
**  Sample:	PS_GPIO_SetInt(&Intc,47,XGPIOPS_IRQ_TYPE_EDGE_FALLING);	//����MIO47Ϊ�½����ж�
*****************************************************************************/
void PS_GPIO_SetInt(XScuGic *IntInstPtr, uint8_t GPIO_Num,uint8_t IntcMode)
{
	//���������ж�ģʽ
	XGpioPs_SetIntrTypePin(&Gpio, GPIO_Num, IntcMode);

	//Ϊ gpio�ж����ô������
	XGpioPs_SetCallbackHandler(&Gpio, (void *)&Gpio, PS_GPIO_IntrHandler);

	//ʹ��Bank��GPIO Pin�ж�
	XGpioPs_IntrEnablePin(&Gpio, GPIO_Num);

	//�����ж�
	XScuGic_Enable(IntInstPtr, XPAR_XGPIOPS_0_INTR);
}
