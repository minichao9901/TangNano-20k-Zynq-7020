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

/******** X���Y�����귽�� ********

 ���� ���� ���� ���� ���� ���� ���� ������	X��(0~800)
|
|
|
|
��
Y��(0~480)

*********************************/

uint16_t tp_x[5];        //��ǰ����
uint16_t tp_y[5];
uint16_t tp_sta;
const uint16_t GT9147_TPX_TBL[5]= {GT_TP1_REG,GT_TP2_REG,GT_TP3_REG,GT_TP4_REG,GT_TP5_REG};

/***************GT9147���ò�����***************/
const uint8_t GT9147_CFG_TBL[]= {
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
  * @��������: ����GT9147���ò���
  *
  * @�������: mode:0,���������浽flash;1,�������浽flash
  *****************************************************************************
**/
uint8_t GT9147_Send_Cfg(uint8_t mode)
{
    uint8_t buf[2];
    uint8_t i=0;
    buf[0]=0;
    buf[1]=mode;    //�Ƿ���籣��
    for(i=0; i<sizeof(GT9147_CFG_TBL); i++)
        buf[0]+=GT9147_CFG_TBL[i]; //����У���
    buf[0]=(~buf[0])+1;
    GT9147_WR_Reg(GT_CFGS_REG,(uint8_t*)GT9147_CFG_TBL,sizeof(GT9147_CFG_TBL));//���ͼĴ�������
    GT9147_WR_Reg(GT_CHECK_REG,buf,2);//д��У���,�����ø��±��
    return 0;
}

/**
  *****************************************************************************
  * @��������: ��ʼ��GT9147������
  *
  * @����ֵ   : 0,��ʼ���ɹ�;1,��ʼ��ʧ��
  *****************************************************************************
**/
void GT9147_Init(void)
{
    uint8_t id[4];
    uint8_t buff[1];

    IIC_Init(400000);		//��ʼ��I2C����,Ƶ��Ϊ100K
    PS_GPIO_Init();		//��ʼ��PS��GPIO

    GPIO_SetMode(0, OUTPUT, 1);		//INT��������Ϊ���1
    GPIO_SetMode(54, OUTPUT, 0);		//RST���Ϊ0����λ
    usleep(10000);		//��ʱ10ms
    XGpioPs_WritePin(&Gpio, 54, 1);		//RST���Ϊ1���ͷŸ�λ
    usleep(10000);		//��ʱ10ms
    XGpioPs_WritePin(&Gpio, 0, 0);		//INT���Ϊ0
    usleep(100000);		//��ʱ100ms

    GT9147_RD_Reg(GT_PID_REG,id,4);		//��ȡID
    printf("ID:%s\n",id);		//��ӡID
    buff[0]=0X02;
	GT9147_WR_Reg(GT_CTRL_REG,buff,1);	//��λGT9147
	GT9147_RD_Reg(GT_CFGS_REG,buff,1);	//��ȡGT_CFGS_REG�Ĵ���
	printf("Previous version: %c\n",buff[0]);		//��ʾ֮ǰ���õİ汾�ţ�A~Z��
	if(buff[0] < GT9147_CFG_TBL[0])		//���֮ǰ���ð汾С��Ԥ���ð汾
		GT9147_Send_Cfg(1);		//�������ò�����
	GT9147_RD_Reg(GT_CFGS_REG,buff,1);		//��ȡGT_CFGS_REG�Ĵ���
	printf("Current version: %c\n",buff[0]);		//��ʾ��ǰ���õİ汾�ţ�A~Z��
	usleep(10000);		//��ʱ10ms
	buff[0]=0X00;
	GT9147_WR_Reg(GT_CTRL_REG,buff,1);//������λ
}

/**
  *****************************************************************************
  * @��������: ɨ�败����(���ò�ѯ��ʽ)
  *
  * @�������: mode:0,����ɨ��.
  *
  * @����ֵ   : ��ǰ����״̬:0,�����޴���;1,�����д���
  *****************************************************************************
**/
uint8_t GT9147_Scan(void)
{
	uint8_t mode = 0;
	uint8_t buf[4];
	uint8_t i=0;
	uint8_t res=0;
	uint16_t temp;
	uint16_t tempsta;
	GT9147_RD_Reg(GT_GSTID_REG,&mode,1);	//��ȡ�������״̬
	if((mode&0X80)&&((mode&0XF)<6))
	{
		//GT9147_WR_Reg(GT_GSTID_REG,0,1);//���־
		PS_IIC_Write_Reg(&Iic,0x14,REG16, GT_GSTID_REG,0);
	}
	if((mode&0XF)&&((mode&0XF)<6))
	{
		temp=0XFFFF<<(mode&0XF);	//������ת��Ϊλ��
		tempsta=tp_sta;				//���浱ǰ��tp_staֵ
		tp_sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES;
		tp_x[4]=tp_x[0];			//���津��0������
		tp_y[4]=tp_y[0];
		for(i=0;i<5;i++)
		{
			if(tp_sta&(1<<i))		//������Ч
			{
				GT9147_RD_Reg(GT9147_TPX_TBL[i],buf,4);	//��ȡXY����ֵ
				tp_x[i]=((uint16_t)buf[1]<<8)+buf[0];
				tp_y[i]=((uint16_t)buf[3]<<8)+buf[2];
			}
		}
		res=1;
		if(tp_x[0]>800||tp_y[0]>480)//���곬����Χ
		{
			if((mode&0XF)>1)		//��������������,�򸴵ڶ�����������ݵ���һ������.
			{
				tp_x[0]=tp_x[1];
				tp_y[0]=tp_y[1];
			}else					//�Ƿ�����,����Դ˴�����(��ԭԭ����)
			{
				tp_x[0]=tp_x[4];
				tp_y[0]=tp_y[4];
				mode=0X80;
				tp_sta=tempsta;		//�ָ�tp_sta
			}
		}
	}
	if((mode&0X8F)==0X80)			//�޴����㰴��
	{
		if(tp_sta&TP_PRES_DOWN)		//֮ǰ�Ǳ����µ�
		{
			tp_sta&=~TP_PRES_DOWN;	//��ǰ����ɿ�
		}else						//֮ǰ��û�б�����
		{
			tp_x[0]=0xffff;
			tp_y[0]=0xffff;
			tp_sta&=0XE000;			//�������Ч���
		}
	}
	return res;
}
