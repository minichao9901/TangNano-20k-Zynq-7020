/**
  *****************************************************************************
  * 						��������ؿ�
  *****************************************************************************
  *
  * @File   : Touch.c
  * @By     : Sun
  * @Version: V1.1
  * @Date   : 2021 / 11 / 04
  * @Shop	: https://xiaomeige.taobao.com/
  *
  *****************************************************************************
**/


#include "Touch.h"
#include "COMMON.h"
/******** X���Y�����귽�� ********

 ���� ���� ���� ���� ���� ���� ���� ������	X��(0~800)
|
|
|
|
��
Y��(0~480)

*********************************/
Touch_Data Touch_LCD;
uint16_t tp_x[5];        //��ǰ����
uint16_t tp_y[5];
uint16_t tp_sta;
const uint16_t GT911_TPX_TBL[5]= {GT_TP1_REG,GT_TP2_REG,GT_TP3_REG,GT_TP4_REG,GT_TP5_REG};

/***************GT911���ò�����***************/
const uint8_t GT911_CFG_TBL[]= {
		'B',0x20,0x3 ,0xe0,0x1 ,0xa ,0xd ,0x0 ,0x1 ,0xa ,
		0x28,0xf ,0x50,0x32,0x3 ,0x5 ,0x0 ,0x0 ,0x0 ,0x0 ,
		0x22,0x22,0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,0x89,0x29,0xb ,
		0x26,0x24,0xea,0x4 ,0x0 ,0x0 ,0x0 ,0x2 ,0x3 ,0x1c,
		0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,0x3 ,0x64,0x32,0x0 ,0x0 ,
		0x0 ,0x14,0x37,0x94,0xc5,0x2 ,0x7 ,0x0 ,0x0 ,0x4 ,
		0xe8,0x13,0x0 ,0x97,0x1e,0x0 ,0x61,0x30,0x0 ,0x3e,
		0x4c,0x0 ,0x28,0x78,0x0 ,0x28,0x0 ,0x0 ,0x0 ,0x0 ,
		0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,
		0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,
		0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,
		0x0 ,0x0 ,0x16,0x14,0x12,0x10,0xe ,0xc ,0xa ,0x8 ,
		0x6 ,0x4 ,0x2 ,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0xff,0x22,0x21,0x20,0x1f,0x1e,0x1d,0x1c,0x18,
		0x16,0x12,0x10,0xf ,0xa ,0x8 ,0x6 ,0x4 ,0x2 ,0x0 ,
		0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
		0xff,0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,0x0 ,
		0x0 ,0x0 ,0x0 ,0x0 ,
};

/**
  *****************************************************************************
  * @��������: ����GT911���ò���
  *
  * @�������: mode:0,���������浽flash;1,�������浽flash
  *****************************************************************************
**/
uint8_t GT911_Send_Cfg(uint8_t mode)
{
    uint8_t buf[2];
    uint8_t i=0;
    buf[0]=0;
    buf[1]=mode;    //�Ƿ���籣��
    for(i=0; i<sizeof(GT911_CFG_TBL); i++)
        buf[0]+=GT911_CFG_TBL[i]; //����У���
    buf[0]=(~buf[0])+1;
    GT911_WR_Reg(GT_CFGS_REG,(uint8_t*)GT911_CFG_TBL,sizeof(GT911_CFG_TBL));//���ͼĴ�������
    GT911_WR_Reg(GT_CHECK_REG,buf,2);//д��У���,�����ø��±��
    return 0;
}

