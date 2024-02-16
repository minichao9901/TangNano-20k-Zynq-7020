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

#define TP_PRES_DOWN 0x8000  //触屏被按下
#define TP_CATH_PRES 0x4000  //有按键按下

//GT9147常用寄存器
#define GT_CTRL_REG     0X8040      //GT9147控制寄存器
#define GT_CFGS_REG     0X8047      //GT9147配置起始地址寄存器
#define GT_CHECK_REG    0X80FF      //GT9147校验和寄存器
#define GT_PID_REG      0X8140      //GT9147产品ID寄存器

#define GT_GSTID_REG    0X814E      //GT9147当前检测到的触摸情况
#define GT_TP1_REG      0X8150      //第一个触摸点数据地址
#define GT_TP2_REG      0X8158      //第二个触摸点数据地址
#define GT_TP3_REG      0X8160      //第三个触摸点数据地址
#define GT_TP4_REG      0X8168      //第四个触摸点数据地址
#define GT_TP5_REG      0X8170      //第五个触摸点数据地址

extern uint16_t tp_x[5];        //当前坐标
extern uint16_t tp_y[5];
extern uint16_t tp_sta;

uint8_t GT9147_Send_Cfg(uint8_t mode);//发送GT9147配置参数
void GT9147_Init(void);		//初始化GT9147触摸屏
uint8_t GT9147_Scan(void);			//扫描触摸屏


#endif /* TOUCH_TOUCH_H_ */
