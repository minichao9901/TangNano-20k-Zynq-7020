/**
  *****************************************************************************
  * 	使用拓展板上按键S0切换通道，按键S1和S2切换频率，开关SW0和SW1切换波形，当前频率、相位和通道可以在数码管中显示
  *****************************************************************************
  * @File   : main.c
  * @By     : huang
  * @Version: V1.1
  * @Date   : 2021 / 11 / 04
  * @Shop	: https://xiaomeige.taobao.com/
  * @Tag	:
  *****************************************************************************
**/

#include "SinWavePoint_12bit.h"
#include "COMMON.h"

//用于存储数码管的编码值，依次为0,1,2,3,4,5,6,7,8,9,-,.
uint8_t Seg_Display[12] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xBF,0x7F};

uint8_t Dig_Data[8] = {0xC0,0xBF,0xC0,0xBF,0xC0,0xBF,0xC0,0xC6};//赋予初值，数码管编码C0-0-0-0(数码管是从右往左)

uint16_t DAC_CH_CtrlWord;

unsigned short SqurWavePoint[4096],TraWavePoint[4096],*Wave_Point;

int main()
{
	uint32_t ControlReg;
	uint16_t i;
	//初始化通用中断控制器
	ScuGic_Init();
	//初始化AXI SPI0设备，设为主机模式,低电平有效，片选信号后，第一个SCK边缘的数据有效
	AXI_SPI_Init(&AXI_SPI0, XPAR_SPI_0_DEVICE_ID, XSP_CLK_ACTIVE_LOW_OPTION |XSP_MASTER_OPTION);

	//选择AXI_SPI0片选设备，一共32位，1在哪一位就选择哪一个为片选设备
	XSpi_SetSlaveSelect(&AXI_SPI0, 1);
	XSpi_SetSlaveSelectReg(&AXI_SPI0,AXI_SPI0.SlaveSelectReg);

	/* 取消AXI_SPI0抑制，启动传输 */
	ControlReg = XSpi_GetControlReg(&AXI_SPI0);
	ControlReg &= ~XSP_CR_TRANS_INHIBIT_MASK;
	XSpi_SetControlReg(&AXI_SPI0, ControlReg);



	PS_GPIO_Init();	//初始化PS端的GPIO外设
	PS_GPIO_SetMode(KEY_S0, INPUT, 0);	//初始化GPIO54，设置为输入，通道选择按键
	PS_GPIO_SetMode(KEY_S1, INPUT, 0);	//初始化GPIO55，设置为输入，频率+按键
	PS_GPIO_SetMode(KEY_S2, INPUT, 0);	//初始化GPIO56，设置为输入，频率－按键
	PS_GPIO_SetMode(KEY_S3, INPUT, 0);	//初始化GPIO57，设置为输入，相位+按键
	PS_GPIO_SetMode(KEY_S4, INPUT, 0);	//初始化GPIO58，设置为输入，相位－按键
	PS_GPIO_SetMode(SW0, INPUT, 0);	//初始化GPIO57，设置为输入，波形控制拨码开关
	PS_GPIO_SetMode(SW1, INPUT, 0);	//初始化GPIO58，设置为输入，波形控制拨码开关

	PS_GPIO_Int_Init();//初始化PS GPIO中断

	//设置引脚中断模式
	PS_GPIO_SetInt(KEY_S0, XGPIOPS_IRQ_TYPE_EDGE_FALLING);
	PS_GPIO_SetInt(KEY_S1, XGPIOPS_IRQ_TYPE_EDGE_FALLING);
	PS_GPIO_SetInt(KEY_S2, XGPIOPS_IRQ_TYPE_EDGE_FALLING);
	PS_GPIO_SetInt(KEY_S3, XGPIOPS_IRQ_TYPE_EDGE_FALLING);
	PS_GPIO_SetInt(KEY_S4, XGPIOPS_IRQ_TYPE_EDGE_FALLING);
	PS_GPIO_SetInt(SW0, XGPIOPS_IRQ_TYPE_EDGE_BOTH);
	PS_GPIO_SetInt(SW1, XGPIOPS_IRQ_TYPE_EDGE_BOTH);

	//初始化TTC_1，每10ms触发一次中断，用于延时消抖
	TTC_Tick_Init(&TTC0_1_Timer, XPAR_XTTCPS_1_DEVICE_ID, XPS_TTC0_1_INT_ID, 10000, TTC0_1_IRQ_Handler);

	//波形数据
	for(i=0;i<2048;i++)
		SqurWavePoint[i] = 0;
	for (i=2047;i<4096;i++)
		SqurWavePoint[i] = 4095;

	for (i=0;i<4096;i++)
		TraWavePoint[i] = i;

	ScuTimer_Int_Init(2.441);	//初始化定时器，设置为2.441us
	DT8_Display_Ctr((0xFF));//数码管显示开关

	while (1) {
		if(!DAC_Wave)
			Wave_Point = SinWavePoint;
		else if(DAC_Wave ==1)
			Wave_Point = SqurWavePoint;
		else if(DAC_Wave ==2)
			Wave_Point = TraWavePoint;

		if (!Channel)
			DAC_CH_CtrlWord = CHB_CtrlWord;
		else if (Channel == 1){
			DAC_CH_CtrlWord = CHA_CtrlWord;}

		//数码管对应位显示值，从右到左
		Dig_Data[6] = Seg_Display[Channel];//第6位显示通道
		Dig_Data[4] = Seg_Display[DAC_Wave];//第4位显示波形
		Dig_Data[2] = Seg_Display[P_Word_cnt];//第2位显示相位
		Dig_Data[0] = Seg_Display[F_Word_cnt];//第0位显示频率

		//数码管显示
		for(i=0;i<8;i++)
			Set_DT8_Display_Val(i, Dig_Data[i]);
	}
}

