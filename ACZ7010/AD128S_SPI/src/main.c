/**
  *****************************************************************************
  * 	使用按键S0和S1，切换ADC128S的8个通道，并在数码管显示检测到的电压值
  *****************************************************************************
  *
  * @File   : main.c
  * @By     : Sun
  * @Version: V1.1
  * @Date   : 2021 / 11 / 04
  * @Shop	: https://xiaomeige.taobao.com/
  * @Tag	: 由于ADC128S单次采集需收发16字节，SPI一次传输8字节，因此每次采集需要传输两次
  *****************************************************************************
**/

#include "stdio.h"
#include "sleep.h"
#include "PS_SPI.h"
#include "PS_GPIO.h"

//用于存储数码管的编码值，依次为0,1,2,3,4,5,6,7,8,9,-,.
uint8_t Seg_Code[12] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xBF,0x7F};

uint8_t Dig_Data[8][2] = {{0xC6,0x80},{0x89,0x40},{0xBF,0x20},{0xFF,0x10},
		{0x40,0x08},{0xC0,0x04},{0xC0,0x02},{0xC0,0x01}};//赋予初值，数码管编码CH0 0.000
uint8_t ADC_Channel = 0;		//设置ADC通道为INT0

int main()
{
	uint16_t i;
	uint8_t Set_Channel[2] = {0x00,0x00};	//存储发送的通道数据8*2=16bit
	uint8_t ADC_Raw_Data[2];				//存储读取的ADC原始数据8*2=16bit
	uint32_t ADC_Acc_Val;					//存储ADC累加值
	float ADC_Val;							//ADC电压值

	ScuGic_Init();	//初始化通用中断控制器
	//初始化SPI0设备，设为主机模式,低电平有效，片选信号后，第二个SCK边缘的数据有效
	PS_SPI_Init(&SPI0, XPAR_XSPIPS_0_DEVICE_ID, XSPIPS_CLK_PRESCALE_16, XSPIPS_MASTER_OPTION
				| XSPIPS_CLK_PHASE_1_OPTION
				//| XSPIPS_CLK_ACTIVE_LOW_OPTION
				);
	//初始化SPI1设备，设为主机模式
	PS_SPI_Init(&SPI1, XPAR_XSPIPS_1_DEVICE_ID, XSPIPS_CLK_PRESCALE_16, XSPIPS_MASTER_OPTION);
	PS_GPIO_Init();	//初始化PS端的GPIO外设
	PS_GPIO_SetMode(54, INPUT, 0);	//初始化GPIO54，设置为输入
	PS_GPIO_SetMode(55, INPUT, 0);	//初始化GPIO55，设置为输入

	ScuTimer_Int_Init(2000);	//初始化定时器，设置为2000us启动定时器

	while (1) {
		ADC_Acc_Val = 0;	//ADC累加值清零
		Set_Channel[0] = ADC_Channel << 3;	//使用通道值计算设定值

		//ADC原始数据累加1024次
		for (i = 0;i < 1024;i++) {
			PS_SPI_Transfer(&SPI0, 0, ADC_Raw_Data, Set_Channel, 2);	//用SPI与ADC128S通信，收发2字节
			ADC_Acc_Val += ((ADC_Raw_Data[0] & 0x1F) << 8) + ADC_Raw_Data[1];	//ADC数值累加
		}

		ADC_Val = ADC_Acc_Val / 1024 * 3.3 / 4095 ;	//将累加值除以1024取平均值，然后使用公式换算为电压值

		Dig_Data[2][0] = Seg_Code[ADC_Channel];					//通道值
		Dig_Data[4][0] = Seg_Code[(int)ADC_Val] & 0x7F;			//电压整数位
		Dig_Data[5][0] = Seg_Code[(int)(ADC_Val * 10) % 10];		//电压值十分位
		Dig_Data[6][0] = Seg_Code[(int)(ADC_Val * 100) % 10];	//电压值百分位
		Dig_Data[7][0] = Seg_Code[(int)(ADC_Val * 1000) % 10];	//电压值千分位

		usleep(500000);	//每500ms更新一次数值
	}
}
