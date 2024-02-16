/**
  *****************************************************************************
  * 						lcd显示相关库
  *****************************************************************************
  *
  * @File   : lcd.c
  * @By     : Sun
  * @Version: V1.0
  * @Date   : 2021 / 11 / 04
  * @Shop	: https://xiaomeige.taobao.com/
  *
  *****************************************************************************
**/
#include "lcd.h"
#include "font.h"

/******** X轴和Y轴坐标方向 ********

 —— —— —— —— —— —— —— ——→	X轴(0~800)
|
|
|
|
↓
Y轴(0~480)

*********************************/

XAxiVdma     Vdma;
DisplayCtrl  DispCtrl;

uint8_t *frame = (uint8_t*)FrameBuffer_Addr;

/***************文字颜色和背景色***************/
uint32_t POINT_COLOR=LCD_RED;	//文字颜色，默认为红色
uint32_t BACK_COLOR =LCD_WHITE; 	//文字背景色，默认为白色


/**
  *****************************************************************************
  * @功能描述: RGBLCD初始化
  *****************************************************************************
**/
void LCD_Init()
{
	//配置VDMA
	run_vdma_frame_buffer(&Vdma, VDMA_ID, RGB_LCD.width, RGB_LCD.height,
							FrameBuffer_Addr,0, 0,ONLY_READ);

    //初始化Display controller
	DisplayInitialize(&DispCtrl, DISP_VTC_ID, DYNCLK_BASEADDR);

    //设置VideoMode
	DisplaySetMode(&DispCtrl, &RGB_LCD);
	DisplayStart(&DispCtrl);
	LCD_Color_Fill(0,0,800,480,0xFFFFFF);
}

/**
  *****************************************************************************
  * @功能描述: RGBLCD刷新，图像数据改变后，必须刷新才能显示新图像
  * @注意：	使用本函数刷新图像需要时间，请在画好图形后再使用本函数一次性刷新全图
  * 		不要短时间内频繁刷新，会导致图像绘制特别慢
  *****************************************************************************
**/
void LCD_Refresh()
{
	//刷新Data Cache，更新图像
	Xil_DCacheFlushRange((UINTPTR) FrameBuffer_Addr, 800*480*BYTES_PIXEL);
}

/**
  *****************************************************************************
  * @功能描述: 画点
  *
  * @输入参数: x,y:点坐标
  * 		 POINT_COLOR:此点的颜色
  *****************************************************************************
**/
void LCD_DrawPoint(uint16_t x,uint16_t y,uint32_t color)
{
	//x限幅
	if(x>800)
		x=800;
	//y限幅
	if(y>480)
		y=480;
	frame[BYTES_PIXEL*x + 800*BYTES_PIXEL*y] = color&0x0000ff;
	frame[BYTES_PIXEL*x + 800*BYTES_PIXEL*y + 1] = (color>>8)&0x0000ff;
	frame[BYTES_PIXEL*x + 800*BYTES_PIXEL*y + 2] = (color>>16)&0x0000ff;

	//刷新Cache，更新数据
	//Xil_DCacheFlush();
}


/**
  *****************************************************************************
  * @功能描述: 在指定区域内填充指定颜色块
  *
  * @输入参数: (x1,y1),(x2,y2):填充矩形对角坐标,区域大小为:(x2-x1+1)*(y2-y1+1)
  * 		  color:要填充的颜色
  *****************************************************************************
**/
void LCD_Color_Fill(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint32_t color)
{
	uint16_t x,y,temp;

	if(y1>y2)
	{
		temp=y1;
		y1=y2;
		y2=temp;
	}

	if(x1>x2)
	{
		temp=x1;
		x1=x2;
		x2=temp;
	}

	for(y=y1;y<=y2;y++)
	{
		for(x=x1;x<=x2;x++)
		{
			frame[BYTES_PIXEL*x + 800*BYTES_PIXEL*y] = color&0x0000ff;
			frame[BYTES_PIXEL*x + 800*BYTES_PIXEL*y + 1] = (color>>8)&0x0000ff;
			frame[BYTES_PIXEL*x + 800*BYTES_PIXEL*y + 2] = (color>>16)&0x0000ff;
		}
	}

	//刷新Cache，更新数据
	//Xil_DCacheFlush();
}

