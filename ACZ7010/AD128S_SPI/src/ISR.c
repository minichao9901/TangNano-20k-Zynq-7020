/**
  *****************************************************************************
  * 					����û��жϴ�����������ͳһ����
  *****************************************************************************
  *
  * @File   : ISR.c
  * @By     : Sun
  * @Version: V0.5
  * @Date   : 2022 / 06 / 01
  * @Shop	: https://xiaomeige.taobao.com/
  *
  *****************************************************************************
**/

#include "ISR.h"

extern uint8_t Dig_Data[8][2];
extern uint8_t ADC_Channel;
static uint8_t i,Flag_200ms;	//200ms��־λ


/**
  *****************************************************
  * @brief	˽�ж�ʱ���жϴ������
  * @tag	��������������˽�ж�ʱ���жϣ����ڲ������û����򼴿�
  *****************************************************
**/
void ScuTimer_IRQ_Handler(void *CallBackRef)
{
	/* �������û���������� */

    //ÿ��200ms���һ�ΰ���
    if (Flag_200ms < 100) {
    	Flag_200ms++;
    } else {
    	Flag_200ms = 0;
    	if((!PS_GPIO_GetPort(54)) && ADC_Channel < 7)
    		ADC_Channel++;
    	else if ((!PS_GPIO_GetPort(55)) && ADC_Channel > 0)
    		ADC_Channel--;
    }

    //ÿ��2ms��SPI�����������ʾ��iλ����
	PS_SPI_Transfer(&SPI1, 0, 0,Dig_Data[i], 2);
	if(i<8)
		i++;
	else
		i = 0;

	/* ������������������� */
    XScuTimer_ClearInterruptStatus(&ScuTimer);
}
