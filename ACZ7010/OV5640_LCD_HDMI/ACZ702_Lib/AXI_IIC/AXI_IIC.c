/**
  *****************************************************************************
  * 						AXI IICӦ�ÿ�
  *****************************************************************************
  *
  * @File   : AXI_IIC.c
  * @By     : Sun
  * @Version: V1.4
  * @Date   : 2022 / 06 / 09
  * @Shop	: https://xiaomeige.taobao.com/
  *
  *****************************************************************************
**/


#include "AXI_IIC.h"
#include "COMMON.h"

XIic AXI_IIC0;//����IIC�豸

/*********************************************************************
**  @brief	��ʼ��AXI IIC�豸
**  @param	InstPtr IIC�豸ʵ��������
**  Sample usage:	AXI_IIC_Init(&AXI_IIC0,XPAR_IIC_0_DEVICE_ID);//��ʼ��AXI_IIC0
**  ע��IICƵ����ͼ�λ������AXI IIC IP�������ã�Ĭ��Ϊ100K
***********************************************************************/
void AXI_IIC_Init(XIic *InstPtr, uint16_t DeviceId)
{
	XIic_Config *ConfigPtr;	//ָ���������ݵ�ָ��
	//��ʼ��IIC��������
	ConfigPtr = XIic_LookupConfig(DeviceId);
	XIic_CfgInitialize(InstPtr, ConfigPtr,ConfigPtr->BaseAddress);
}

/************************************************************************************************************
**  @brief	д��һ���ֽ����ݵ�I2C�豸ָ���Ĵ�����ַ
**  @param	InstPtr IIC�豸ʵ��������
**  @param	SlaveID	�ӻ���ַ(7λ��ַ)
**  @param	RegBIT	�ӻ��Ĵ���λ����REG8��8λ��REG16��16λ��
**  @param	reg		�ӻ��Ĵ�����ַ(8λ��16λ)
**  @param	Data	Ҫд�������
**	Sample usage:	AXI_IIC_Write_Reg(&AXI_IIC0,0x12,REG8,0x34, 6);//д������6��0x34��ַ���ӻ���ַ0x12
************************************************************************************************************/
void AXI_IIC_Write_Reg(XIic *InstPtr,uint8_t SlaveID,uint8_t RegBIT, uint16_t reg, uint8_t dat)
{
	uint8_t Byte = 0;
	uint8_t WR_Buffer[3];
	if(RegBIT== REG16)
	{
		WR_Buffer[0] = reg>>8;
		WR_Buffer[1] = reg&0x00FF;
		WR_Buffer[2] = dat;
	}
	else if(RegBIT== REG8)
	{
		WR_Buffer[0] = reg;
		WR_Buffer[1] = dat;
	}


	//��������
	if(RegBIT== REG16)
	{
		do
		{
			Byte = XIic_Send(InstPtr->BaseAddress,SlaveID,WR_Buffer, 3,XIIC_STOP);
		}while(Byte < 3);
	}
	else if(RegBIT== REG8)
	{
		do
		{
			Byte = XIic_Send(InstPtr->BaseAddress,SlaveID,WR_Buffer, 2,XIIC_STOP);
		}while(Byte < 2);
	}
}
/************************************************************************************************************
**  @brief	��ȡI2C�豸ָ����ַ�Ĵ���������
**  @param	InstPtr IIC�豸ʵ��������
**  @param	SlaveID	�ӻ���ַ(7λ��ַ)
**  @param	RegBIT	�ӻ��Ĵ���λ����REG8��8λ��REG16��16λ��
**  @param	reg		�ӻ��Ĵ�����ַ
**  @return			Ҫ��ȡ������
**  Sample usage:	uint8_t value = AXI_IIC_Read_Reg(&AXI_IIC0,0x12,REG8,0x34);//�ӻ��豸��ַΪ0x12����ȡ�ӻ��Ĵ�����ַΪ0x34������
************************************************************************************************************/
uint8_t AXI_IIC_Read_Reg(XIic *InstPtr,uint8_t SlaveID,uint8_t RegBIT,uint16_t reg)
{
	uint8_t Byte = 0;
	uint8_t RD_Buffer[1],WR_Buffer[2];

	if(RegBIT== REG16)
	{
		WR_Buffer[0] = reg>>8;
		WR_Buffer[1] = reg&0x00FF;
	}
	else if(RegBIT== REG8)
	{
		WR_Buffer[0] = reg;
	}

	//��������
	if(RegBIT== REG16)
	{
		do
		{
			Byte = XIic_Send(InstPtr->BaseAddress,SlaveID,WR_Buffer, 2,XIIC_STOP);
		}while(Byte<2);
	}
	else if(RegBIT== REG8)
	{
		do
		{
			Byte = XIic_Send(InstPtr->BaseAddress,SlaveID,WR_Buffer, 1,XIIC_STOP);
		}while(Byte<1);
	}

	//��������
	do
	{
		Byte = XIic_Recv(InstPtr->BaseAddress, SlaveID,RD_Buffer, 1,XIIC_STOP);
	}while(Byte<1);

	return RD_Buffer[0];//���ض�ȡ����ֵ
}

