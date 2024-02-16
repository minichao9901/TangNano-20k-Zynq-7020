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

//GPIO��صĽṹ�壬ʹ��GPIOʱ�������

XGpioPs Gpio;

/************************************************************************************************************
**  @brief	��ʼ��PS��MIO��EMIO
**	Sample:	PS_GPIO_Init();	//��ʼ��PS��GPIO
************************************************************************************************************/
void PS_GPIO_Init()
{
	XGpioPs_Config *ConfigPtr;
	ConfigPtr = XGpioPs_LookupConfig(GPIO_DEVICE_ID);
	XGpioPs_CfgInitialize(&Gpio, ConfigPtr, ConfigPtr->BaseAddr);
}

/************************************************************************************************************
**  @brief	��ʼ���ض�GPIO�ڵ�ģʽ��״̬
**  @param	GPIO_Num	GPIO��ţ�MIOΪ0~53��EMIO��54��ʼ��
**  @param	Dir			����/�����OUTPUTΪ�����INPUTΪ����
**  @param	Data		�����ƽ�ߵͣ�0Ϊ�ͣ�1Ϊ�ߣ�������Ϊ������˴�������Ӱ�죩
**  Sample:	GPIO_SetMode(46, OUTPUT, 1);//����GPIO46Ϊ���ģʽ�����Ϊ�ߵ�ƽ
************************************************************************************************************/
void GPIO_SetMode(uint8_t GPIO_Num, uint8_t Dri, uint8_t Data)
{
	XGpioPs_SetDirectionPin(&Gpio, GPIO_Num, Dri);		//����LED�������0Ϊ���룬1Ϊ���
	XGpioPs_SetOutputEnablePin(&Gpio, GPIO_Num, Dri);	//���ʹ�ܣ�0Ϊ���ã�1Ϊʹ��
	XGpioPs_WritePin(&Gpio, GPIO_Num, Data);			//�����/�͵�ƽ��0Ϊ�ͣ�1Ϊ��
}

/************************************************************************************************************
**  @brief	�����ض�GPIO�ڵ������/�͵�ƽ,ʹ��ǰ��������GPIO_SetMode���ø�GPIO��Ϊ"���"ģʽ
**  @param	GPIO_Num	GPIO��ţ�MIOΪ0~53��EMIO��54��ʼ��
**  @param	Data		�����ƽ�ߵͣ�0Ϊ�ͣ�1Ϊ��
**  Sample:	GPIO_SetPort(46, 1);//����GPIO46����ߵ�ƽ
************************************************************************************************************/
void GPIO_SetPort(uint8_t GPIO_Num, uint8_t Data)
{
	XGpioPs_WritePin(&Gpio, GPIO_Num, Data);	//�����/�͵�ƽ��0Ϊ�ͣ�1Ϊ��
}
/************************************************************************************************************
**  @brief	��ȡ�ض�GPIO�ڵ������ƽ,ʹ��ǰ��������GPIO_SetMode���ø�GPIO��Ϊ"����"ģʽ
**  @param	GPIO_Num	GPIO��ţ�MIOΪ0~53��EMIO��54��ʼ��
**  @return ���زɼ���GPIO��ƽֵ��0Ϊ�ͣ�1Ϊ��
**  Sample:	state = GPIO_GetPort(46, state);	//�ɼ�GPIO46�ĵ�ƽ�������state��
************************************************************************************************************/
uint8_t GPIO_GetPort(uint8_t GPIO_Num)
{
	return XGpioPs_ReadPin(&Gpio, GPIO_Num);		//�����/�͵�ƽ��0Ϊ�ͣ�1Ϊ��
}
