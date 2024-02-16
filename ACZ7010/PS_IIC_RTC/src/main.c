/**
  *****************************************************************************
  * 使用PS端的IIC外设实现RTC功能,使用数码管显示时间和日期，拨码开关切换显示模式
  *****************************************************************************
  *
  * @File   : main.c
  * @By     : Sun
  * @Version: V1.1
  * @Date   : 2022 / 06 / 09
  * @Shop	: https://xiaomeige.taobao.com/
  *
  *****************************************************************************
**/

#include "COMMON.h"

//用于存储数码管的段选编码值，依次为0,1,2,3,4,5,6,7,8,9,-,.
uint8_t Seg_Code[12] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xBF,0x7F};

//数码管显示对应的数组
uint8_t TubeBuffer[8][2] = {{0,0x01}, 	//数码管第0位
							{0,0x02}, 	//数码管第1位
							{0,0x04}, 	//数码管第2位
							{0,0x08}, 	//数码管第3位
							{0,0x10}, 	//数码管第4位
							{0,0x20}, 	//数码管第5位
							{0,0x40}, 	//数码管第6位
							{0,0x80}};	//数码管第7位

RTC_TypeDef Time;	//存储时间数据

int main(void)
{
	uint8_t Sel_Num = 0;	//数码管位选
	uint8_t Mode;			//显示模式

	//初始化通用中断控制器
	ScuGic_Init();

	//初始化IIC0
	PS_IIC_Init(&IIC0,XPAR_XIICPS_0_DEVICE_ID,100000);

	//初始化SPI0
	PS_SPI_Init(&SPI0,XPAR_XSPIPS_0_DEVICE_ID,
			XSPIPS_CLK_PRESCALE_64,XSPIPS_MASTER_OPTION);

	//初始化PS端GPIO
	PS_GPIO_Init();

	//初始化RTC
	RTC_Init();

	//设置时间为23点59分55秒
	Set_Clock(23,59,55);

	//设置日期为2021年12月31日，星期五
	Set_Date(2021,12,31,5);

	//将GPIO54设置为输入
	PS_GPIO_SetMode(54, INPUT, 0);

	//初始化私有定时器中断，定时间隔1ms
	ScuTimer_Int_Init(1000);

	while(1)
	{
		//RTC读取刷新间隔由私有定时器中断控制，中断服务函数在ISR.c中
		if(RTC_Refresh_Flag || (DISPLAY_MODE != Mode)) {
			//清除RTC刷新标志
			RTC_Refresh_Flag = 0;
			//获取时间
			RTC_Get_Time();
			//模式检测
			Mode = DISPLAY_MODE;

			if (Mode) {
				//使数码管可显示当前时间。格式为 XX-XX-XX
				Time_Seg_Code(&Time,TubeBuffer[0]);
			} else {
				//使数码管可显示当前日期。格式为 20XX.XX.XX.
				Date_Seg_Code(&Time,TubeBuffer[0]);
			}
		}

		//刷新间隔由私有定时器中断控制，中断服务函数在ISR.c中
		if(Tube_Refresh_Flag) {
			//清除数码管刷新标志
			Tube_Refresh_Flag = 0;

			//SPI传输，控制第Sel_Num位数码管显示数字
			PS_SPI_Transfer(&SPI0, 0, NULL, TubeBuffer[Sel_Num], 2);

			//位选循环(0~7)
			if(Sel_Num >= 7)
				Sel_Num = 0;
			else
				Sel_Num++;
		}
	}

	return 0;
}
