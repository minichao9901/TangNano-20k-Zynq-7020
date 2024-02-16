#ifndef __PS_IIC_H__
#define __PS_IIC_H__

#include "xparameters.h"
#include "xiicps.h"
#include "xil_printf.h"
#include "sleep.h"
#include "stdio.h"

#define IIC_DEVICE_ID		XPAR_XIICPS_0_DEVICE_ID

//IICʱ��Ƶ��100KHz
#define IIC_SCLK_RATE		100000

#define	REG8	1
#define	REG16	2

// ����SIL9022�ڲ���ַ
#define	DEV_ADDR	    0X14	//IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ


extern XIicPs Iic;/**IIC�豸��ʵ��������**/

void PS_IIC_Init(XIicPs *InstancePtr,uint16_t DeviceId,uint32_t FsclHz);
void PS_IIC_Write_Reg(XIicPs *InstancePtr,uint8_t SlaveID, uint8_t RegBIT, uint16_t reg, uint8_t dat);
uint8_t PS_IIC_Read_Reg(XIicPs *InstancePtr,uint8_t SlaveID, uint8_t RegBIT, uint16_t reg);
void PS_IIC_SeqWrite_Reg(XIicPs *InstancePtr,uint8_t SlaveID, uint8_t RegBIT, uint16_t reg, uint8_t *send_dat, uint32_t send_len);
void PS_IIC_SeqRead_Reg(XIicPs *InstancePtr,uint8_t SlaveID, uint8_t RegBIT, uint16_t reg,uint8_t *recv_data,uint32_t recv_len);


#endif
