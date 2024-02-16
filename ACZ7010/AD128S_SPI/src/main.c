/**
  *****************************************************************************
  * 	ʹ�ð���S0��S1���л�ADC128S��8��ͨ���������������ʾ��⵽�ĵ�ѹֵ
  *****************************************************************************
  *
  * @File   : main.c
  * @By     : Sun
  * @Version: V1.1
  * @Date   : 2021 / 11 / 04
  * @Shop	: https://xiaomeige.taobao.com/
  * @Tag	: ����ADC128S���βɼ����շ�16�ֽڣ�SPIһ�δ���8�ֽڣ����ÿ�βɼ���Ҫ��������
  *****************************************************************************
**/

#include "stdio.h"
#include "sleep.h"
#include "PS_SPI.h"
#include "PS_GPIO.h"

//���ڴ洢����ܵı���ֵ������Ϊ0,1,2,3,4,5,6,7,8,9,-,.
uint8_t Seg_Code[12] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xBF,0x7F};

uint8_t Dig_Data[8][2] = {{0xC6,0x80},{0x89,0x40},{0xBF,0x20},{0xFF,0x10},
		{0x40,0x08},{0xC0,0x04},{0xC0,0x02},{0xC0,0x01}};//�����ֵ������ܱ���CH0 0.000
uint8_t ADC_Channel = 0;		//����ADCͨ��ΪINT0

int main()
{
	uint16_t i;
	uint8_t Set_Channel[2] = {0x00,0x00};	//�洢���͵�ͨ������8*2=16bit
	uint8_t ADC_Raw_Data[2];				//�洢��ȡ��ADCԭʼ����8*2=16bit
	uint32_t ADC_Acc_Val;					//�洢ADC�ۼ�ֵ
	float ADC_Val;							//ADC��ѹֵ

	ScuGic_Init();	//��ʼ��ͨ���жϿ�����
	//��ʼ��SPI0�豸����Ϊ����ģʽ,�͵�ƽ��Ч��Ƭѡ�źź󣬵ڶ���SCK��Ե��������Ч
	PS_SPI_Init(&SPI0, XPAR_XSPIPS_0_DEVICE_ID, XSPIPS_CLK_PRESCALE_16, XSPIPS_MASTER_OPTION
				| XSPIPS_CLK_PHASE_1_OPTION
				//| XSPIPS_CLK_ACTIVE_LOW_OPTION
				);
	//��ʼ��SPI1�豸����Ϊ����ģʽ
	PS_SPI_Init(&SPI1, XPAR_XSPIPS_1_DEVICE_ID, XSPIPS_CLK_PRESCALE_16, XSPIPS_MASTER_OPTION);
	PS_GPIO_Init();	//��ʼ��PS�˵�GPIO����
	PS_GPIO_SetMode(54, INPUT, 0);	//��ʼ��GPIO54������Ϊ����
	PS_GPIO_SetMode(55, INPUT, 0);	//��ʼ��GPIO55������Ϊ����

	ScuTimer_Int_Init(2000);	//��ʼ����ʱ��������Ϊ2000us������ʱ��

	while (1) {
		ADC_Acc_Val = 0;	//ADC�ۼ�ֵ����
		Set_Channel[0] = ADC_Channel << 3;	//ʹ��ͨ��ֵ�����趨ֵ

		//ADCԭʼ�����ۼ�1024��
		for (i = 0;i < 1024;i++) {
			PS_SPI_Transfer(&SPI0, 0, ADC_Raw_Data, Set_Channel, 2);	//��SPI��ADC128Sͨ�ţ��շ�2�ֽ�
			ADC_Acc_Val += ((ADC_Raw_Data[0] & 0x1F) << 8) + ADC_Raw_Data[1];	//ADC��ֵ�ۼ�
		}

		ADC_Val = ADC_Acc_Val / 1024 * 3.3 / 4095 ;	//���ۼ�ֵ����1024ȡƽ��ֵ��Ȼ��ʹ�ù�ʽ����Ϊ��ѹֵ

		Dig_Data[2][0] = Seg_Code[ADC_Channel];					//ͨ��ֵ
		Dig_Data[4][0] = Seg_Code[(int)ADC_Val] & 0x7F;			//��ѹ����λ
		Dig_Data[5][0] = Seg_Code[(int)(ADC_Val * 10) % 10];		//��ѹֵʮ��λ
		Dig_Data[6][0] = Seg_Code[(int)(ADC_Val * 100) % 10];	//��ѹֵ�ٷ�λ
		Dig_Data[7][0] = Seg_Code[(int)(ADC_Val * 1000) % 10];	//��ѹֵǧ��λ

		usleep(500000);	//ÿ500ms����һ����ֵ
	}
}
