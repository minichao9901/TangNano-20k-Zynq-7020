/**
  *****************************************************************************
  * 						PS����SPI��ؿ�
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
**  @brief	��ʼ��PS_SPI�豸
**  @param	SpiInstancePtr	SPI����
**  @param	SpiDeviceId		SPI�豸ID
**  @param	SPI_Mode		SPIģʽѡ��
**  @param	Prescaler		��Ƶϵ��xx				XSPIPS_CLK_PRESCALE_xx
** 	ģʽ�����¼��֣�	XSPIPS_MASTER_OPTION			����Ϊ������Ĭ��Ϊ�ӻ���
** 					XSPIPS_CLK_ACTIVE_LOW_OPTION	��ʱ�Ӽ�����Ϊ�͵�ƽ��Ч��Ĭ��Ϊ�ߵ�ƽ��Ч��
** 					XSPIPS_CLK_PHASE_1_OPTION		Ƭѡ�źź󣬵ڶ���SCK��Ե��������Ч��Ĭ��Ϊ��һ����Ե��Ч��
** 					XSPIPS_DECODE_SSELECT_OPTION	����Ϊ8λ�ӻ�ģʽ��ʹ��3λ2��������ѡ��ӻ���Ĭ��Ϊ3λ�ӻ�ģʽ��
** 					XSPIPS_FORCE_SSELECT_OPTION		�ֶ�Ƭѡģʽ
** 					XSPIPS_MANUAL_START_OPTION		����Ϊ�ֶ���ʼģʽ
**	Sample:	//��ʼ��SPI�豸����Ϊ����ģʽ��64��Ƶ
**			PS_SPI_Init(&SPI0, XPAR_XSPIPS_0_DEVICE_ID,
**				XSPIPS_CLK_PRESCALE_64, XSPIPS_MASTER_OPTION);
************************************************************************************************************/
void PS_SPI_Init(XSpiPs *SpiInstPtr,uint16_t DeviceId, uint8_t Prescaler, uint8_t SPI_Mode)
{
	XSpiPs_Config *SpiConfig;

	//�����豸ID�����ñ��в������ã�Ȼ���ʼ������
	SpiConfig = XSpiPs_LookupConfig(DeviceId);
	XSpiPs_CfgInitialize(SpiInstPtr, SpiConfig, SpiConfig->BaseAddress);

	//����SPI��ģʽ
	XSpiPs_SetOptions(SpiInstPtr, SPI_Mode);

	//����SPI�ķ�Ƶ��ʹ��
	XSpiPs_SetClkPrescaler(SpiInstPtr, Prescaler);
	XSpiPs_Enable(SpiInstPtr);
}

/************************************************************************************************************
**	@brief	ʹ��SPI��������֮������ݴ���
**	@param	SpiInstancePtr	SPI����
**	@param	Sel_Num			ѡ����豸����ţ���ͨģʽֻ��ѡ3���豸��0~2��
**	@param	ReadBuffer		���ܵ��������洢������
**	@param	WriteBuffer		���͵��������洢������
**	@param	BUFFER_SIZE		�����뷢�͵����ݵ��ֽ���
**	Sample:	PS_SPI_Transfer(&SPI0, 0, ReadBuffer, WriteBuffer, 2);//��SPI��0�Ŵӻ�ͨ�ţ��շ�2�ֽ�
************************************************************************************************************/
void PS_SPI_Transfer(XSpiPs *SpiInstPtr, uint32_t Sel_Num, uint8_t *ReadBuffer, uint8_t *WriteBuffer, uint32_t BUFFER_SIZE)
{
	//ѡ��Ƭѡ�豸��һ��3λ
	XSpiPs_SetSlaveSelect(SpiInstPtr, Sel_Num);

	//��ӻ��������ݴ��䣬ÿ����һ���ֽھͽ���һ���ֽڣ����͵�����ΪWriteBuffer������ݣ����յ����ݴ���ReadBuffer��
	XSpiPs_PolledTransfer(SpiInstPtr, WriteBuffer, ReadBuffer, BUFFER_SIZE);
}