/**
  *****************************************************************************
  * @��������: ��ʼ��GT911������
  *
  * @����ֵ   : 0,��ʼ���ɹ�;1,��ʼ��ʧ��
  *****************************************************************************
**/
void GT911_Init(void)
{
    uint8_t id[4];
    uint8_t buff[1];

    IIC_Init(400000);	//��ʼ��I2C,Ƶ��Ϊ100K
    PS_GPIO_Init();		//��ʼ��PS��GPIO

    PS_GPIO_SetMode(TP_RST_PIN, OUTPUT, 1);		//INT��������Ϊ���1
    PS_GPIO_SetMode(TP_INT_PIN, OUTPUT, 0);		//RST���Ϊ0����λ
    usleep(10000);		//��ʱ10ms
    PS_GPIO_SetPort(TP_INT_PIN, 1);		//RST���Ϊ1���ͷŸ�λ
    usleep(10000);		//��ʱ10ms
    PS_GPIO_SetPort(TP_RST_PIN, 0);		//INT���Ϊ0
    usleep(100000);		//��ʱ100ms

    GT911_RD_Reg(GT_PID_REG,id,4);		//��ȡID
    printf("ID:%s\n",id);		//��ӡID
    buff[0]=0X02;
	GT911_WR_Reg(GT_CTRL_REG,buff,1);	//��λGT911
	GT911_RD_Reg(GT_CFGS_REG,buff,1);	//��ȡGT_CFGS_REG�Ĵ���
	printf("Previous version: %c\n",buff[0]);		//��ʾ֮ǰ���õİ汾�ţ�A~Z��
	if(buff[0] < GT911_CFG_TBL[0])		//���֮ǰ���ð汾С��Ԥ���ð汾
		GT911_Send_Cfg(1);		//�������ò�����
	GT911_RD_Reg(GT_CFGS_REG,buff,1);		//��ȡGT_CFGS_REG�Ĵ���
	printf("Current version: %c\n",buff[0]);		//��ʾ��ǰ���õİ汾�ţ�A~Z��
	usleep(10000);		//��ʱ10ms
	buff[0]=0X00;
	GT911_WR_Reg(GT_CTRL_REG,buff,1);//������λ
}

/**
  *****************************************************************************
  * @��������: ɨ�败����(��ѯ��ʽ)
  * @����ֵ��Touch_Data�ṹ�����
  *****************************************************************************
**/
void GT911_Scan(Touch_Data *Touch_LCD)
{
	uint8_t i;
	uint8_t State = 0;
	uint8_t Data_XY[4]={0};

	Touch_LCD->Touched_Last = Touch_LCD->Touched;//�����ϴεĴ���״̬
	GT911_RD_Reg(GT_GSTID_REG,&State,1);	//��ȡ����״̬�Ĵ���
	//���λΪ1��ʾ������Ч
	if(State & 0X80) {
		GT911_WR_Reg(GT_GSTID_REG, Data_XY, 1);	//д0��Ĵ�����������һ�μ��
		Touch_LCD->Touch_Num = State & 0X0F;		//��ȡ��������������Ϊ0��ʾ�ް���

		//�������㻻�㵽��Ӧbit��Ϊ1��ʾ������Ϊ0��ʾδ����
		Touch_LCD->Touched = 0x1F >> (5 - Touch_LCD->Touch_Num);
//		if(Touch_LCD->Touch_Num == 0)
//			Touch_LCD->Touched = 0;		//�ް���
//		else
//			Touch_LCD->Touched = 1;		//�а���

		//���λ�ȡ����������
		for(i=0;i<5;i++) {//for(i=0;i<Touch_LCD->Touch_Num;i++) {
			GT911_RD_Reg(GT911_TPX_TBL[i],Data_XY,4);	//��ȡXY����ֵ
			if(((((uint16_t)Data_XY[1]<<8)+Data_XY[0]) <= 800) &&
					((((uint16_t)Data_XY[3]<<8)+Data_XY[2]) <= 480))
			Touch_LCD->Tp_X[i] = ((uint16_t)Data_XY[1]<<8)+Data_XY[0];
			Touch_LCD->Tp_Y[i] = ((uint16_t)Data_XY[3]<<8)+Data_XY[2];
		}
	}
}

uint8_t Judge_TpXY(Touch_Data Touch_LCD, Box_XY Box)
{
	if((Touch_LCD.Tp_X[0] >= Box.X1)&&(Touch_LCD.Tp_X[0] <= Box.X1 + Box.Width)
			&&(Touch_LCD.Tp_Y[0] >= Box.Y1)&&(Touch_LCD.Tp_Y[0] <= Box.Y1 + Box.Height))
		return 1;
	else
		return 0;
}

uint8_t Judge_Two_TpXY(Touch_Data Touch_LCD, Box_XY Box)
{
	if(Touch_LCD.Touch_Num == 2) {
		if((Touch_LCD.Tp_X[0] >= Box.X1)&&(Touch_LCD.Tp_X[0] <= Box.X1 + Box.Width)
				&&(Touch_LCD.Tp_Y[0] >= Box.Y1)&&(Touch_LCD.Tp_Y[0] <= Box.Y1 + Box.Height)) {
			if((Touch_LCD.Tp_X[1] >= Box.X1)&&(Touch_LCD.Tp_X[1] <= Box.X1 + Box.Width)
						&&(Touch_LCD.Tp_Y[1] >= Box.Y1)&&(Touch_LCD.Tp_Y[1] <= Box.Y1 + Box.Height)) {
				return 1;
			} else
				return 0;
		} else
			return 0;
	}
	else
		return 0;
}
