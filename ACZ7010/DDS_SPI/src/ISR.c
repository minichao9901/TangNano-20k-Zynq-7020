/**
  *****************************************************************************
  * 					存放用户中断处理函数，方便统一处理
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
volatile uint32_t Key_Int_Num = 0;//按键中断编号标识
/**
  *****************************************************
  * @brief	私有定时器中断处理程序
  * @tag	本函数用来处理私有定时器中断，在内部加入用户程序即可
  *****************************************************
**/
void ScuTimer_IRQ_Handler(void *CallBackRef)
{
	static uint32_t m=0;
	static uint16_t addr=0;
	static uint8_t n=0;	//定时标志位
	/* ↓↓↓用户处理↓↓↓ */
	if(Channel == 2){
	    //4096个数据
		m = m+F_Word ;
		addr = m>>20;
		if(!n){
			n = 1;
			//B通道 数据写入缓冲区
			AXI_SPI_Sendshort(&AXI_SPI0, (*(Wave_Point+addr) | CHAB_CtrlWord));
		}
		else{
			n = 0;
			addr += P_Word;//地址+相位控制字
			if(addr > 4095)//检测地址是否溢出
				addr -= 4096;//溢出则从0开始读数
			else{
				addr = addr;
			}
			//将相位偏移后的数据写入A通道，并从缓冲区更新B
			AXI_SPI_Sendshort(&AXI_SPI0, (*(Wave_Point+addr) | CHABC_CtrlWord));
		}
 }
	 //单通道直接写
	 else {
	    	m = m+F_Word;
	    	addr = m >> 20;
	    	AXI_SPI_Sendshort(&AXI_SPI0,(*(Wave_Point+addr) | DAC_CH_CtrlWord));
	 }

	/* ↑↑↑结束处理↑↑↑ */
    XScuTimer_ClearInterruptStatus(&ScuTimer);
}

//按键中断处理函数，对应GPIO触发中断时给出标志值
void PS_GPIO_IRQ_Handler(void *CallBackRef, uint32_t Bank, uint32_t Status)
{
	uint32_t Int_State,Int_State1;

	Int_State = XGpioPs_IntrGetStatusPin(&GpioPs, KEY_S0);
	if(Int_State) {
		TTC_Tick_Start(&TTC0_1_Timer);//开启定时器，延时10ms
		Key_Int_Num |= 1;
	}

	Int_State = XGpioPs_IntrGetStatusPin(&GpioPs, KEY_S1);
	if(Int_State) {
		TTC_Tick_Start(&TTC0_1_Timer);//开启定时器，延时10ms
		Key_Int_Num |= 2;
	}

	Int_State = XGpioPs_IntrGetStatusPin(&GpioPs, KEY_S2);
	if(Int_State) {
		TTC_Tick_Start(&TTC0_1_Timer);//开启定时器，延时10ms
		Key_Int_Num |= 4;
	}

	Int_State = XGpioPs_IntrGetStatusPin(&GpioPs, SW0);
	Int_State1 = XGpioPs_IntrGetStatusPin(&GpioPs, SW1);
	if(Int_State | Int_State1) {
		TTC_Tick_Start(&TTC0_1_Timer);//开启定时器，延时10ms
		Key_Int_Num |= 8;
	}

	Int_State = XGpioPs_IntrGetStatusPin(&GpioPs, KEY_S3);
	if(Int_State) {
		TTC_Tick_Start(&TTC0_1_Timer);//开启定时器，延时10ms
		Key_Int_Num |= 16;
	}

	Int_State = XGpioPs_IntrGetStatusPin(&GpioPs, KEY_S4);
	if(Int_State) {
		TTC_Tick_Start(&TTC0_1_Timer);//开启定时器，延时10ms
		Key_Int_Num |= 32;
	}
}

//TTC0定时器1的中断处理函数
void TTC0_1_IRQ_Handler(void *CallBackRef)
{
    uint32_t StatusEvent;
    /* ↓↓↓用户处理↓↓↓ */
    uint32_t Key_State,SW0_State,SW1_State;

    //关闭TTC0_1
    TTC_Tick_Stop(&TTC0_1_Timer);

    //控制通道切换
    if(Key_Int_Num & 1){
    	Key_State = XGpioPs_ReadPin(&GpioPs, KEY_S0);
		if((!Key_State)&& (Channel <= 1))
			Channel++;
		else if((!Key_State)&& (Channel > 1))
			Channel = 0;
		else
			Channel = Channel;
    }

    //再次检测到按键S1按下则频率计数器+1
    if(Key_Int_Num & 2){
		Key_State = XGpioPs_ReadPin(&GpioPs, KEY_S1);
		if((!Key_State)&&(F_Word_cnt <= 5)){
			F_Word_cnt++;
		}
		else{
			F_Word_cnt = F_Word_cnt;
		}
    }

    //再次检测到S2按下，则频率计数器-1
    if(Key_Int_Num & 4){
    	Key_State = XGpioPs_ReadPin(&GpioPs, KEY_S2);
		if((!Key_State)&& (F_Word_cnt > 1)){
			F_Word_cnt--;
		}
		else{
			F_Word_cnt = F_Word_cnt;
		}
    }

    //频率控制字
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

    //根据拨码开关SW1和SW0的组合值判断输出波形类型
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

    //如果再次检测S3按下，相位计数器+1
    if(Key_Int_Num & 16){
		Key_State = XGpioPs_ReadPin(&GpioPs, KEY_S3);
		if((!Key_State)&&(P_Word_cnt <= 7)){
			P_Word_cnt++;
		}
		else{
			P_Word_cnt = P_Word_cnt;
		}
    }

    //如果再次检测按下，相位计数器-1
    if(Key_Int_Num & 32){
    	Key_State = XGpioPs_ReadPin(&GpioPs, KEY_S4);
		if((!Key_State)&& (P_Word_cnt > 0)){
			P_Word_cnt--;
		}
		else{
			P_Word_cnt = P_Word_cnt;
		}
    }

    //相位偏移值
    if(P_Word_cnt == 0)
    	P_Word = 0;
    else if(P_Word_cnt == 1)
    	P_Word = 341;//（30/360）*4096=341，30°
    else if(P_Word_cnt == 2)
    	P_Word = 512;//45°
    else if(P_Word_cnt == 3)
    	P_Word = 1024;//90°
    else if(P_Word_cnt == 4)
    	P_Word = 1536;//135°
    else if(P_Word_cnt == 5)
    	P_Word = 2048;//180°
    else if(P_Word_cnt == 6)
    	P_Word = 2560;//225°
    else if(P_Word_cnt == 7)
    	P_Word = 3072;//270°
    else if(P_Word_cnt == 8)
    	P_Word = 3584;//315°

    /* ↑↑↑结束处理↑↑↑ */
    StatusEvent = XTtcPs_GetInterruptStatus(&TTC0_1_Timer);
    XTtcPs_ClearInterruptStatus(&TTC0_1_Timer, StatusEvent);
}

