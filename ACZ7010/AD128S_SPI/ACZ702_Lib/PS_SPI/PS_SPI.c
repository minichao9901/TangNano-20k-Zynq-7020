/**
  *****************************************************************************
  * 						PS部分SPI相关库
  *****************************************************************************
  *
  * @File   : PS_SPI.c
  * @By     : Sun
  * @Version: V1.2
  * @Date   : 2021 / 11 / 04
  * @Shop	: https://xiaomeige.taobao.com/
  *
  *****************************************************************************
**/


#include "PS_SPI.h"


XSpiPs SPI0,SPI1;


/************************************************************************************************************
**  @brief	初始化PS_SPI设备
**  @param	SpiInstancePtr	SPI对象
**  @param	SpiDeviceId		SPI设备ID
**  @param	SPI_Mode		SPI模式选择
**  @param	Prescaler		分频系数xx				XSPIPS_CLK_PRESCALE_xx
** 	模式有以下几种：	XSPIPS_MASTER_OPTION			设置为主机（默认为从机）
** 					XSPIPS_CLK_ACTIVE_LOW_OPTION	将时钟极性设为低电平有效（默认为高电平有效）
** 					XSPIPS_CLK_PHASE_1_OPTION		片选信号后，第二个SCK边缘的数据有效（默认为第一个边缘有效）
** 					XSPIPS_DECODE_SSELECT_OPTION	设置为8位从机模式，使用3位2进制数来选择从机（默认为3位从机模式）
** 					XSPIPS_FORCE_SSELECT_OPTION		手动片选模式
** 					XSPIPS_MANUAL_START_OPTION		设置为手动开始模式
**	Sample:	//初始化SPI设备，设为主机模式，64分频
**			PS_SPI_Init(&SPI0, XPAR_XSPIPS_0_DEVICE_ID,
**				XSPIPS_CLK_PRESCALE_64, XSPIPS_MASTER_OPTION);
************************************************************************************************************/
void PS_SPI_Init(XSpiPs *SpiInstPtr,uint16_t DeviceId, uint8_t Prescaler, uint8_t SPI_Mode)
{
	XSpiPs_Config *SpiConfig;

	//根据设备ID在配置表中查找配置，然后初始化配置
	SpiConfig = XSpiPs_LookupConfig(DeviceId);
	XSpiPs_CfgInitialize(SpiInstPtr, SpiConfig, SpiConfig->BaseAddress);

	//设置SPI的模式
	XSpiPs_SetOptions(SpiInstPtr, SPI_Mode);

	//设置SPI的分频并使能
	XSpiPs_SetClkPrescaler(SpiInstPtr, Prescaler);
	XSpiPs_Enable(SpiInstPtr);
}

/************************************************************************************************************
**	@brief	使用SPI进行主从之间的数据传输
**	@param	SpiInstancePtr	SPI对象
**	@param	Sel_Num			选择从设备的序号，普通模式只能选3个设备（0~2）
**	@param	ReadBuffer		接受的数据所存储的数组
**	@param	WriteBuffer		发送的数据所存储的数组
**	@param	BUFFER_SIZE		接收与发送的数据的字节数
**	Sample:	PS_SPI_Transfer(&SPI0, 0, ReadBuffer, WriteBuffer, 2);//用SPI与0号从机通信，收发2字节
************************************************************************************************************/
void PS_SPI_Transfer(XSpiPs *SpiInstPtr, uint32_t Sel_Num, uint8_t *ReadBuffer, uint8_t *WriteBuffer, uint32_t BUFFER_SIZE)
{
	//选择片选设备，一共3位
	XSpiPs_SetSlaveSelect(SpiInstPtr, Sel_Num);

	//与从机进行数据传输，每发送一个字节就接受一个字节，发送的数据为WriteBuffer里的内容，接收的数据存在ReadBuffer里
	XSpiPs_PolledTransfer(SpiInstPtr, WriteBuffer, ReadBuffer, BUFFER_SIZE);
}