/**
  *****************************************************************************
  * @功能描述: 清屏
  *
  * @输入参数: color:要填充的颜色
  *****************************************************************************
**/
void LCD_Clear(uint32_t color)
{
	uint16_t x,y;
	for(y=0;y<480;y++)
	{
		for(x=0;x<800;x++)
		{
			frame[BYTES_PIXEL*x + 800*BYTES_PIXEL*y] = color&0x0000ff;
			frame[BYTES_PIXEL*x + 800*BYTES_PIXEL*y + 1] = (color>>8)&0x0000ff;
			frame[BYTES_PIXEL*x + 800*BYTES_PIXEL*y + 2] = (color>>16)&0x0000ff;
		}
	}

	//刷新Cache，更新数据
	Xil_DCacheFlush();
}
/*******************************************************
 *@功能描述:以POINT_COLOR指定的颜色画直线
 *@输入参数:uint16_t x1   : 第一个点的x坐标
 *			uint16_t y1   : 第一个点的y坐标
 *			uint16_t x2   : 第二个点的x坐标
 *			uint16_t y2   : 第二个点的y坐标
 *			uint16_t color: 线的颜色
 *使用说明 :使用这个函数只要求两个点都在LCD显示范围内，不要求点的相对位置
 *@返回值  :0:正常, 1:画线失败
 *******************************************************/
uint8_t LCD_DrawWaveLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;

	if(y1 >= 480 - 40 - 1)
		y1 = 480 - 40 - 1;

	if(y1 <= 40)
		y1 = 40;

	if(y2 >= 480 - 40 - 1)
		y2 = 480 - 40 - 1;

	if(y2 <= 40)
		y2 = 40;

	delta_x = x2 - x1; //计算坐标增量
	delta_y = y2 - y1;
	uRow = x1;
	uCol = y1;
	if (delta_x > 0)
		incx = 1; //x步进为-1
	else if (delta_x == 0)
		incx = 0; //垂直线
	else {
		incx = -1; //x步进为1
	}

	if (delta_y > 0)
		incy = 1;
	else if (delta_y == 0)
		incy = 0; //水平线
	else {
		incy = -1;
		delta_y = -delta_y;
	}

	if (delta_x > delta_y)
		distance = delta_x; //选取基本增量坐标轴
	else
		distance = delta_y;

	for (t = 0; t <= distance + 1; t++) //画线输出
	{
		//LCD_Fast_DrawPoint(uRow, uCol, POINT_COLOR); //画点

		//x限幅
		if(uRow>800)
			uRow=800;
		//y限幅
		if(uCol>480)
			uCol=480;
		frame[BYTES_PIXEL*uRow + 800*BYTES_PIXEL*uCol] = POINT_COLOR&0x0000ff;
		frame[BYTES_PIXEL*uRow + 800*BYTES_PIXEL*uCol + 1] = (POINT_COLOR>>8)&0x0000ff;
		frame[BYTES_PIXEL*uRow + 800*BYTES_PIXEL*uCol + 2] = (POINT_COLOR>>16)&0x0000ff;


		xerr += delta_x;
		yerr += delta_y;
		if (xerr > distance) {
			xerr -= distance;
			uRow += incx;
		}
		if (yerr > distance) {
			yerr -= distance;
			uCol += incy;
		}
	}
	//刷新Cache，更新数据
	//Xil_DCacheFlush();

	return 0;
}
/**
  *****************************************************************************
  * @功能描述: 画线
  *
  * @输入参数: x1,y1:起点坐标
  * 		 x2,y2:终点坐标
  *****************************************************************************
**/
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint32_t color)
{
	uint16_t t;
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //计算坐标增量
	delta_y=y2-y1;
	uRow=x1;
	uCol=y1;
	if(delta_x>0)
		incx=1;  //设置单步方向
	else if(delta_x==0)
		incx=0;  //垂直线
	else
	{
		incx=-1;
		delta_x=-delta_x;
	}
	if(delta_y>0)
		incy=1;
	else if(delta_y==0)
		incy=0;  //水平线
	else
	{
		incy=-1;
		delta_y=-delta_y;
	}
	if(delta_x>delta_y)
		distance=delta_x;         //选取基本增量坐标轴
	else
		distance=delta_y;
	for(t=0;t<=distance+1;t++ )    //画线输出
	{
		frame[BYTES_PIXEL*uRow + 800*BYTES_PIXEL*uCol] = color&0x0000ff;
		frame[BYTES_PIXEL*uRow + 800*BYTES_PIXEL*uCol + 1] = (color>>8)&0x0000ff;
		frame[BYTES_PIXEL*uRow + 800*BYTES_PIXEL*uCol + 2] = (color>>16)&0x0000ff;

		xerr+=delta_x ;
		yerr+=delta_y ;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}

	//刷新Cache，更新数据
	//Xil_DCacheFlush();
}

