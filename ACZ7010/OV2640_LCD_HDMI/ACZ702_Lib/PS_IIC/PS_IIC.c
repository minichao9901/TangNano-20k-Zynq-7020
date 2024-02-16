/**
  *****************************************************************************
  * 						PS����IIC��ؿ�
  *****************************************************************************
  *
  * @File   : PS_IIC.c
  * @By     : Sun
  * @Version: V1.4
  * @Date   : 2022 / 06 / 09
  * @Shop	: https://xiaomeige.taobao.com/
  *
  *****************************************************************************
**/


#include "PS_IIC.h"
#include "COMMON.h"

XIicPs IIC0;/**IIC�豸��ʵ��������**/


/************************************************************************************************************
**  @brief	��ʼ��IIC�豸
**  @param	InstPtr IIC�豸ʵ��������
**  @param	DeviceId	�豸ID
**  @param	FsclHz		IICʱ��Ƶ��
**	Sample:	PS_IIC_Init(&IIC0,XPAR_XIICPS_0_DEVICE_ID,100000);	//��ʼ���豸IIC0��ʱ��Ƶ��Ϊ100kHz
************************************************************************************************************/
void PS_IIC_Init(XIicPs *InstPtr,uint16_t DeviceId,uint32_t FsclHz)
{
	XIicPs_Config *Config;

	//��ʼ��IIC
	Config = XIicPs_LookupConfig(DeviceId);
	XIicPs_CfgInitialize(InstPtr, Config, Config->BaseAddress);

	//����IIC����ʱ�����ʡ�
	XIicPs_SetSClk(InstPtr, FsclHz);
}

/************************************************************************************************************
**  @brief	д��һ���ֽ����ݵ�I2C�豸ָ���Ĵ�����ַ
**  @param	SlaveID	�ӻ���ַ(7λ��ַ)
**  @param	RegBIT	�ӻ��Ĵ���λ����REG8��8λ��REG16��16λ��
**  @param	reg		�ӻ��Ĵ�����ַ
**  @param	Data	Ҫд�������
**	Sample:	PS_IIC_Write_Reg(&IIC0,0x12,REG8, 0x34,6);	//�豸IIC0д������6��0x34��ַ���ӻ���ַ0x12
************************************************************************************************************/
void PS_IIC_Write_Reg(XIicPs *InstPtr,uint8_t SlaveID, uint8_t RegBIT, uint16_t reg, uint8_t dat)
{
	uint8_t WR_Buffer[3];
	if(RegBIT == REG8)
	{
		WR_Buffer[0] = reg;
		WR_Buffer[1] = dat;

		//��������
		XIicPs_MasterSendPolled(InstPtr, WR_Buffer,2, SlaveID);
	}
	else if(RegBIT == REG16)
	{
		WR_Buffer[0] = reg>>8;		//ȡ�߰�λ
		WR_Buffer[1] = reg&0x00FF;	//ȡ�Ͱ�λ
		WR_Buffer[2] = dat;

		//��������
		XIicPs_MasterSendPolled(InstPtr, WR_Buffer,3, SlaveID);
	}

	//�ȴ����ݴ������
	while (XIicPs_BusIsBusy(InstPtr));
}

/************************************************************************************************************
**  @brief	��ȡI2C�豸ָ����ַ�Ĵ���������
**  @param	InstPtr	IICʵ��������
**  @param	SlaveID	�ӻ���ַ(7λ��ַ)
**  @param	RegBIT	�ӻ��Ĵ���λ����REG8��8λ��REG16��16λ��
**  @param	reg		�ӻ��Ĵ�����ַ
**  @return			Ҫ��ȡ������
**  Sample:	uint8_t value = PS_IIC_Read_Reg(&IIC0, 0x12,REG8, 0x34);//�ӻ��豸��ַΪ0x12����ȡ�ӻ��Ĵ�����ַΪ0x34������
************************************************************************************************************/
uint8_t PS_IIC_Read_Reg(XIicPs *InstPtr,uint8_t SlaveID, uint8_t RegBIT, uint16_t reg)
{
	uint8_t RD_Buffer[1],WR_Buffer[2];

	if(RegBIT == REG8)
	{
		WR_Buffer[0] = reg;

		//��������
		XIicPs_MasterSendPolled(InstPtr, WR_Buffer,1, SlaveID);
	}
	else if(RegBIT == REG16)
	{
		WR_Buffer[0] = reg>>8;		//ȡ�߰�λ
		WR_Buffer[1] = reg&0x00FF;	//ȡ�Ͱ�λ
		//��������
		XIicPs_MasterSendPolled(InstPtr, WR_Buffer,2, SlaveID);
	}
	//�ȴ����ݴ������
	while (XIicPs_BusIsBusy(InstPtr));

	XIicPs_MasterRecvPolled(InstPtr, RD_Buffer,1, SlaveID);

	//�ȴ���ֱ�����е����ݱ�����
	while (XIicPs_BusIsBusy(InstPtr));

	return RD_Buffer[0];
}

