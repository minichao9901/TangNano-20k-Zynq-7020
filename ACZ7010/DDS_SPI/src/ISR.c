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

uint8_t Channel,DAC_Wave,F_Word_cnt=1,P_Word_cnt=0;
uint32_t F_Word=0;
uint16_t P_Word=0;
volatile uint32_t Key_Int_Num = 0;//�����жϱ�ű�ʶ
/**
  *****************************************************
  * @brief	˽�ж�ʱ���жϴ������
  * @tag	��������������˽�ж�ʱ���жϣ����ڲ������û����򼴿�
  *****************************************************
**/
void ScuTimer_IRQ_Handler(void *CallBackRef)
{
	static uint32_t m=0;
	static uint16_t addr=0;
	static uint8_t n=0;	//��ʱ��־λ
	/* �������û���������� */
	if(Channel == 2){
	    //4096������
		m = m+F_Word ;
		addr = m>>20;
		if(!n){
			n = 1;
			//Bͨ�� ����д�뻺����
			AXI_SPI_Sendshort(&AXI_SPI0, (*(Wave_Point+addr) | CHAB_CtrlWord));
		}
		else{
			n = 0;
			addr += P_Word;//��ַ+��λ������
			if(addr > 4095)//����ַ�Ƿ����
				addr -= 4096;//������0��ʼ����
			else{
				addr = addr;
			}
			//����λƫ�ƺ������д��Aͨ�������ӻ���������B
			AXI_SPI_Sendshort(&AXI_SPI0, (*(Wave_Point+addr) | CHABC_CtrlWord));
		}
 }
	 //��ͨ��ֱ��д
	 else {
	    	m = m+F_Word;
	    	addr = m >> 20;
	    	AXI_SPI_Sendshort(&AXI_SPI0,(*(Wave_Point+addr) | DAC_CH_CtrlWord));
	 }

	/* ������������������� */
    XScuTimer_ClearInterruptStatus(&ScuTimer);
}

//�����жϴ���������ӦGPIO�����ж�ʱ������־ֵ
void PS_GPIO_IRQ_Handler(void *CallBackRef, uint32_t Bank, uint32_t Status)
{
	uint32_t Int_State,Int_State1;

	Int_State = XGpioPs_IntrGetStatusPin(&GpioPs, KEY_S0);
	if(Int_State) {
		TTC_Tick_Start(&TTC0_1_Timer);//������ʱ������ʱ10ms
		Key_Int_Num |= 1;
	}

	Int_State = XGpioPs_IntrGetStatusPin(&GpioPs, KEY_S1);
	if(Int_State) {
		TTC_Tick_Start(&TTC0_1_Timer);//������ʱ������ʱ10ms
		Key_Int_Num |= 2;
	}

	Int_State = XGpioPs_IntrGetStatusPin(&GpioPs, KEY_S2);
	if(Int_State) {
		TTC_Tick_Start(&TTC0_1_Timer);//������ʱ������ʱ10ms
		Key_Int_Num |= 4;
	}

	Int_State = XGpioPs_IntrGetStatusPin(&GpioPs, SW0);
	Int_State1 = XGpioPs_IntrGetStatusPin(&GpioPs, SW1);
	if(Int_State | Int_State1) {
		TTC_Tick_Start(&TTC0_1_Timer);//������ʱ������ʱ10ms
		Key_Int_Num |= 8;
	}

	Int_State = XGpioPs_IntrGetStatusPin(&GpioPs, KEY_S3);
	if(Int_State) {
		TTC_Tick_Start(&TTC0_1_Timer);//������ʱ������ʱ10ms
		Key_Int_Num |= 16;
	}

	Int_State = XGpioPs_IntrGetStatusPin(&GpioPs, KEY_S4);
	if(Int_State) {
		TTC_Tick_Start(&TTC0_1_Timer);//������ʱ������ʱ10ms
		Key_Int_Num |= 32;
	}
}

