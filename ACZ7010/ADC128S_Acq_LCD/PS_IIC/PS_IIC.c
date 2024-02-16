/**
  *****************************************************************************
  * 						PS部分IIC相关库
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

XIicPs Iic;/**IIC设备的实例化对象**/


/************************************************************************************************************
**  @brief	初始化IIC设备
**  @param	InstancePtr IIC设备实例化对象
**  @param	DeviceId	设备ID
**  @param	FsclHz		IIC时钟频率
**	Sample:	PS_IIC_Init(&Iic,IIC_DEVICE_ID,100000);	//初始化设备Iic，时钟频率为100kHz
************************************************************************************************************/
void PS_IIC_Init(XIicPs *InstancePtr,uint16_t DeviceId,uint32_t FsclHz)
{
	XIicPs_Config *Config;

	//初始化IIC
	Config = XIicPs_LookupConfig(DeviceId);
	XIicPs_CfgInitialize(InstancePtr, Config, Config->BaseAddress);

	//执行自我测试，以确保硬件构建正确。
	while(XIicPs_SelfTest(InstancePtr));

	//设置IIC串行时钟速率。
	XIicPs_SetSClk(InstancePtr, FsclHz);
}

/************************************************************************************************************
**  @brief	写入一个字节数据到I2C设备指定寄存器地址
**  @param	SlaveID	从机地址(7位地址)
**  @param	RegBIT	从机寄存器位数：REG8（8位）REG16（16位）
**  @param	reg		从机寄存器地址
**  @param	Data	要写入的数据
**	Sample:	PS_IIC_Write_Reg(&Iic,0x12,REG8, 0x34,6);	//设备IiC写入数据6到0x34地址，从机地址0x12
************************************************************************************************************/
void PS_IIC_Write_Reg(XIicPs *InstancePtr,uint8_t SlaveID, uint8_t RegBIT, uint16_t reg, uint8_t dat)
{
	uint8_t WR_Buffer[3];
	if(RegBIT == REG8)
	{
		WR_Buffer[0] = reg;
		WR_Buffer[1] = dat;

		//传输数据
		XIicPs_MasterSendPolled(InstancePtr, WR_Buffer,2, SlaveID);
	}
	else if(RegBIT == REG16)
	{
		WR_Buffer[0] = reg>>8;		//取高八位
		WR_Buffer[1] = reg&0x00FF;	//取低八位
		WR_Buffer[2] = dat;

		//传输数据
		XIicPs_MasterSendPolled(InstancePtr, WR_Buffer,3, SlaveID);
	}

	//等待数据传输完成
	while (XIicPs_BusIsBusy(InstancePtr));
}

/************************************************************************************************************
**  @brief	读取I2C设备指定地址寄存器的数据
**  @param	InstancePtr	IIC实例化对象
**  @param	SlaveID	从机地址(7位地址)
**  @param	RegBIT	从机寄存器位数：REG8（8位）REG16（16位）
**  @param	reg		从机寄存器地址
**  @return			要读取的数据
**  Sample:	uint8_t value = PS_IIC_Read_Reg(&Iic, 0x12,REG8, 0x34);//从机设备地址为0x12，读取从机寄存器地址为0x34的数据
************************************************************************************************************/
uint8_t PS_IIC_Read_Reg(XIicPs *InstancePtr,uint8_t SlaveID, uint8_t RegBIT, uint16_t reg)
{
	uint8_t RD_Buffer[1],WR_Buffer[2];

	if(RegBIT == REG8)
	{
		WR_Buffer[0] = reg;

		//发送数据
		XIicPs_MasterSendPolled(InstancePtr, WR_Buffer,1, SlaveID);
	}
	else if(RegBIT == REG16)
	{
		WR_Buffer[0] = reg>>8;		//取高八位
		WR_Buffer[1] = reg&0x00FF;	//取低八位
		//发送数据
		XIicPs_MasterSendPolled(InstancePtr, WR_Buffer,2, SlaveID);
	}
	//等待数据传输完成
	while (XIicPs_BusIsBusy(InstancePtr));

	XIicPs_MasterRecvPolled(InstancePtr, RD_Buffer,1, SlaveID);

	//等待，直到所有的数据被接收
	while (XIicPs_BusIsBusy(InstancePtr));

	return RD_Buffer[0];
}

/************************************************************************************************************
**  @brief	连续写入多个字节数据到I2C设备指定寄存器地址
**  @param	InstancePtr	IIC实例化对象
**  @param	SlaveID	从机地址(7位地址)
**  @param	RegBIT	从机寄存器位数：REG8（8位）REG16（16位）
**  @param	reg		从机寄存器地址
**  @param	send_dat	要写入的数据
**  @param	send_len	要写入的数据长度
**	Sample:	PS_IIC_SeqWrite_Reg(&Iic,0x12,REG8, 0x34,senddata,8);
			//设备Iic写入数据senddata[0]~senddata[7]到0x34地址，从机地址0x12
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

		//传输数据
		XIicPs_MasterSendPolled(InstancePtr, WR_Buffer,send_len+1, SlaveID);
	}
	else if(RegBIT == REG16)
	{
		WR_Buffer[0] = reg>>8;		//取高八位
		WR_Buffer[1] = reg&0x00FF;	//取低八位
		for(cnt=0;cnt<send_len;cnt++)
		{
			WR_Buffer[cnt+2]=send_dat[cnt];
		}
		//传输数据
		XIicPs_MasterSendPolled(InstancePtr, WR_Buffer,send_len+2, SlaveID);
	}

	//等待数据传输完成
	while (XIicPs_BusIsBusy(InstancePtr));
}

/************************************************************************************************************
**  @brief	连续读取多个I2C设备指定地址寄存器的数据
**  @param	InstancePtr	IIC实例化对象
**  @param	SlaveID	从机地址(7位地址)
**  @param	RegBIT	从机寄存器位数：REG8（8位）REG16（16位）
**  @param	reg		从机寄存器地址
**  @param	recv_data	将读取的数据存放在数组recv_data里
**  @param	recv_len	读取数据的长度
**  Sample:	PS_IIC_SeqRead_Reg(&Iic, 0x12,REG8, 0x34,recv_data,8);
**  		//从机设备地址为0x12，读取从机寄存器地址为0x34的数据，连读8个，存到recv_data[0]~recv_data[7]中
************************************************************************************************************/
void PS_IIC_SeqRead_Reg(XIicPs *InstancePtr,uint8_t SlaveID, uint8_t RegBIT, uint16_t reg,uint8_t *recv_data, uint32_t recv_len)
{
	uint8_t WR_Buffer[2];

	if(RegBIT == REG8)
	{
		WR_Buffer[0] = reg;

		//发送数据
		XIicPs_MasterSendPolled(InstancePtr, WR_Buffer,1, SlaveID);
	}
	else if(RegBIT == REG16)
	{
		WR_Buffer[0] = reg>>8;		//取高八位
		WR_Buffer[1] = reg&0x00FF;	//取低八位
		//发送数据
		XIicPs_MasterSendPolled(InstancePtr, WR_Buffer,2, SlaveID);
	}
	//等待数据传输完成
	while (XIicPs_BusIsBusy(InstancePtr));

	XIicPs_MasterRecvPolled(InstancePtr, recv_data,recv_len, SlaveID);

	//等待，直到所有的数据被接收
	while (XIicPs_BusIsBusy(InstancePtr));

}