/************************************************************************************************************
**  @brief	����д�����ֽ����ݵ�I2C�豸ָ���Ĵ�����ַ
**  @param	InstPtr	IICʵ��������
**  @param	SlaveID	�ӻ���ַ(7λ��ַ)
**  @param	RegBIT	�ӻ��Ĵ���λ����REG8��8λ��REG16��16λ��
**  @param	reg		�ӻ��Ĵ�����ַ
**  @param	send_dat	Ҫд�������
**  @param	send_len	Ҫд������ݳ���
**	Sample:	PS_IIC_SeqWrite_Reg(&IIC0,0x12,REG8, 0x34,senddata,8);
			//�豸IIC0д������senddata[0]~senddata[7]��0x34��ַ���ӻ���ַ0x12
************************************************************************************************************/
void PS_IIC_SeqWrite_Reg(XIicPs *InstPtr,uint8_t SlaveID, uint8_t RegBIT,
		uint16_t reg, uint8_t *send_dat, uint32_t send_len)
{
	uint8_t WR_Buffer[send_len+2];

	if(RegBIT == REG8)
	{
		WR_Buffer[0] = reg;
		memcpy(WR_Buffer+1,send_dat,send_len);

		//��������
		XIicPs_MasterSendPolled(InstPtr, WR_Buffer,send_len+1, SlaveID);
	}
	else if(RegBIT == REG16)
	{
		WR_Buffer[0] = reg>>8;		//ȡ�߰�λ
		WR_Buffer[1] = reg&0x00FF;	//ȡ�Ͱ�λ
		memcpy(WR_Buffer+2,send_dat,send_len);

		//��������
		XIicPs_MasterSendPolled(InstPtr, WR_Buffer,send_len+2, SlaveID);
	}

	//�ȴ����ݴ������
	while (XIicPs_BusIsBusy(InstPtr));
}

/************************************************************************************************************
**  @brief	������ȡ���I2C�豸ָ����ַ�Ĵ���������
**  @param	InstPtr	IICʵ��������
**  @param	SlaveID	�ӻ���ַ(7λ��ַ)
**  @param	RegBIT	�ӻ��Ĵ���λ����REG8��8λ��REG16��16λ��
**  @param	reg		�ӻ��Ĵ�����ַ
**  @param	recv_data	����ȡ�����ݴ��������recv_data��
**  @param	recv_len	��ȡ���ݵĳ���
**  Sample:	PS_IIC_SeqRead_Reg(&IIC0, 0x12,REG8, 0x34,recv_data,8);
**  		//�ӻ��豸��ַΪ0x12����ȡ�ӻ��Ĵ�����ַΪ0x34�����ݣ�����8�����浽recv_data[0]~recv_data[7]��
************************************************************************************************************/
void PS_IIC_SeqRead_Reg(XIicPs *InstPtr,uint8_t SlaveID, uint8_t RegBIT,
		uint16_t reg,uint8_t *recv_data, uint32_t recv_len)
{
	uint8_t WR_Buffer[2];

	if(RegBIT == REG8)
	{
		WR_Buffer[0] = reg;

		//��������
		XIicPs_MasterSendPolled(InstPtr, WR_Buffer,1, SlaveID);
	}
	else if(RegBIT == REG16)
	{
		WR_Buffer[0] = reg>>8;		//ȡ�߰�λ
		WR_Buffer[1] = reg&0x00FF;	//ȡ�Ͱ�λ
		//��������
		XIicPs_MasterSendPolled(InstPtr, WR_Buffer,2, SlaveID);
	}
	//�ȴ����ݴ������
	while (XIicPs_BusIsBusy(InstPtr));

	XIicPs_MasterRecvPolled(InstPtr, recv_data,recv_len, SlaveID);

	//�ȴ���ֱ�����е����ݱ�����
	while (XIicPs_BusIsBusy(InstPtr));

}