//TTC0��ʱ��1���жϴ�����
void TTC0_1_IRQ_Handler(void *CallBackRef)
{
    uint32_t StatusEvent;
    /* �������û���������� */
    uint32_t Key_State,SW0_State,SW1_State;

    //�ر�TTC0_1
    TTC_Tick_Stop(&TTC0_1_Timer);

    //����ͨ���л�
    if(Key_Int_Num & 1){
    	Key_State = XGpioPs_ReadPin(&GpioPs, KEY_S0);
		if((!Key_State)&& (Channel <= 1))
			Channel++;
		else if((!Key_State)&& (Channel > 1))
			Channel = 0;
		else
			Channel = Channel;
    }

    //�ٴμ�⵽����S1������Ƶ�ʼ�����+1
    if(Key_Int_Num & 2){
		Key_State = XGpioPs_ReadPin(&GpioPs, KEY_S1);
		if((!Key_State)&&(F_Word_cnt <= 5)){
			F_Word_cnt++;
		}
		else{
			F_Word_cnt = F_Word_cnt;
		}
    }

    //�ٴμ�⵽S2���£���Ƶ�ʼ�����-1
    if(Key_Int_Num & 4){
    	Key_State = XGpioPs_ReadPin(&GpioPs, KEY_S2);
		if((!Key_State)&& (F_Word_cnt > 1)){
			F_Word_cnt--;
		}
		else{
			F_Word_cnt = F_Word_cnt;
		}
    }

    //Ƶ�ʿ�����
    if(F_Word_cnt == 1)
    	F_Word = 0x100000;//100Hz
    else if(F_Word_cnt == 2)
        F_Word = 0x200000;//200Hz
    else if(F_Word_cnt == 3)
        F_Word = 0x500000;//500Hz
    else if(F_Word_cnt == 4)
    	F_Word = 0xA00000;//1KHz
    else if(F_Word_cnt == 5)
        F_Word = 0xF00000;//1.5KHz
    else if(F_Word_cnt == 6)
        F_Word = 0x1400000;//2KHz

    //���ݲ��뿪��SW1��SW0�����ֵ�ж������������
    if(Key_Int_Num & 8){
    	SW0_State = XGpioPs_ReadPin(&GpioPs, SW0);
		SW1_State = XGpioPs_ReadPin(&GpioPs, SW1);
		if((!SW1_State)&(!SW0_State)) //00
			DAC_Wave = 0;
		else if((!SW1_State)&(SW0_State))//01
			DAC_Wave = 1;
		else if((SW1_State)&(!SW0_State))//10
			DAC_Wave = 2;
		else
			DAC_Wave = DAC_Wave;
    }

    //����ٴμ��S3���£���λ������+1
    if(Key_Int_Num & 16){
		Key_State = XGpioPs_ReadPin(&GpioPs, KEY_S3);
		if((!Key_State)&&(P_Word_cnt <= 7)){
			P_Word_cnt++;
		}
		else{
			P_Word_cnt = P_Word_cnt;
		}
    }

    //����ٴμ�ⰴ�£���λ������-1
    if(Key_Int_Num & 32){
    	Key_State = XGpioPs_ReadPin(&GpioPs, KEY_S4);
		if((!Key_State)&& (P_Word_cnt > 0)){
			P_Word_cnt--;
		}
		else{
			P_Word_cnt = P_Word_cnt;
		}
    }

    //��λƫ��ֵ
    if(P_Word_cnt == 0)
    	P_Word = 0;
    else if(P_Word_cnt == 1)
    	P_Word = 341;//��30/360��*4096=341��30��
    else if(P_Word_cnt == 2)
    	P_Word = 512;//45��
    else if(P_Word_cnt == 3)
    	P_Word = 1024;//90��
    else if(P_Word_cnt == 4)
    	P_Word = 1536;//135��
    else if(P_Word_cnt == 5)
    	P_Word = 2048;//180��
    else if(P_Word_cnt == 6)
    	P_Word = 2560;//225��
    else if(P_Word_cnt == 7)
    	P_Word = 3072;//270��
    else if(P_Word_cnt == 8)
    	P_Word = 3584;//315��

    /* ������������������� */
    StatusEvent = XTtcPs_GetInterruptStatus(&TTC0_1_Timer);
    XTtcPs_ClearInterruptStatus(&TTC0_1_Timer, StatusEvent);
}

