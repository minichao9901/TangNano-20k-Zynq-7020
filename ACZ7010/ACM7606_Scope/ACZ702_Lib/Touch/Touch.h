#ifndef TOUCH_TOUCH_H_
#define TOUCH_TOUCH_H_

#include "PS_IIC.h"
#include "PS_GPIO.h"

#define SLAVE_DEV_ADDR 0x14

#define Touch_IIC_Init(freq)  					PS_IIC_Init(&IIC0,XPAR_XIICPS_0_DEVICE_ID,freq);
#define GT9147_WR_Reg(REG_ADDR, Data, len)  PS_IIC_SeqWrite_Reg(&IIC0,SLAVE_DEV_ADDR, REG16, REG_ADDR, Data,len)
#define GT9147_RD_Reg(REG_ADDR, Data, len)  PS_IIC_SeqRead_Reg(&IIC0,SLAVE_DEV_ADDR, REG16, REG_ADDR, Data,len)

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

//�����������ݽṹ��
typedef struct {
	uint8_t Touched;		//�����Ƿ񱻴���
	uint8_t Touched_Last;	//�ϴ��Ƿ񱻴���
	uint8_t Touch_Num;		//��������
	uint16_t Tp_X[5];		//�������X����
	uint16_t Tp_Y[5];		//�������Y����
} Touch_Data;

extern Touch_Data Touch_LCD;

uint8_t GT9147_Send_Cfg(uint8_t mode);//����GT9147���ò���
void GT9147_Init(void);		//��ʼ��GT9147������
void GT9147_Scan(Touch_Data *Touch_LCD);	//ɨ�败����
uint8_t Judge_TpXY(Touch_Data Touch_LCD, Box_XY Box);//�жϴ�������
uint8_t Judge_Two_TpXY(Touch_Data Touch_LCD, Box_XY Box);

#endif /* TOUCH_TOUCH_H_ */
