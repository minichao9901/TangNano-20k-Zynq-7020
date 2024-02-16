/**
  *****************************************************************************
  * 						AXI_QUAD_SPI相关库
  *****************************************************************************
  *
  * @File   : AXI_SPI.c
  * @By     : Sun
  * @Version: V1.3
  * @Date   : 2021 / 11 / 04
  * @Shop	: https://xiaomeige.taobao.com/
  *
  *****************************************************************************
**/


#include "AXI_SPI.h"
#include "COMMON.h"


XSpi AXI_SPI0;


/************************************************************************************************************
**  @brief	初始化PL_SPI设备
**  @param	SpiInstPtr		SPI对象
**  @param	SpiDeviceId		SPI设备ID
**  @param	SPI_Mode		SPI模式选择
** 	模式有以下几种：XSP_MASTER_OPTION				设置为主机（默认为从机）
** 					XSP_CLK_ACTIVE_LOW_OPTION		将时钟极性设为低电平有效（默认为高电平有效）
** 					XSP_CLK_PHASE_1_OPTION			片选信号后，第二个SCK边缘的数据有效（默认为第一个边缘有效）
** 					XSP_LOOPBACK_OPTION				设置为回环模式（默认为标准模式）
** 					XSP_MANUAL_SSELECT_OPTION		设置为手动片选模式（默认为自动片选模式）
**	Sample usage:	AXI_SPI_Init(&AXI_SPI0, SPI_DEVICE_ID, XSP_MASTER_OPTION);//初始化SPI设备，设为主机模式
************************************************************************************************************/
void AXI_SPI_Init(XSpi *SpiInstPtr, uint16_t SpiDeviceId,uint8_t SPI_Mode)
{
	XSpi_Config *ConfigPtr;

	//初始化SPI驱动程序
	ConfigPtr = XSpi_LookupConfig(SpiDeviceId);
	XSpi_CfgInitialize(SpiInstPtr, ConfigPtr,ConfigPtr->BaseAddress);

	//设置SPI的模式
	XSpi_SetOptions(SpiInstPtr, SPI_Mode);

	//启动SPI驱动程序
	XSpi_Start(SpiInstPtr);

	//禁用全局中断以使用轮询模式操作
	XSpi_IntrGlobalDisable(SpiInstPtr);
}

/************************************************************************************************************
**	@brief	使用SPI进行主从之间的数据传输
**	@param	SpiInstPtr		SPI对象指针
**	@param	Sel_Num			从设备的编号，0~31（如果被设置为从设备，则此处无效）
**	@param	ReadBuffer		接受的数据所存储的数组
**	@param	WriteBuffer		发送的数据所存储的数组
**	@param	BUFFER_SIZE		接收与发送的数据的字节数
**	Sample usage:			//用SPI与0号从机通信，收发2字节
**							AXI_SPI_Transfer(&AXI_SPI0, 0, ReadBuffer, WriteBuffer, 2);
************************************************************************************************************/
void AXI_SPI_Transfer(XSpi *SpiInstPtr, uint32_t Sel_Num, uint8_t *ReadBuffer, uint8_t *WriteBuffer, uint32_t BUFFER_SIZE)
{
	//选择片选设备，一共32位，1在哪一位就选择哪一个为片选设备
	XSpi_SetSlaveSelect(SpiInstPtr, 1 << Sel_Num);

	//与从机进行数据传输，每发送一个字节就接受一个字节，发送的数据为WriteBuffer里的内容，接收的数据存在ReadBuffer里
	XSpi_Transfer(SpiInstPtr, WriteBuffer, ReadBuffer, BUFFER_SIZE);
}
