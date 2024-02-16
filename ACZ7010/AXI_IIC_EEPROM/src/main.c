/**
  *****************************************************************************
  * 					使用AXI IIC IP核读写EEEPROM
  *****************************************************************************
  *
  * @File    : main.c
  * @By      : Sun
  * @Version : V1.1
  * @Date    : 2022 / 06 / 13
  *
  *****************************************************************************
**/
#include "COMMON.h"

#define SEND_PACK_SIZE 32 //EEPROM最多只能连续写一页数据(32byte)

int main(void)
{
	uint8_t Write_Data[SEND_PACK_SIZE];
	uint8_t Read_Data[SEND_PACK_SIZE];
	uint16_t i;
	uint16_t Error_Cnt=0;

	AXI_IIC_Init(&AXI_IIC0,XPAR_IIC_0_DEVICE_ID);	//初始化设备Iic，时钟频率为100kHz

	//将数据0~31放入数组Write_Data里
	for(i=0;i<SEND_PACK_SIZE;i++)
	{
		Write_Data[i]=i;
	}

	//将0~31写入EEPROM的第一页0~31地址里
	AXI_IIC_SeqWrite_Reg(&AXI_IIC0,0x50,REG16, 0x00,Write_Data,SEND_PACK_SIZE);

	usleep(100000);

	//读取EEPROM的第一页0~31地址里的数据，存放在Read_Data数组里
	AXI_IIC_SeqRead_Reg(&AXI_IIC0, 0x50,REG16, 0x00,Read_Data,SEND_PACK_SIZE);

	//对比Write_Data与Read_Data是否一致
	for(i=0;i<SEND_PACK_SIZE;i++)
	{
		if(Write_Data[i] != Read_Data[i])
		{
			Error_Cnt++;//记录错误数据个数
		}
	}

	printf("Error = %d !\n",Error_Cnt);//打印错误数据的个数，全对则为0，代表试验成功
	if(Error_Cnt == 0)
		printf("Write EEPROM successful !!!\n");
	else
		printf("Write EEPROM failed !!!\n");

	return 0;
}

