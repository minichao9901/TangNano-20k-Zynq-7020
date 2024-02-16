/**
  *****************************************************************************
  * 						PS����IIC��ؿ�
  *****************************************************************************
  *
  * @File   : PS_IIC.c
  * @By     : Sun
  * @Version: V1.3
  * @Date   : 2021 / 11 / 04
  * @Shop	: https://xiaomeige.taobao.com/
  *
  *****************************************************************************
**/


#include "PS_IIC.h"

XIicPs Iic;/**IIC�豸��ʵ��������**/


/************************************************************************************************************
**  @brief	��ʼ��IIC�豸
**  @param	InstancePtr IIC�豸ʵ��������
**  @param	DeviceId	�豸ID
**  @param	FsclHz		IICʱ��Ƶ��
**	Sample:	PS_IIC_Init(&Iic,IIC_DEVICE_ID,100000);	//��ʼ���豸Iic��ʱ��Ƶ��Ϊ100kHz
************************************************************************************************************/
void PS_IIC_Init(XIicPs *InstancePtr,uint16_t DeviceId,uint32_t FsclHz)
{
	XIicPs_Config *Config;

	//��ʼ��IIC
	Config = XIicPs_LookupConfig(DeviceId);
	XIicPs_CfgInitialize(InstancePtr, Config, Config->BaseAddress);

	//ִ�����Ҳ��ԣ���ȷ��Ӳ��������ȷ��
	while(XIicPs_SelfTest(InstancePtr));

	//����IIC����ʱ�����ʡ�
	XIicPs_SetSClk(InstancePtr, FsclHz);
}

/************************************************************************************************************
**  @brief	д��һ���ֽ����ݵ�I2C�豸ָ���Ĵ�����ַ
**  @param	SlaveID	�ӻ���ַ(7λ��ַ)
**  @param	RegBIT	�ӻ��Ĵ���λ����REG8��8λ��REG16��16λ��
**  @param	reg		�ӻ��Ĵ�����ַ
**  @param	Data	Ҫд�������
**	Sample:	PS_IIC_Write_Reg(&Iic,0x12,REG8, 0x34,6);	//�豸IiCд������6��0x34��ַ���ӻ���ַ0x12
************************************************************************************************************/
void PS_IIC_Write_Reg(XIicPs *InstancePtr,uint8_t SlaveID, uint8_t RegBIT, uint16_t reg, uint8_t dat)
{
	uint8_t WR_Buffer[3];
	if(RegBIT == REG8)
	{
		WR_Buffer[0] = reg;
		WR_Buffer[1] = dat;

		//��������
		XIicPs_MasterSendPolled(InstancePtr, WR_Buffer,2, SlaveID);
	}
	else if(RegBIT == REG16)
	{
		WR_Buffer[0] = reg>>8;		//ȡ�߰�λ
		WR_Buffer[1] = reg&0x00FF;	//ȡ�Ͱ�λ
		WR_Buffer[2] = dat;

		//��������
		XIicPs_MasterSendPolled(InstancePtr, WR_Buffer,3, SlaveID);
	}

	//�ȴ����ݴ������
	while (XIicPs_BusIsBusy(InstancePtr));
}

/************************************************************************************************************
**  @brief	��ȡI2C�豸ָ����ַ�Ĵ���������
**  @param	InstancePtr	IICʵ��������
**  @param	SlaveID	�ӻ���ַ(7λ��ַ)
**  @param	RegBIT	�ӻ��Ĵ���λ����REG8��8λ��REG16��16λ��
**  @param	reg		�ӻ��Ĵ�����ַ
**  @return			Ҫ��ȡ������
**  Sample:	uint8_t value = PS_IIC_Read_Reg(&Iic, 0x12,REG8, 0x34);//�ӻ��豸��ַΪ0x12����ȡ�ӻ��Ĵ�����ַΪ0x34������
************************************************************************************************************/
uint8_t PS_IIC_Read_Reg(XIicPs *InstancePtr,uint8_t SlaveID, uint8_t RegBIT, uint16_t reg)
{
	uint8_t RD_Buffer[1],WR_Buffer[2];

	if(RegBIT == REG8)
	{
		WR_Buffer[0] = reg;

		//��������
		XIicPs_MasterSendPolled(InstancePtr, WR_Buffer,1, SlaveID);
	}
	else if(RegBIT == REG16)
	{
		WR_Buffer[0] = reg>>8;		//ȡ�߰�λ
		WR_Buffer[1] = reg&0x00FF;	//ȡ�Ͱ�λ
		//��������
		XIicPs_MasterSendPolled(InstancePtr, WR_Buffer,2, SlaveID);
	}
	//�ȴ����ݴ������
	while (XIicPs_BusIsBusy(InstancePtr));

	XIicPs_MasterRecvPolled(InstancePtr, RD_Buffer,1, SlaveID);

	//�ȴ���ֱ�����е����ݱ�����
	while (XIicPs_BusIsBusy(InstancePtr));

	return RD_Buffer[0];
}

