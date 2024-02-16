/**
  *****************************************************************************
  * 						AXI_QUAD_SPI��ؿ�
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
**  @brief	��ʼ��PL_SPI�豸
**  @param	SpiInstPtr		SPI����
**  @param	SpiDeviceId		SPI�豸ID
**  @param	SPI_Mode		SPIģʽѡ��
** 	ģʽ�����¼��֣�XSP_MASTER_OPTION				����Ϊ������Ĭ��Ϊ�ӻ���
** 					XSP_CLK_ACTIVE_LOW_OPTION		��ʱ�Ӽ�����Ϊ�͵�ƽ��Ч��Ĭ��Ϊ�ߵ�ƽ��Ч��
** 					XSP_CLK_PHASE_1_OPTION			Ƭѡ�źź󣬵ڶ���SCK��Ե��������Ч��Ĭ��Ϊ��һ����Ե��Ч��
** 					XSP_LOOPBACK_OPTION				����Ϊ�ػ�ģʽ��Ĭ��Ϊ��׼ģʽ��
** 					XSP_MANUAL_SSELECT_OPTION		����Ϊ�ֶ�Ƭѡģʽ��Ĭ��Ϊ�Զ�Ƭѡģʽ��
**	Sample usage:	AXI_SPI_Init(&AXI_SPI0, SPI_DEVICE_ID, XSP_MASTER_OPTION);//��ʼ��SPI�豸����Ϊ����ģʽ
************************************************************************************************************/
void AXI_SPI_Init(XSpi *SpiInstPtr, uint16_t SpiDeviceId,uint8_t SPI_Mode)
{
	XSpi_Config *ConfigPtr;

	//��ʼ��SPI��������
	ConfigPtr = XSpi_LookupConfig(SpiDeviceId);
	XSpi_CfgInitialize(SpiInstPtr, ConfigPtr,ConfigPtr->BaseAddress);

	//����SPI��ģʽ
	XSpi_SetOptions(SpiInstPtr, SPI_Mode);

	//����SPI��������
	XSpi_Start(SpiInstPtr);

	//����ȫ���ж���ʹ����ѯģʽ����
	XSpi_IntrGlobalDisable(SpiInstPtr);
}

/************************************************************************************************************
**	@brief	ʹ��SPI��������֮������ݴ���
**	@param	SpiInstPtr		SPI����ָ��
**	@param	Sel_Num			���豸�ı�ţ�0~31�����������Ϊ���豸����˴���Ч��
**	@param	ReadBuffer		���ܵ��������洢������
**	@param	WriteBuffer		���͵��������洢������
**	@param	BUFFER_SIZE		�����뷢�͵����ݵ��ֽ���
**	Sample usage:			//��SPI��0�Ŵӻ�ͨ�ţ��շ�2�ֽ�
**							AXI_SPI_Transfer(&AXI_SPI0, 0, ReadBuffer, WriteBuffer, 2);
************************************************************************************************************/
void AXI_SPI_Transfer(XSpi *SpiInstPtr, uint32_t Sel_Num, uint8_t *ReadBuffer, uint8_t *WriteBuffer, uint32_t BUFFER_SIZE)
{
	//ѡ��Ƭѡ�豸��һ��32λ��1����һλ��ѡ����һ��ΪƬѡ�豸
	XSpi_SetSlaveSelect(SpiInstPtr, 1 << Sel_Num);

	//��ӻ��������ݴ��䣬ÿ����һ���ֽھͽ���һ���ֽڣ����͵�����ΪWriteBuffer������ݣ����յ����ݴ���ReadBuffer��
	XSpi_Transfer(SpiInstPtr, WriteBuffer, ReadBuffer, BUFFER_SIZE);
}
