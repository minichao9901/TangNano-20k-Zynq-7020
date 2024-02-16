/**
  *****************************************************************************
  * 					ʹ��AXI IIC IP�˶�дEEEPROM
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

#define SEND_PACK_SIZE 32 //EEPROM���ֻ������дһҳ����(32byte)

int main(void)
{
	uint8_t Write_Data[SEND_PACK_SIZE];
	uint8_t Read_Data[SEND_PACK_SIZE];
	uint16_t i;
	uint16_t Error_Cnt=0;

	AXI_IIC_Init(&AXI_IIC0,XPAR_IIC_0_DEVICE_ID);	//��ʼ���豸Iic��ʱ��Ƶ��Ϊ100kHz

	//������0~31��������Write_Data��
	for(i=0;i<SEND_PACK_SIZE;i++)
	{
		Write_Data[i]=i;
	}

	//��0~31д��EEPROM�ĵ�һҳ0~31��ַ��
	AXI_IIC_SeqWrite_Reg(&AXI_IIC0,0x50,REG16, 0x00,Write_Data,SEND_PACK_SIZE);

	usleep(100000);

	//��ȡEEPROM�ĵ�һҳ0~31��ַ������ݣ������Read_Data������
	AXI_IIC_SeqRead_Reg(&AXI_IIC0, 0x50,REG16, 0x00,Read_Data,SEND_PACK_SIZE);

	//�Ա�Write_Data��Read_Data�Ƿ�һ��
	for(i=0;i<SEND_PACK_SIZE;i++)
	{
		if(Write_Data[i] != Read_Data[i])
		{
			Error_Cnt++;//��¼�������ݸ���
		}
	}

	printf("Error = %d !\n",Error_Cnt);//��ӡ�������ݵĸ�����ȫ����Ϊ0����������ɹ�
	if(Error_Cnt == 0)
		printf("Write EEPROM successful !!!\n");
	else
		printf("Write EEPROM failed !!!\n");

	return 0;
}

