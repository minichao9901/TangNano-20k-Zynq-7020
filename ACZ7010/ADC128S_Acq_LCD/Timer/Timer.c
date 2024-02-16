#include "Timer.h"

//�����������̵ı�־λ
extern uint8_t Flag_DrawWave;		//���Ʋ��Σ�10msһ��
extern uint8_t Flag_DrawGrid;		//��������20msһ��
extern uint8_t Flag_TouchScan;		//����ɨ�裬30msһ��
extern uint8_t Flag_Refresh_Val;	//ˢ�µ�ѹ��Ƶ����ֵ��500msһ��
extern uint8_t Flag_Refresh_Time;	//ˢ��ʱ�䣬500msһ��

XScuGic Intc;               //�жϿ�������������ʵ��
XScuTimer Timer;            //��ʱ����������ʵ��

//����ʱ���־λ
static uint8_t Flag_20ms = 0;
static uint8_t Flag_30ms = 0;
static uint8_t Flag_50ms = 0;
static uint8_t Flag_100ms = 0;
static uint8_t Flag_200ms = 0;
static uint8_t Flag_500ms = 0;

//��ʱ���жϳ�ʼ�����������ڶ�ʱ����ʼ������Timer_Init�ڲ����ã��ⲿ������á�
void Timer_Intr_Init(XScuGic *Intc_Ptr,XScuTimer *Timer_Ptr);
void touch_button();

/**
  *****************************************************
  * @brief	��ʱ���жϴ������
  * @tag	��������������ʱ���жϣ����ڲ������û����򼴿�
  *****************************************************
**/
void Timer_Intr_Handler(void *CallBackRef)
{
    XScuTimer *Timer_Ptr = (XScuTimer *) CallBackRef;


    /*****����������жϷ������û���Ҫִ�еĲ���*****/
    /*** ��ʼ�� ***/
    //����ͨ����ʱ������ÿ�����̵Ĵ��������������Ӧ�ı�־λ
    Flag_DrawWave = 1;

    if(Flag_20ms >= 1) {
    	Flag_20ms = 0;
    	Flag_DrawGrid = 1;
    }
    else
    	Flag_20ms++;

    if(Flag_30ms >= 2) {
    	Flag_30ms = 0;
    	Flag_TouchScan = 1;
    }
    else
    	Flag_30ms++;

    if(Flag_50ms >= 4) {
    	Flag_50ms = 0;

    }
    else
    	Flag_50ms++;

    if(Flag_100ms >= 9) {
    	Flag_100ms = 0;

    }
    else
    	Flag_100ms++;

    if(Flag_200ms >= 19) {
    	Flag_200ms = 0;

    }
    else
    	Flag_200ms++;

    if(Flag_500ms >= 49) {
    	Flag_500ms = 0;
    	Flag_Refresh_Val = 1;
    	Flag_Refresh_Time = 1;
    }
    else
    	Flag_500ms++;

    /*** ������ ***/


    //����жϱ�־
    XScuTimer_ClearInterruptStatus(Timer_Ptr);
}

/**
  *****************************************************
  * @brief	��ʼ����ʱ��
  * @param	*Intc_Ptr	ָ��Intc_Ptrʵ����ָ��
  * @param	*Timer_Ptr	ָ��XScuTimerʵ����ָ��
  * @param	Time_us		��ʱ���жϴ���ʱ�䣬��λΪus
  * @usage	Timer_Init(&Intc, &Timer, 2000);	//��ʼ����ʱ����ÿ2000us����һ��
  * 		XScuTimer_Start (&Timer ); //������ʱ��
  * @tag	��������������ʼ����ʱ����Ҫ������ʱ����Ҫʹ��XScuTimer_Start������ֹͣ����XScuTimer_Stop
  *****************************************************
**/
void Timer_Init(XScuGic *Intc_Ptr,XScuTimer *Timer_Ptr, uint32_t Time_us)
{
    //��ʱ����ʼ��
    XScuTimer_Config *Timer_Cfg_Ptr;
    Timer_Cfg_Ptr = XScuTimer_LookupConfig(XPAR_XSCUTIMER_0_DEVICE_ID);
    XScuTimer_CfgInitialize(Timer_Ptr, Timer_Cfg_Ptr,Timer_Cfg_Ptr->BaseAddr);
    XScuTimer_LoadTimer(Timer_Ptr, TIMER_LOAD_VALUE(Time_us));	//���ؼ�������
    XScuTimer_EnableAutoReload(Timer_Ptr);	//�����Զ�װ��ģʽ
	Timer_Intr_Init(Intc_Ptr,Timer_Ptr);   //��ʱ���жϳ�ʼ��
}

/**
  *****************************************************
  * @brief	��ʱ���жϳ�ʼ��������
  * @tag	���ڶ�ʱ����ʼ������Timer_Init�ڲ����ã��ⲿ������á�
  *****************************************************
**/
void Timer_Intr_Init(XScuGic *Intc_Ptr,XScuTimer *Timer_Ptr)
{
    //��ʼ���жϿ�����
    XScuGic_Config *Intc_Cfg_Ptr;
    Intc_Cfg_Ptr = XScuGic_LookupConfig(XPAR_SCUGIC_SINGLE_DEVICE_ID);
    XScuGic_CfgInitialize(Intc_Ptr, Intc_Cfg_Ptr,Intc_Cfg_Ptr->CpuBaseAddress);
    //���ò����ж��쳣������
    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
            (Xil_ExceptionHandler)XScuGic_InterruptHandler, Intc_Ptr);
    Xil_ExceptionEnable();
    //���ö�ʱ���ж�
    XScuGic_Connect(Intc_Ptr, XPAR_SCUTIMER_INTR,
    		(Xil_ExceptionHandler)Timer_Intr_Handler, (void *)Timer_Ptr);
    //ʹ��GIC�еĶ�ʱ���ж�
    XScuGic_Enable(Intc_Ptr, XPAR_SCUTIMER_INTR);
    //ʹ�ܶ�ʱ���ж�
    XScuTimer_EnableInterrupt(Timer_Ptr);
}
