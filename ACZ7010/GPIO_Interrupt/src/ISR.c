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
volatile uint32_t Key_Int_Num = 0;//�����жϱ�ű�ʶ
volatile uint32_t Key_Cnt0 = 0,Key_Cnt1 = 0;//��������

/**
  *****************************************************
  * @brief	˽�ж�ʱ���жϴ������
  * @tag	��������������˽�ж�ʱ���жϣ����ڲ������û����򼴿�
  *****************************************************
**/
void ScuTimer_IRQ_Handler(void *CallBackRef)
{
	/* �������û���������� */


	/* ������������������� */
    XScuTimer_ClearInterruptStatus(&ScuTimer);
}

void PS_GPIO_IRQ_Handler(void *CallBackRef, uint32_t Bank, uint32_t Status)
{
    uint32_t Int_State;

    Int_State = XGpioPs_IntrGetStatusPin(&GpioPs, PS_KEY);
    if(Int_State) {
        TTC_Tick_Start(&TTC0_1_Timer);//������ʱ������ʱ10ms
        Key_Int_Num |= 1;
    }
}

//GPIO���жϴ������,ֻҪGPIOͨ��0�е�ƽ�仯�ͻ������ж�
void AXI_GPIO_IRQ_Handler(void *CallbackRef)
{
    uint32_t Key_Int_State;

    //�ж���һλ�����ж�
    Key_Int_State = XGpio_InterruptGetStatus(&AXI_GPIO0);

    /* �������û���������� */
        if(Key_Int_State & (1<<KEY_S1)) {

                //��������������TTC0_1����
                TTC_Tick_Start(&TTC0_1_Timer);
                Key_Int_Num |= 2;
        }
    /* ������������������� */

    //����жϱ�־
    XGpio_InterruptClear(&AXI_GPIO0, XGPIO_IR_CH1_MASK);
}

//TTC0��1�ż��������жϴ�����
void TTC0_1_IRQ_Handler(void *CallBackRef)
{
    uint32_t StatusEvent;
    /* �������û���������� */
        uint32_t Key_State;

        //�ر�TTC0_1����
        TTC_Tick_Stop(&TTC0_1_Timer);

        if(Key_Int_Num & 1) {
            //�жϰ������
            Key_State = XGpioPs_ReadPin(&GpioPs, PS_KEY);
            if(!Key_State) {//�͵�ƽ��ʾ����
                Key_Cnt0++;
                LOGW("PS Key Press!\n");
                LOGW("Cnt0: %lu\n",Key_Cnt0);

            } else if(Key_State) {//�ߵ�ƽ��ʾ����
                LOGW("PS Key Release!\n");
            }
            Key_Int_Num ^= 1;
        }

        if(Key_Int_Num & 2) {
            //��ȡ����S1��ֵ
            Key_State = AXI_GPIO_GetPin(&AXI_GPIO0,XGPIO_IR_CH1_MASK,0);
            if(!Key_State) {//�͵�ƽ��ʾ����
                Key_Cnt1++;
                LOGW("AXI Key Press!\n");
                LOGW("Cnt1: %lu\n",Key_Cnt1);
            } else if(Key_State){
                LOGW("AXI Key Release!\n");
            }
            Key_Int_Num ^= 2;
        }

    /* ������������������� */
    StatusEvent = XTtcPs_GetInterruptStatus(&TTC0_1_Timer);
    XTtcPs_ClearInterruptStatus(&TTC0_1_Timer, StatusEvent);
}
