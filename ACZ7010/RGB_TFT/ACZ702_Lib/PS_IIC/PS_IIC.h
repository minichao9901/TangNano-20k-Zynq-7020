#ifndef __PS_IIC_H__
#define __PS_IIC_H__


#include "xiicps.h"


extern XIicPs IIC0;/**IIC设备的实例化对象**/

void PS_IIC_Init(XIicPs *InstPtr,uint16_t DeviceId,uint32_t FsclHz);
void PS_IIC_Write_Reg(XIicPs *InstPtr,uint8_t SlaveID, uint8_t RegBIT, uint16_t reg, uint8_t dat);
uint8_t PS_IIC_Read_Reg(XIicPs *InstPtr,uint8_t SlaveID, uint8_t RegBIT, uint16_t reg);
void PS_IIC_SeqWrite_Reg(XIicPs *InstPtr,uint8_t SlaveID, uint8_t RegBIT,
		uint16_t reg, uint8_t *send_dat, uint32_t send_len);
void PS_IIC_SeqRead_Reg(XIicPs *InstPtr,uint8_t SlaveID, uint8_t RegBIT,
		uint16_t reg,uint8_t *recv_data,uint32_t recv_len);


#endif
