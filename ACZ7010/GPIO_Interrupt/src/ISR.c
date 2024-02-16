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
volatile uint32_t Key_Int_Num = 0;//按键中断编号标识
volatile uint32_t Key_Cnt0 = 0,Key_Cnt1 = 0;//按键计数

/**
  *****************************************************
  * @brief	私有定时器中断处理程序
  * @tag	本函数用来处理私有定时器中断，在内部加入用户程序即可
  *****************************************************
**/
void ScuTimer_IRQ_Handler(void *CallBackRef)
{
	/* ↓↓↓用户处理↓↓↓ */


	/* ↑↑↑结束处理↑↑↑ */
    XScuTimer_ClearInterruptStatus(&ScuTimer);
}

void PS_GPIO_IRQ_Handler(void *CallBackRef, uint32_t Bank, uint32_t Status)
{
    uint32_t Int_State;

    Int_State = XGpioPs_IntrGetStatusPin(&GpioPs, PS_KEY);
    if(Int_State) {
        TTC_Tick_Start(&TTC0_1_Timer);//开启定时器，延时10ms
        Key_Int_Num |= 1;
    }
}

//GPIO的中断处理程序,只要GPIO通道0有电平变化就会进入该中断
void AXI_GPIO_IRQ_Handler(void *CallbackRef)
{
    uint32_t Key_Int_State;

    //判断哪一位出现中断
    Key_Int_State = XGpio_InterruptGetStatus(&AXI_GPIO0);

    /* ↓↓↓用户处理↓↓↓ */
        if(Key_Int_State & (1<<KEY_S1)) {

                //按键消抖，开启TTC0_1计数
                TTC_Tick_Start(&TTC0_1_Timer);
                Key_Int_Num |= 2;
        }
    /* ↑↑↑结束处理↑↑↑ */

    //清除中断标志
    XGpio_InterruptClear(&AXI_GPIO0, XGPIO_IR_CH1_MASK);
}

//TTC0的1号计数器的中断处理函数
void TTC0_1_IRQ_Handler(void *CallBackRef)
{
    uint32_t StatusEvent;
    /* ↓↓↓用户处理↓↓↓ */
        uint32_t Key_State;

        //关闭TTC0_1计数
        TTC_Tick_Stop(&TTC0_1_Timer);

        if(Key_Int_Num & 1) {
            //判断按键情况
            Key_State = XGpioPs_ReadPin(&GpioPs, PS_KEY);
            if(!Key_State) {//低电平表示按下
                Key_Cnt0++;
                LOGW("PS Key Press!\n");
                LOGW("Cnt0: %lu\n",Key_Cnt0);

            } else if(Key_State) {//高电平表示松手
                LOGW("PS Key Release!\n");
            }
            Key_Int_Num ^= 1;
        }

        if(Key_Int_Num & 2) {
            //读取按键S1的值
            Key_State = AXI_GPIO_GetPin(&AXI_GPIO0,XGPIO_IR_CH1_MASK,0);
            if(!Key_State) {//低电平表示按下
                Key_Cnt1++;
                LOGW("AXI Key Press!\n");
                LOGW("Cnt1: %lu\n",Key_Cnt1);
            } else if(Key_State){
                LOGW("AXI Key Release!\n");
            }
            Key_Int_Num ^= 2;
        }

    /* ↑↑↑结束处理↑↑↑ */
    StatusEvent = XTtcPs_GetInterruptStatus(&TTC0_1_Timer);
    XTtcPs_ClearInterruptStatus(&TTC0_1_Timer, StatusEvent);
}