/**
  *****************************************************************************
  * @功能描述: 画矩形
  *
  * @输入参数: (x1,y1),(x2,y2):矩形的对角坐标
  *****************************************************************************
**/
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color)
{
    LCD_DrawLine(x1, y1, x2, y1, color);
    LCD_DrawLine(x1, y1, x1, y2, color);
    LCD_DrawLine(x1, y2, x2, y2, color);
    LCD_DrawLine(x2, y1, x2, y2, color);
}

/**
  *****************************************************************************
  * @功能描述: 在指定位置显示一个字符
  *
  * @输入参数:	x,y:起始坐标
  * 		 	num:要显示的字符:" "--->"~"
  * 		 	size:字体大小 12/16/24/32
  * 			mode:叠加方式(1)还是非叠加方式(0)
  *
  * @Sample:	LCD_ShowChar(200, 50,'A',16,1);//在(200,50)显示字符'A'
  *****************************************************************************
**/
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode)
{
    uint8_t temp,t1,t;
    uint16_t y0=y;
    uint8_t csize=(size/8+((size%8)?1:0))*(size/2);   //得到字体一个字符对应点阵集所占的字节数
    num=num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
    for(t=0;t<csize;t++) {
        if(size==12)
            temp=asc2_1206[num][t];  //调用1206字体
        else if(size==16)
            temp=asc2_1608[num][t];  //调用1608字体
        else if(size==24)
            temp=asc2_2412[num][t];  //调用2412字体
        else if(size==32)
            temp=asc2_3216[num][t];  //调用3216字体
        else return;                 //没有的字库
        for(t1=0;t1<8;t1++) {
            if(temp&0x80)
			{
            	frame[BYTES_PIXEL*x + 800*BYTES_PIXEL*y] = POINT_COLOR&0x0000ff;
            	frame[BYTES_PIXEL*x + 800*BYTES_PIXEL*y + 1] = (POINT_COLOR>>8)&0x0000ff;
            	frame[BYTES_PIXEL*x + 800*BYTES_PIXEL*y + 2] = (POINT_COLOR>>16)&0x0000ff;
			}
            else if(mode==0)
            {
				frame[BYTES_PIXEL*x + 800*BYTES_PIXEL*y] = BACK_COLOR&0x0000ff;
				frame[BYTES_PIXEL*x + 800*BYTES_PIXEL*y + 1] = (BACK_COLOR>>8)&0x0000ff;
				frame[BYTES_PIXEL*x + 800*BYTES_PIXEL*y + 2] = (BACK_COLOR>>16)&0x0000ff;
            }
            temp<<=1;
            y++;
            if(y>=480)
                return;     //超区域了
            if((y-y0)==size) {
                y=y0;
                x++;
                if(x>=800)
                    return;  //超区域了
                break;
            }
        }
    }

    //刷新Cache，更新数据
    //Xil_DCacheFlush();
}

