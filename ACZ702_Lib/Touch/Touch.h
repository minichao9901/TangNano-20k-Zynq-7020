#ifndef TOUCH_TOUCH_H_
#define TOUCH_TOUCH_H_

#include "PageDesign.h"
#include "PS_GPIO.h"
#include "PS_IIC.h"

#define TP_RST_PIN	0
#define TP_INT_PIN	54
#define SLAVE_DEV_ADDR 0x14

#define IIC_Init(freq)  					PS_IIC_Init(&IIC0,XPAR_XIICPS_0_DEVICE_ID,freq)
#define GT911_WR_Reg(REG_ADDR, Data, len)  PS_IIC_SeqWrite_Reg(&IIC0,SLAVE_DEV_ADDR, REG16, REG_ADDR, Data,len)
#define GT911_RD_Reg(REG_ADDR, Data, len)  PS_IIC_SeqRead_Reg(&IIC0,SLAVE_DEV_ADDR, REG16, REG_ADDR, Data,len)

//GT911常用寄存器
#define GT_CTRL_REG     0X8040      //GT911控制寄存器
#define GT_CFGS_REG     0X8047      //GT911配置起始地址寄存器
#define GT_CHECK_REG    0X80FF      //GT911校验和寄存器
#define GT_PID_REG      0X8140      //GT911产品ID寄存器

#define GT_GSTID_REG    0X814E      //GT911当前检测到的触摸情况
#define GT_TP1_REG      0X8150      //第一个触摸点数据地址
#define GT_TP2_REG      0X8158      //第二个触摸点数据地址
#define GT_TP3_REG      0X8160      //第三个触摸点数据地址
#define GT_TP4_REG      0X8168      //第四个触摸点数据地址
#define GT_TP5_REG      0X8170      //第五个触摸点数据地址

//创建触摸数据结构体
typedef struct {
	uint8_t Touched;		//本次是否被触摸
	uint8_t Touched_Last;	//上次是否被触摸
	uint8_t Touch_Num;		//触摸点数
	uint16_t Tp_X[5];		//触摸点的X坐标
	uint16_t Tp_Y[5];		//触摸点的Y坐标
} Touch_Data;

extern Touch_Data Touch_LCD;

uint8_t GT911_Send_Cfg(uint8_t mode);//发送GT911配置参数
void GT911_Init(void);		//初始化GT911触摸屏
void GT911_Scan(Touch_Data *Touch_LCD);	//扫描触摸屏
uint8_t Judge_TpXY(Touch_Data Touch_LCD, Box_XY Box);//判断触摸区域
uint8_t Judge_Two_TpXY(Touch_Data Touch_LCD, Box_XY Box);

#endif /* TOUCH_TOUCH_H_ */
