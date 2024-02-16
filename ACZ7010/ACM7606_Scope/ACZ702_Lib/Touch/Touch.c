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
Touch_Data Touch_LCD;
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

    Touch_IIC_Init(100000);		//初始化I2C总线,频率为100K
    PS_GPIO_Init();		//初始化PS端GPIO

    PS_GPIO_SetMode(0, OUTPUT, 1);		//INT引脚设置为输出1
    PS_GPIO_SetMode(54, OUTPUT, 0);		//RST输出为0，复位
    usleep(10000);		//延时10ms
    PS_GPIO_SetPort(54, 1);		//RST输出为1，释放复位
    usleep(10000);		//延时10ms
    PS_GPIO_SetPort(0, 0);		//INT输出为0
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
  * @功能描述: 扫描触摸屏(轮询方式)
  * @输入值：Touch_Data结构体变量
  *
  * @返回值   : 当前触屏状态:0,触屏无触摸;1,触屏有触摸
  *****************************************************************************
**/
void GT9147_Scan(Touch_Data *Touch_LCD)
{
	uint8_t i;
	uint8_t State = 0;
	uint8_t Data_XY[4]={0};

	Touch_LCD->Touched_Last = Touch_LCD->Touched;//保存上次的触摸状态
	GT9147_RD_Reg(GT_GSTID_REG,&State,1);	//读取触摸状态寄存器
	//最高位为1表示数据有效
	if(State & 0X80) {
		GT9147_WR_Reg(GT_GSTID_REG,Data_XY,1);	//写0清寄存器来开启下一次检测
		Touch_LCD->Touch_Num = State & 0X0F;	//获取触点数量，数量为0表示无按键
		if(Touch_LCD->Touch_Num == 0)
			Touch_LCD->Touched = 0;		//无按键
		else
			Touch_LCD->Touched = 1;		//有按键
		//依次获取触摸点坐标
		for(i=0;i<Touch_LCD->Touch_Num;i++) {
			GT9147_RD_Reg(GT9147_TPX_TBL[i],Data_XY,4);	//读取XY坐标值
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
