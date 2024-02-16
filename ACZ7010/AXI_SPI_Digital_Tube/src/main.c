/**
  *****************************************************************************
  * 					使用AXI Quad IP核，控制8位数码管显示数字0~7
  *****************************************************************************
  *
  * @File   : main.c
  * @By     : Sun
  * @Version: V1.0
  * @Date   : 2022 / 06 / 08
  * @Shop	: https://xiaomeige.taobao.com/
  *
  *****************************************************************************
**/

//数码管编码
// 0	 1	  2	   3	4	 5	  6	   7	8	 9	  A	   b	C    d	  E    F    -
//0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x8C,0xBF,0xC6,0xA1,0x86,0xFF,0xbf

//数码管位选（左高右低）
//  7	 6	  5	   4	3	 2	  1	   0
//0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01

#include "COMMON.h"

uint8_t WriteBuffer[8][2] = {
							{0xC0,0x01}, //{'0',数码管第0位}
							{0xf9,0x02}, //{'1',数码管第1位}
							{0xA4,0x04}, //{'2',数码管第2位}
							{0xB0,0x08}, //{'3',数码管第3位}
							{0x99,0x10}, //{'4',数码管第4位}
							{0x92,0x20}, //{'5',数码管第5位}
							{0x82,0x40}, //{'6',数码管第6位}
							{0xF8,0x80}, //{'7',数码管第7位}
							};

int main(void)
{
	//数码管位选
	uint8_t Sel_Num = 0;

	//初始化通用中断控制器
	ScuGic_Init();

	//初始化AXI_SPI0，设为主机模式
	AXI_SPI_Init(&AXI_SPI0, XPAR_SPI_0_DEVICE_ID, XSP_MASTER_OPTION);

	//初始化私有定时器中断，定时间隔1ms
	ScuTimer_Int_Init(1000);

	while(1) {
		//刷新间隔由私有定时器中断控制，中断服务函数在ISR.c中
		if(Refresh_Flag) {

			//清除数码管刷新标志
			Refresh_Flag = 0;

			//SPI传输，控制第Sel_Num位数码管显示数字
			AXI_SPI_Transfer(&AXI_SPI0, 0, NULL, WriteBuffer[Sel_Num], 2);

			//位选循环(0~7)
			if(Sel_Num >= 7)
				Sel_Num = 0;
			else
				Sel_Num++;
		}
	}

	return 0;
}
