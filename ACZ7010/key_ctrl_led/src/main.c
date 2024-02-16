/*
 * 使用PS_GPIO实现了按键控制LED功能:
 * PS端按键按下→PL端的灯亮起，松开熄灭
 * PL端按键按下→PS端的灯亮起，松开熄灭
*/
#include "COMMON.h"

int main(void)
{
    uint8_t State;   //存放按键(MIO47)的电平状态，0为低电平，1为高电平

    PS_GPIO_Init(); //初始化PS端MIO和EMIO
    //设置PS_LED(MIO7)为输出并且初始为低电平
    PS_GPIO_SetMode(PS_LED, OUTPUT, 0);
    //设置PL_LED(EMIO0)为输出并且初始为低电平
    PS_GPIO_SetMode(PL_LED, OUTPUT, 0);
    PS_GPIO_SetMode(PS_KEY, INPUT, 0); //设置PS_KEY(MIO47)方向为输入
    PS_GPIO_SetMode(PL_KEY, INPUT, 0); //设置PL_KEY(EMIO1)方向为输入

    while(1)
    {
        State = PS_GPIO_GetPort(PS_KEY);   //读取PS_KEY的电平值并存储到State变量里
        PS_GPIO_SetPort(PL_LED,!State);    //将State变量的值取非赋予PL_LED来输出
        State = PS_GPIO_GetPort(PL_KEY);   //读取PL_KEY的电平值并存储到State变量里
        PS_GPIO_SetPort(PS_LED,!State);    //将State变量的值取非赋予PS_LED来输出
    }
    return 0;
}
