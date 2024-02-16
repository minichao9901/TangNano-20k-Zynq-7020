#ifndef TOUCH_TOUCH_H_
#define TOUCH_TOUCH_H_

#include "sleep.h"
#include "xparameters.h"
#include "xiicps.h"
#include "xil_printf.h"
#include "stdio.h"
#include "PS_GPIO.h"
#include "PS_IIC.h"

#define SLAVE_DEV_ADDR 0x14

#define IIC_Init(freq)  					PS_IIC_Init(&Iic,IIC_DEVICE_ID,freq)
#define GT9147_WR_Reg(REG_ADDR, Data, len)  PS_IIC_SeqWrite_Reg(&Iic,SLAVE_DEV_ADDR, REG16, REG_ADDR, Data,len)
#define GT9147_RD_Reg(REG_ADDR, Data, len)  PS_IIC_SeqRead_Reg(&Iic,SLAVE_DEV_ADDR, REG16, REG_ADDR, Data,len)

#define TP_PRES_DOWN 0x8000  //����������
#define TP_CATH_PRES 0x4000  //�а�������

//GT9147���üĴ���
#define GT_CTRL_REG     0X8040      //GT9147���ƼĴ���
#define GT_CFGS_REG     0X8047      //GT9147������ʼ��ַ�Ĵ���
#define GT_CHECK_REG    0X80FF      //GT9147У��ͼĴ���
#define GT_PID_REG      0X8140      //GT9147��ƷID�Ĵ���

#define GT_GSTID_REG    0X814E      //GT9147��ǰ��⵽�Ĵ������
#define GT_TP1_REG      0X8150      //��һ�����������ݵ�ַ
#define GT_TP2_REG      0X8158      //�ڶ������������ݵ�ַ
#define GT_TP3_REG      0X8160      //���������������ݵ�ַ
#define GT_TP4_REG      0X8168      //���ĸ����������ݵ�ַ
#define GT_TP5_REG      0X8170      //��������������ݵ�ַ

extern uint16_t tp_x[5];        //��ǰ����
extern uint16_t tp_y[5];
extern uint16_t tp_sta;

uint8_t GT9147_Send_Cfg(uint8_t mode);//����GT9147���ò���
void GT9147_Init(void);		//��ʼ��GT9147������
uint8_t GT9147_Scan(void);			//ɨ�败����


#endif /* TOUCH_TOUCH_H_ */
