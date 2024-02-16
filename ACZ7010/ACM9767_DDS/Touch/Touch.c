/**
  *****************************************************************************
  * 						触摸屏相关库
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

/******** X轴和Y轴坐标方向 ********

 —— —— —— —— —— —— —— ——→	X轴(0~800)
|
|
|
|
↓
Y轴(0~480)

*********************************/

uint16_t tp_x[5];        //当前坐标
uint16_t tp_y[5];
uint16_t tp_sta;
const uint16_t GT9147_TPX_TBL[5]= {GT_TP1_REG,GT_TP2_REG,GT_TP3_REG,GT_TP4_REG,GT_TP5_REG};

/***************GT9147配置参数表***************/
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
  * @功能描述: 发送GT9147配置参数
  *
  * @输入参数: mode:0,参数不保存到flash;1,参数保存到flash
  *****************************************************************************
**/
uint8_t GT9147_Send_Cfg(uint8_t mode)
{
    uint8_t buf[2];
    uint8_t i=0;
    buf[0]=0;
    buf[1]=mode;    //是否掉电保存
    for(i=0; i<sizeof(GT9147_CFG_TBL); i++)
        buf[0]+=GT9147_CFG_TBL[i]; //计算校验和
    buf[0]=(~buf[0])+1;
    GT9147_WR_Reg(GT_CFGS_REG,(uint8_t*)GT9147_CFG_TBL,sizeof(GT9147_CFG_TBL));//发送寄存器配置
    GT9147_WR_Reg(GT_CHECK_REG,buf,2);//写入校验和,和配置更新标记
    return 0;
}

/**
  *****************************************************************************
  * @功能描述: 初始化GT9147触摸屏
  *
  * @返回值   : 0,初始化成功;1,初始化失败
  *****************************************************************************
**/
void GT9147_Init(void)
{
    uint8_t id[4];
    uint8_t buff[1];

    IIC_Init(400000);		//初始化I2C总线,频率为100K
    PS_GPIO_Init();		//初始化PS端GPIO

    GPIO_SetMode(0, OUTPUT, 1);		//INT引脚设置为输出1
    GPIO_SetMode(54, OUTPUT, 0);		//RST输出为0，复位
    usleep(10000);		//延时10ms
    XGpioPs_WritePin(&Gpio, 54, 1);		//RST输出为1，释放复位
    usleep(10000);		//延时10ms
    XGpioPs_WritePin(&Gpio, 0, 0);		//INT输出为0
    usleep(100000);		//延时100ms

    GT9147_RD_Reg(GT_PID_REG,id,4);		//读取ID
    printf("ID:%s\n",id);		//打印ID
    buff[0]=0X02;
	GT9147_WR_Reg(GT_CTRL_REG,buff,1);	//软复位GT9147
	GT9147_RD_Reg(GT_CFGS_REG,buff,1);	//读取GT_CFGS_REG寄存器
	printf("Previous version: %c\n",buff[0]);		//显示之前配置的版本号（A~Z）
	if(buff[0] < GT9147_CFG_TBL[0])		//如果之前配置版本小于预配置版本
		GT9147_Send_Cfg(1);		//更新配置并保存
	GT9147_RD_Reg(GT_CFGS_REG,buff,1);		//读取GT_CFGS_REG寄存器
	printf("Current version: %c\n",buff[0]);		//显示当前配置的版本号（A~Z）
	usleep(10000);		//延时10ms
	buff[0]=0X00;
	GT9147_WR_Reg(GT_CTRL_REG,buff,1);//结束复位
}

/**
  *****************************************************************************
  * @功能描述: 扫描触摸屏(采用查询方式)
  *
  * @输入参数: mode:0,正常扫描.
  *
  * @返回值   : 当前触屏状态:0,触屏无触摸;1,触屏有触摸
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
	GT9147_RD_Reg(GT_GSTID_REG,&mode,1);	//读取触摸点的状态
	if((mode&0X80)&&((mode&0XF)<6))
	{
		//GT9147_WR_Reg(GT_GSTID_REG,0,1);//清标志
		PS_IIC_Write_Reg(&Iic,0x14,REG16, GT_GSTID_REG,0);
	}
	if((mode&0XF)&&((mode&0XF)<6))
	{
		temp=0XFFFF<<(mode&0XF);	//将个数转换为位数
		tempsta=tp_sta;				//保存当前的tp_sta值
		tp_sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES;
		tp_x[4]=tp_x[0];			//保存触点0的数据
		tp_y[4]=tp_y[0];
		for(i=0;i<5;i++)
		{
			if(tp_sta&(1<<i))		//触摸有效
			{
				GT9147_RD_Reg(GT9147_TPX_TBL[i],buf,4);	//读取XY坐标值
				tp_x[i]=((uint16_t)buf[1]<<8)+buf[0];
				tp_y[i]=((uint16_t)buf[3]<<8)+buf[2];
			}
		}
		res=1;
		if(tp_x[0]>800||tp_y[0]>480)//坐标超出范围
		{
			if((mode&0XF)>1)		//有其他点有数据,则复第二个触点的数据到第一个触点.
			{
				tp_x[0]=tp_x[1];
				tp_y[0]=tp_y[1];
			}else					//非法数据,则忽略此次数据(还原原来的)
			{
				tp_x[0]=tp_x[4];
				tp_y[0]=tp_y[4];
				mode=0X80;
				tp_sta=tempsta;		//恢复tp_sta
			}
		}
	}
	if((mode&0X8F)==0X80)			//无触摸点按下
	{
		if(tp_sta&TP_PRES_DOWN)		//之前是被按下的
		{
			tp_sta&=~TP_PRES_DOWN;	//标记按键松开
		}else						//之前就没有被按下
		{
			tp_x[0]=0xffff;
			tp_y[0]=0xffff;
			tp_sta&=0XE000;			//清除点有效标记
		}
	}
	return res;
}