/************************************************************************************************************
**  @brief	����д�����ֽ����ݵ�I2C�豸ָ���Ĵ�����ַ
**  @param	InstancePtr	IICʵ��������
**  @param	SlaveID	�ӻ���ַ(7λ��ַ)
**  @param	RegBIT	�ӻ��Ĵ���λ����REG8��8λ��REG16��16λ��
**  @param	reg		�ӻ��Ĵ�����ַ
**  @param	send_dat	Ҫд�������
**  @param	send_len	Ҫд������ݳ���
**	Sample:	PS_IIC_SeqWrite_Reg(&Iic,0x12,REG8, 0x34,senddata,8);
			//�豸Iicд������senddata[0]~senddata[7]��0x34��ַ���ӻ���ַ0x12
************************************************************************************************************/
void PS_IIC_SeqWrite_Reg(XIicPs *InstancePtr,uint8_t SlaveID, uint8_t RegBIT, uint16_t reg, uint8_t *send_dat, uint32_t send_len)
{
	uint8_t WR_Buffer[send_len+2];
	uint8_t cnt;
	if(RegBIT == REG8)
	{
		WR_Buffer[0] = reg;
		for(cnt=0;cnt<send_len;cnt++)
		{
			WR_Buffer[cnt+1]=send_dat[cnt];
		}

		//��������
		XIicPs_MasterSendPolled(InstancePtr, WR_Buffer,send_len+1, SlaveID);
	}
	else if(RegBIT == REG16)
	{
		WR_Buffer[0] = reg>>8;		//ȡ�߰�λ
		WR_Buffer[1] = reg&0x00FF;	//ȡ�Ͱ�λ
		for(cnt=0;cnt<send_len;cnt++)
		{
			WR_Buffer[cnt+2]=send_dat[cnt];
		}
		//��������
		XIicPs_MasterSendPolled(InstancePtr, WR_Buffer,send_len+2, SlaveID);
	}

	//�ȴ����ݴ������
	while (XIicPs_BusIsBusy(InstancePtr));
}

/************************************************************************************************************
**  @brief	������ȡ���I2C�豸ָ����ַ�Ĵ���������
**  @param	InstancePtr	IICʵ��������
**  @param	SlaveID	�ӻ���ַ(7λ��ַ)
**  @param	RegBIT	�ӻ��Ĵ���λ����REG8��8λ��REG16��16λ��
**  @param	reg		�ӻ��Ĵ�����ַ
**  @param	recv_data	����ȡ�����ݴ��������recv_data��
**  @param	recv_len	��ȡ���ݵĳ���
**  Sample:	PS_IIC_SeqRead_Reg(&Iic, 0x12,REG8, 0x34,recv_data,8);
**  		//�ӻ��豸��ַΪ0x12����ȡ�ӻ��Ĵ�����ַΪ0x34�����ݣ�����8�����浽recv_data[0]~recv_data[7]��
************************************************************************************************************/
void PS_IIC_SeqRead_Reg(XIicPs *InstancePtr,uint8_t SlaveID, uint8_t RegBIT, uint16_t reg,uint8_t *recv_data, uint32_t recv_len)
{
	uint8_t WR_Buffer[2];

	if(RegBIT == REG8)
	{
		WR_Buffer[0] = reg;

		//��������
		XIicPs_MasterSendPolled(InstancePtr, WR_Buffer,1, SlaveID);
	}
	else if(RegBIT == REG16)
	{
		WR_Buffer[0] = reg>>8;		//ȡ�߰�λ
		WR_Buffer[1] = reg&0x00FF;	//ȡ�Ͱ�λ
		//��������
		XIicPs_MasterSendPolled(InstancePtr, WR_Buffer,2, SlaveID);
	}
	//�ȴ����ݴ������
	while (XIicPs_BusIsBusy(InstancePtr));

	XIicPs_MasterRecvPolled(InstancePtr, recv_data,recv_len, SlaveID);

	//�ȴ���ֱ�����е����ݱ�����
	while (XIicPs_BusIsBusy(InstancePtr));

}

