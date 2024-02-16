/*
 * ʹ��PS_GPIOʵ���˰�������LED����:
 * PS�˰������¡�PL�˵ĵ������ɿ�Ϩ��
 * PL�˰������¡�PS�˵ĵ������ɿ�Ϩ��
*/
#include "COMMON.h"

int main(void)
{
    uint8_t State;   //��Ű���(MIO47)�ĵ�ƽ״̬��0Ϊ�͵�ƽ��1Ϊ�ߵ�ƽ

    PS_GPIO_Init(); //��ʼ��PS��MIO��EMIO
    //����PS_LED(MIO7)Ϊ������ҳ�ʼΪ�͵�ƽ
    PS_GPIO_SetMode(PS_LED, OUTPUT, 0);
    //����PL_LED(EMIO0)Ϊ������ҳ�ʼΪ�͵�ƽ
    PS_GPIO_SetMode(PL_LED, OUTPUT, 0);
    PS_GPIO_SetMode(PS_KEY, INPUT, 0); //����PS_KEY(MIO47)����Ϊ����
    PS_GPIO_SetMode(PL_KEY, INPUT, 0); //����PL_KEY(EMIO1)����Ϊ����

    while(1)
    {
        State = PS_GPIO_GetPort(PS_KEY);   //��ȡPS_KEY�ĵ�ƽֵ���洢��State������
        PS_GPIO_SetPort(PL_LED,!State);    //��State������ֵȡ�Ǹ���PL_LED�����
        State = PS_GPIO_GetPort(PL_KEY);   //��ȡPL_KEY�ĵ�ƽֵ���洢��State������
        PS_GPIO_SetPort(PS_LED,!State);    //��State������ֵȡ�Ǹ���PS_LED�����
    }
    return 0;
}