/**
  *****************************************************************************
  * @功能描述: 显示字符串
  *
  * @输入参数: x,y:起点坐标
  * 		 width,height:区域大小
  * 		 size:字体大小
  * 		 *p:字符串起始地址
  * 		 mode:叠加方式(1)还是非叠加方式(0)
  *****************************************************************************
**/
void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t mode,uint8_t *p)
{
    uint8_t x0=x;
    width+=x;
    height+=y;
    while((*p<='~')&&(*p>=' ')) {	//判断是不是非法字符!
        if(x>=width) {
            x=x0;
            y+=size;
        }
        if(y>=height)
            break;//退出
        LCD_ShowChar(x,y,*p,size,mode);
        x+=size/2;
        p++;
    }

}

/**
  *****************************************************************************
  * @功能描述: 计算m^n
  *
  * @输入参数: m,n
  *
  * @返回值   : m^n次方.
  *****************************************************************************
**/
uint32_t LCD_Pow(uint8_t m,uint8_t n)
{
    uint32_t result=1;
    while(n--)
        result*=m;
    return result;
}

/**
  *****************************************************************************
  * @功能描述: 显示数字,高位为0,不显示
  *
  * @输入参数: x,y:起点坐标
  * 		 len:长度(即要显示的位数)
  * 		 size:字体大小
  * 		 color:颜色
  * 		 num:数值(0~4294967295);
  *****************************************************************************
**/
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
{
    uint8_t t,temp;
    uint8_t enshow=0;
    for(t=0;t<len;t++) {
       temp=(num/LCD_Pow(10,len-t-1))%10;
       if(enshow==0&&t<(len-1))
       {
            if(temp==0)
            {
                LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
                continue;
            }else enshow=1;
       }
       LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0);
    }
}


/**
  *****************************************************************************
  * @功能描述: 显示数字,高位为0,显示0
  *
  * @输入参数: x,y:起点坐标
  * 		 num:数值(0~999999999);
  * 		 len:长度(即要显示的位数)
  * 		 size:字体大小
  * 		 mode[7]:0,不填充0;1,填充0.
  * 		 mode[1]:0,num=0显示0;1,num=0不显示
  * 		 [6:2]:保留
  * 		 mode[0]:0,非叠加显示;1,叠加显示.
  *****************************************************************************
**/
void LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode)
{
    uint8_t t,temp;
    uint8_t enshow=0;
    for(t=0;t<len;t++)
    {
       temp=(num/LCD_Pow(10,len-t-1))%10;
       if(enshow==0&&t<(len-1))
       {
            if(temp==0)
            {
                 if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);
                 else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);
                 continue;
            }else enshow=1;
       }
       if((mode&0X02)&&(!num))
    	   LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);
       else
    	   LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01);
    }
}

/**
  *****************************************************************************
  * @功能描述: 显示图片
  *
  * @输入参数: x,y:起点坐标
  * 		 pic:图片的数组指针(图片必须保留头部信息)
  *****************************************************************************
**/
void LCD_DisplayPic(uint16_t x, uint16_t y,const uint8_t *Pic)
{
	uint32_t width,height;
	uint32_t x_pos, y_pos;
	uint32_t Cnt = 0;

	width = (Pic[2]<<8) + Pic[3];//读取图片宽度
	height = (Pic[4]<<8) + Pic[5];//读取图片高度

	for(y_pos=y;y_pos<y+height;y_pos++)
	{
		for(x_pos=x;x_pos<x+width;x_pos++)
		{
			frame[BYTES_PIXEL*x_pos + 800*BYTES_PIXEL*y_pos] =  Pic[Cnt + 8];
			frame[BYTES_PIXEL*x_pos + 800*BYTES_PIXEL*y_pos + 1] = Pic[Cnt + 1 + 8];
			frame[BYTES_PIXEL*x_pos + 800*BYTES_PIXEL*y_pos + 2] = Pic[Cnt + 2 + 8];
			Cnt+=BYTES_PIXEL;
		}
	}

	//刷新Cache，更新数据
	LCD_Refresh();

}

