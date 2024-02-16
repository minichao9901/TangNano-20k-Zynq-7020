#include "COMMON.h"
#include "xi2srx.h"
#include "xi2stx.h"

#define FS_KHZ 			48000		//��Ƶ������48KHz
#define MCLK_KHZ		18432000	//��ʱ��Ƶ��3*128*fs KHz
#define WM8960_ADDR		0x1A		//WM8960������ַ
#define IIC_SCLK_RATE	100000		//I2Cͨ������

//WM8960д�Ĵ���
#define	WM8960_Write_Reg(REG,DATA)	PS_IIC_Write_Reg(&IIC0,WM8960_ADDR,REG8,(REG<<1)|(DATA>>8), DATA&0xFF)

//����ʵ��
XI2s_Rx I2s_Rx;
XI2s_Tx I2s_Tx;

int main()
{
	//���ö���
	XI2srx_Config *I2s_Rx_Config;
	XI2stx_Config *I2s_Tx_Config;

    //��ʼ�� I2S Receiver
    I2s_Rx_Config = XI2s_Rx_LookupConfig(XPAR_XI2SRX_0_DEVICE_ID);
    XI2s_Rx_CfgInitialize(&I2s_Rx, I2s_Rx_Config, I2s_Rx_Config->BaseAddress);
    //���� I2S Receiver ģ���λʱ��BCLK����
    XI2s_Rx_SetSclkOutDiv(&I2s_Rx, MCLK_KHZ, FS_KHZ);
    //ʹ�� I2S Receiver ģ��
    XI2s_Rx_Enable(&I2s_Rx, TRUE);

    //��ʼ�� I2S Transmitter
    I2s_Tx_Config = XI2s_Tx_LookupConfig(XPAR_XI2STX_0_DEVICE_ID);
    XI2s_Tx_CfgInitialize(&I2s_Tx, I2s_Tx_Config, I2s_Tx_Config->BaseAddress);
    //���� I2S Transmitter ģ���λʱ��BCLK����
    XI2s_Tx_SetSclkOutDiv(&I2s_Tx, MCLK_KHZ, FS_KHZ);
    //ʹ�� I2S Transmitter ģ��
    XI2s_Tx_Enable(&I2s_Tx,TRUE);

    //IIC��ʼ��
    PS_IIC_Init(&IIC0,XPAR_XIICPS_0_DEVICE_ID,100000);

    //����WM8960�Ĵ���
    WM8960_Write_Reg(0x0F,0x0000);	//��λ���мĴ���
    WM8960_Write_Reg(0x19,0x00FC);	//����VMIDΪ����������ʹ��VREF��AINL��AINR��ADCL��ADCR
    WM8960_Write_Reg(0x1A,0x01E1);	//ʹ��DACL��DACR��LOUT1��ROUT1��SPKL��SPKR��PLL
    WM8960_Write_Reg(0x2F,0x000C);	//ʹ��LOMIX��ROMIX
    WM8960_Write_Reg(0x22,0x0100);	//ʹ��Left DAC To Left output Mixer
    WM8960_Write_Reg(0x25,0x0100);	//ʹ��Right DAC To Right output Mixer
    WM8960_Write_Reg(0x05,0x0000);	//�ر�ADC��ͨ�˲�����ȥ���ء�DAC������ADC���ԡ�DAC6db��С
    WM8960_Write_Reg(0x02,0x0179);  //LOUT1������bit[6:0]���ƶ���������С��bit8����Ϊ������������
    WM8960_Write_Reg(0x03,0x0179);	//ROUT1������bit[6:0]���ƶ���������С��bit8����Ϊ������������
    WM8960_Write_Reg(0x2B,0x0050);	//ʹ��LINPUT3������0db������LINPUT2
    WM8960_Write_Reg(0x2C,0x000A);	//ʹ��RINPUT2������0db������RINPUT3
    WM8960_Write_Reg(0x07,0x000A);	//��Ƶ��ʽ����ΪI2S����ģʽ��24λ��DSP������Aģʽ���������
    WM8960_Write_Reg(0x04,0x0005);	//����ADC��DAC������Ϊ48K������SYSCLKDIVֵΪ2��SYSCLK��PLL���
    WM8960_Write_Reg(0x34,0x0028);	//����PLLΪС��ģʽ������PLLPRESCALEֵΪ2������PLLNֵΪ8

    return 0;
}
