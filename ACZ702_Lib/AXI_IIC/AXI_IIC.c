/**
  *****************************************************************************
  * 						AXI IIC应用库
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

XIic AXI_IIC0;//例化IIC设备

/*********************************************************************
**  @brief	初始化AXI IIC设备
**  @param	InstPtr IIC设备实例化对象
**  Sample usage:	AXI_IIC_Init(&AXI_IIC0,XPAR_IIC_0_DEVICE_ID);//初始化AXI_IIC0
**  注：IIC频率在图形化界面的AXI IIC IP核中配置，默认为100K
***********************************************************************/
void AXI_IIC_Init(XIic *InstPtr, uint16_t DeviceId)
{
	XIic_Config *ConfigPtr;	//指向配置数据的指针
	//初始化IIC驱动程序
	ConfigPtr = XIic_LookupConfig(DeviceId);
	XIic_CfgInitialize(InstPtr, ConfigPtr,ConfigPtr->BaseAddress);
}

/************************************************************************************************************
**  @brief	写入一个字节数据到I2C设备指定寄存器地址
**  @param	InstPtr IIC设备实例化对象
**  @param	SlaveID	从机地址(7位地址)
**  @param	RegBIT	从机寄存器位数：REG8（8位）REG16（16位）
**  @param	reg		从机寄存器地址(8位或16位)
**  @param	Data	要写入的数据
**	Sample usage:	AXI_IIC_Write_Reg(&AXI_IIC0,0x12,REG8,0x34, 6);//写入数据6到0x34地址，从机地址0x12
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


	//发送数据
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
**  @brief	读取I2C设备指定地址寄存器的数据
**  @param	InstPtr IIC设备实例化对象
**  @param	SlaveID	从机地址(7位地址)
**  @param	RegBIT	从机寄存器位数：REG8（8位）REG16（16位）
**  @param	reg		从机寄存器地址
**  @return			要读取的数据
**  Sample usage:	uint8_t value = AXI_IIC_Read_Reg(&AXI_IIC0,0x12,REG8,0x34);//从机设备地址为0x12，读取从机寄存器地址为0x34的数据
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

	//发送数据
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

	//接收数据
	do
	{
		Byte = XIic_Recv(InstPtr->BaseAddress, SlaveID,RD_Buffer, 1,XIIC_STOP);
	}while(Byte<1);

	return RD_Buffer[0];//返回读取到的值
}

/************************************************************************************************************
**  @brief	连续写入多个字节数据到I2C设备指定寄存器地址
**  @param	InstPtr IIC设备实例化对象
**  @param	SlaveID	从机地址(7位地址)
**  @param	RegBIT	从机寄存器位数：REG8（8位）REG16（16位）
**  @param	reg		从机寄存器地址(8位或16位)
**  @param	send_dat	要写入的数据
**  @param	send_len	写入数据的长度
**	Sample usage:	AXI_IIC_SeqWrite_Reg(&AXI_IIC0,0x12,REG8,0x34,send_dat,6);//将数组send_dat里的6个数据连续写入0x12设备的0x34到0x39寄存器
************************************************************************************************************/
void AXI_IIC_SeqWrite_Reg(XIic *InstPtr,uint8_t SlaveID,uint8_t RegBIT, uint16_t reg, uint8_t *send_dat, uint32_t send_len)
{
	uint32_t Byte = 0;
	uint8_t WR_Buffer[send_len+2];
	if(RegBIT == REG16)
	{
		WR_Buffer[0] = reg>>8;		//取高八位
		WR_Buffer[1] = reg&0x00FF;	//取低八位

		memcpy(WR_Buffer+2,send_dat,send_len);
	}
	else if(RegBIT == REG8)
	{
		WR_Buffer[0] = reg;

		memcpy(WR_Buffer+1,send_dat,send_len);
	}
	
	//发送数据
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
**  @brief	连续读取多个字节数据到I2C设备指定寄存器地址
**  @param	InstPtr IIC设备实例化对象
**  @param	SlaveID	从机地址(7位地址)
**  @param	RegBIT	从机寄存器位数：REG8（8位）REG16（16位）
**  @param	reg		从机寄存器地址(8位或16位)
**  @param	recv_data	将读取的数据存放在数组recv_data里
**  @param	recv_len	读取数据的长度
**	Sample usage:	AXI_IIC_SeqRead_Reg(&AXI_IIC0,0x12,REG8,0x34,recv_data,6);//将0x12设备的0x34到0x39寄存器里的数据读取到数组send_dat里，长度为6
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

	//发送数据
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

	//接收数据
	do
	{
		Byte = XIic_Recv(InstPtr->BaseAddress, SlaveID,recv_data, recv_len,XIIC_STOP);
	}while(Byte<recv_len);
}
