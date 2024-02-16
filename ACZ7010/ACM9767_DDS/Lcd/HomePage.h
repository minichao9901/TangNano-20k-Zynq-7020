#ifndef LCD_HOMEPAGE_H_
#define LCD_HOMEPAGE_H_

#include "Touch.h"
#include "lcd.h"

#define Up_Line1		4		//�׿��ϱ߽�Y����
#define Down_Line1		54		//�׿��±߽�Y����
#define Left_Line1		2		//�׿���߽�X����
#define Right_Line1		42		//�׿��ұ߽�X����

#define Up_Line2		4		//�׿��ϱ߽�Y����
#define Down_Line2		54		//�׿��±߽�Y����
#define Left_Line2		45		//�׿���߽�X����
#define Right_Line2		96		//�׿��ұ߽�X����

#define Up_Line3		6		//�׿��ϱ߽�Y����
#define Down_Line3		12		//�׿��±߽�Y����
#define Left_Line3		47		//�׿���߽�X����
#define Right_Line3		90		//�׿��ұ߽�X����

#define FREQ1_X			17
#define FREQ1_Y			30
#define FREQ2_X			30
#define FREQ2_Y			30

//��ť������ؽṹ�嶨��
typedef struct {
	uint16_t X1;	//��������x1
	uint16_t X2;	//��������x2
	uint16_t Y1;	//��������y1
	uint16_t Y2;	//��������y2

} Button_XY;

//�ı�������ؽṹ�嶨��
typedef struct {
	float X;	//�ı�����x
	float Y;	//�ı�����y

} Text_XY;

void Draw_Button(Button_XY Button_XY);
void Draw_Small_Button(Button_XY Button_XY);
void Fill_Button(Button_XY Button_XY,uint32_t Color);
void WR_Text(Text_XY Text_XY, uint8_t *TEXT, uint8_t SIZE, uint8_t MODE);
void WR_Num(Text_XY Text_XY, uint32_t NUM, uint8_t LEN);
uint8_t Judge_Button(Button_XY Button_XY);
void Press_Button(Button_XY Button_XY);
void Release_Button(Button_XY Button_XY);

#endif /* LCD_HOMEPAGE_H_ */