/************************************************************************************************************
**  @brief	����д�����ֽ����ݵ�I2C�豸ָ���Ĵ�����ַ
**  @param	InstPtr IIC�豸ʵ��������
**  @param	SlaveID	�ӻ���ַ(7λ��ַ)
**  @param	RegBIT	�ӻ��Ĵ���λ����REG8��8λ��REG16��16λ��
**  @param	reg		�ӻ��Ĵ�����ַ(8λ��16λ)
**  @param	send_dat	Ҫд�������
**  @param	send_len	д�����ݵĳ���
**	Sample usage:	AXI_IIC_SeqWrite_Reg(&AXI_IIC0,0x12,REG8,0x34,send_dat,6);//������send_dat���6����������д��0x12�豸��0x34��0x39�Ĵ���
************************************************************************************************************/
void AXI_IIC_SeqWrite_Reg(XIic *InstPtr,uint8_t SlaveID,uint8_t RegBIT, uint16_t reg, uint8_t *send_dat, uint32_t send_len)
{
	uint32_t Byte = 0;
	uint8_t WR_Buffer[send_len+2];
	if(RegBIT == REG16)
	{
		WR_Buffer[0] = reg>>8;		//ȡ�߰�λ
		WR_Buffer[1] = reg&0x00FF;	//ȡ�Ͱ�λ

		memcpy(WR_Buffer+2,send_dat,send_len);
	}
	else if(RegBIT == REG8)
	{
		WR_Buffer[0] = reg;

		memcpy(WR_Buffer+1,send_dat,send_len);
	}
	
	//��������
	if(RegBIT== REG16)
	{
		do
		{
			Byte = XIic_Send(InstPtr->BaseAddress,SlaveID,WR_Buffer, send_len + 2,XIIC_STOP);
		}while(Byte < (send_len + 2));
	}
	else if(RegBIT== REG8)
	{
		do
		{
			Byte = XIic_Send(InstPtr->BaseAddress,SlaveID,WR_Buffer, send_len + 1,XIIC_STOP);
		}while(Byte < (send_len + 1));
	}
}
/************************************************************************************************************
**  @brief	������ȡ����ֽ����ݵ�I2C�豸ָ���Ĵ�����ַ
**  @param	InstPtr IIC�豸ʵ��������
**  @param	SlaveID	�ӻ���ַ(7λ��ַ)
**  @param	RegBIT	�ӻ��Ĵ���λ����REG8��8λ��REG16��16λ��
**  @param	reg		�ӻ��Ĵ�����ַ(8λ��16λ)
**  @param	recv_data	����ȡ�����ݴ��������recv_data��
**  @param	recv_len	��ȡ���ݵĳ���
**	Sample usage:	AXI_IIC_SeqRead_Reg(&AXI_IIC0,0x12,REG8,0x34,recv_data,6);//��0x12�豸��0x34��0x39�Ĵ���������ݶ�ȡ������send_dat�����Ϊ6
************************************************************************************************************/
void AXI_IIC_SeqRead_Reg(XIic *InstPtr,uint8_t SlaveID, uint8_t RegBIT, uint16_t reg,uint8_t *recv_data, uint32_t recv_len)
{
	uint32_t Byte = 0;
	uint8_t WR_Buffer[2];

	if(RegBIT== REG16)
	{
		WR_Buffer[0] = reg>>8;
		WR_Buffer[1] = reg&0x00FF;
	}
	else if(RegBIT== REG8)
	{
		WR_Buffer[0] = reg;
	}

	//��������
	if(RegBIT== REG16)
	{
		do
		{
			Byte = XIic_Send(InstPtr->BaseAddress,SlaveID,WR_Buffer, 2,XIIC_STOP);
		}while(Byte<2);
	}
	else if(RegBIT== REG8)
	{
		do
		{
			Byte = XIic_Send(InstPtr->BaseAddress,SlaveID,WR_Buffer, 1,XIIC_STOP);
		}while(Byte<1);
	}

	//��������
	do
	{
		Byte = XIic_Recv(InstPtr->BaseAddress, SlaveID,recv_data, recv_len,XIIC_STOP);
	}while(Byte<recv_len);
}
