#include "COMMON.h"
#include "xi2srx.h"
#include "xi2stx.h"

#define FS_KHZ 			48000		//音频采样率48KHz
#define MCLK_KHZ		18432000	//主时钟频率3*128*fs KHz
#define WM8960_ADDR		0x1A		//WM8960器件地址
#define IIC_SCLK_RATE	100000		//I2C通信速率

//WM8960写寄存器
#define	WM8960_Write_Reg(REG,DATA)	PS_IIC_Write_Reg(&IIC0,WM8960_ADDR,REG8,(REG<<1)|(DATA>>8), DATA&0xFF)

//驱动实例
XI2s_Rx I2s_Rx;
XI2s_Tx I2s_Tx;

int main()
{
	//配置定义
	XI2srx_Config *I2s_Rx_Config;
	XI2stx_Config *I2s_Tx_Config;

    //初始化 I2S Receiver
    I2s_Rx_Config = XI2s_Rx_LookupConfig(XPAR_XI2SRX_0_DEVICE_ID);
    XI2s_Rx_CfgInitialize(&I2s_Rx, I2s_Rx_Config, I2s_Rx_Config->BaseAddress);
    //设置 I2S Receiver 模块的位时钟BCLK速率
    XI2s_Rx_SetSclkOutDiv(&I2s_Rx, MCLK_KHZ, FS_KHZ);
    //使能 I2S Receiver 模块
    XI2s_Rx_Enable(&I2s_Rx, TRUE);

    //初始化 I2S Transmitter
    I2s_Tx_Config = XI2s_Tx_LookupConfig(XPAR_XI2STX_0_DEVICE_ID);
    XI2s_Tx_CfgInitialize(&I2s_Tx, I2s_Tx_Config, I2s_Tx_Config->BaseAddress);
    //设置 I2S Transmitter 模块的位时钟BCLK速率
    XI2s_Tx_SetSclkOutDiv(&I2s_Tx, MCLK_KHZ, FS_KHZ);
    //使能 I2S Transmitter 模块
    XI2s_Tx_Enable(&I2s_Tx,TRUE);

    //IIC初始化
    PS_IIC_Init(&IIC0,XPAR_XIICPS_0_DEVICE_ID,100000);

    //配置WM8960寄存器
    WM8960_Write_Reg(0x0F,0x0000);	//复位所有寄存器
    WM8960_Write_Reg(0x19,0x00FC);	//设置VMID为快速启动，使能VREF、AINL、AINR、ADCL、ADCR
    WM8960_Write_Reg(0x1A,0x01E1);	//使能DACL、DACR、LOUT1、ROUT1、SPKL、SPKR、PLL
    WM8960_Write_Reg(0x2F,0x000C);	//使能LOMIX和ROMIX
    WM8960_Write_Reg(0x22,0x0100);	//使能Left DAC To Left output Mixer
    WM8960_Write_Reg(0x25,0x0100);	//使能Right DAC To Right output Mixer
    WM8960_Write_Reg(0x05,0x0000);	//关闭ADC高通滤波器、去加重、DAC静音、ADC极性、DAC6db减小
    WM8960_Write_Reg(0x02,0x0179);  //LOUT1音量，bit[6:0]控制耳机音量大小，bit8设置为立即更新音量
    WM8960_Write_Reg(0x03,0x0179);	//ROUT1音量，bit[6:0]控制耳机音量大小，bit8设置为立即更新音量
    WM8960_Write_Reg(0x2B,0x0050);	//使能LINPUT3，增益0db，禁用LINPUT2
    WM8960_Write_Reg(0x2C,0x000A);	//使能RINPUT2，增益0db，禁用RINPUT3
    WM8960_Write_Reg(0x07,0x000A);	//音频格式设置为I2S，从模式，24位宽，DSP工作于A模式，正常输出
    WM8960_Write_Reg(0x04,0x0005);	//设置ADC和DAC采样率为48K，设置SYSCLKDIV值为2，SYSCLK由PLL输出
    WM8960_Write_Reg(0x34,0x0028);	//设置PLL为小数模式，设置PLLPRESCALE值为2，设置PLLN值为8

    return 0;
}
