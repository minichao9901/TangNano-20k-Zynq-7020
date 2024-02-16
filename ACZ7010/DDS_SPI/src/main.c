/**
  *****************************************************************************
  * 	ʹ����չ���ϰ���S0�л�ͨ��������S1��S2�л�Ƶ�ʣ�����SW0��SW1�л����Σ���ǰƵ�ʡ���λ��ͨ�����������������ʾ
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

//���ڴ洢����ܵı���ֵ������Ϊ0,1,2,3,4,5,6,7,8,9,-,.
uint8_t Seg_Display[12] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xBF,0x7F};

uint8_t Dig_Data[8] = {0xC0,0xBF,0xC0,0xBF,0xC0,0xBF,0xC0,0xC6};//�����ֵ������ܱ���C0-0-0-0(������Ǵ�������)

uint16_t DAC_CH_CtrlWord;

unsigned short SqurWavePoint[4096],TraWavePoint[4096],*Wave_Point;

int main()
{
	uint32_t ControlReg;
	uint16_t i;
	//��ʼ��ͨ���жϿ�����
	ScuGic_Init();
	//��ʼ��AXI SPI0�豸����Ϊ����ģʽ,�͵�ƽ��Ч��Ƭѡ�źź󣬵�һ��SCK��Ե��������Ч
	AXI_SPI_Init(&AXI_SPI0, XPAR_SPI_0_DEVICE_ID, XSP_CLK_ACTIVE_LOW_OPTION |XSP_MASTER_OPTION);

	//ѡ��AXI_SPI0Ƭѡ�豸��һ��32λ��1����һλ��ѡ����һ��ΪƬѡ�豸
	XSpi_SetSlaveSelect(&AXI_SPI0, 1);
	XSpi_SetSlaveSelectReg(&AXI_SPI0,AXI_SPI0.SlaveSelectReg);

	/* ȡ��AXI_SPI0���ƣ��������� */
	ControlReg = XSpi_GetControlReg(&AXI_SPI0);
	ControlReg &= ~XSP_CR_TRANS_INHIBIT_MASK;
	XSpi_SetControlReg(&AXI_SPI0, ControlReg);



	PS_GPIO_Init();	//��ʼ��PS�˵�GPIO����
	PS_GPIO_SetMode(KEY_S0, INPUT, 0);	//��ʼ��GPIO54������Ϊ���룬ͨ��ѡ�񰴼�
	PS_GPIO_SetMode(KEY_S1, INPUT, 0);	//��ʼ��GPIO55������Ϊ���룬Ƶ��+����
	PS_GPIO_SetMode(KEY_S2, INPUT, 0);	//��ʼ��GPIO56������Ϊ���룬Ƶ�ʣ�����
	PS_GPIO_SetMode(KEY_S3, INPUT, 0);	//��ʼ��GPIO57������Ϊ���룬��λ+����
	PS_GPIO_SetMode(KEY_S4, INPUT, 0);	//��ʼ��GPIO58������Ϊ���룬��λ������
	PS_GPIO_SetMode(SW0, INPUT, 0);	//��ʼ��GPIO57������Ϊ���룬���ο��Ʋ��뿪��
	PS_GPIO_SetMode(SW1, INPUT, 0);	//��ʼ��GPIO58������Ϊ���룬���ο��Ʋ��뿪��

	PS_GPIO_Int_Init();//��ʼ��PS GPIO�ж�

	//���������ж�ģʽ
	PS_GPIO_SetInt(KEY_S0, XGPIOPS_IRQ_TYPE_EDGE_FALLING);
	PS_GPIO_SetInt(KEY_S1, XGPIOPS_IRQ_TYPE_EDGE_FALLING);
	PS_GPIO_SetInt(KEY_S2, XGPIOPS_IRQ_TYPE_EDGE_FALLING);
	PS_GPIO_SetInt(KEY_S3, XGPIOPS_IRQ_TYPE_EDGE_FALLING);
	PS_GPIO_SetInt(KEY_S4, XGPIOPS_IRQ_TYPE_EDGE_FALLING);
	PS_GPIO_SetInt(SW0, XGPIOPS_IRQ_TYPE_EDGE_BOTH);
	PS_GPIO_SetInt(SW1, XGPIOPS_IRQ_TYPE_EDGE_BOTH);

	//��ʼ��TTC_1��ÿ10ms����һ���жϣ�������ʱ����
	TTC_Tick_Init(&TTC0_1_Timer, XPAR_XTTCPS_1_DEVICE_ID, XPS_TTC0_1_INT_ID, 10000, TTC0_1_IRQ_Handler);

	//��������
	for(i=0;i<2048;i++)
		SqurWavePoint[i] = 0;
	for (i=2047;i<4096;i++)
		SqurWavePoint[i] = 4095;

	for (i=0;i<4096;i++)
		TraWavePoint[i] = i;

	ScuTimer_Int_Init(2.441);	//��ʼ����ʱ��������Ϊ2.441us
	DT8_Display_Ctr((0xFF));//�������ʾ����

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

		//����ܶ�Ӧλ��ʾֵ�����ҵ���
		Dig_Data[6] = Seg_Display[Channel];//��6λ��ʾͨ��
		Dig_Data[4] = Seg_Display[DAC_Wave];//��4λ��ʾ����
		Dig_Data[2] = Seg_Display[P_Word_cnt];//��2λ��ʾ��λ
		Dig_Data[0] = Seg_Display[F_Word_cnt];//��0λ��ʾƵ��

		//�������ʾ
		for(i=0;i<8;i++)
			Set_DT8_Display_Val(i, Dig_Data[i]);
	}
}

