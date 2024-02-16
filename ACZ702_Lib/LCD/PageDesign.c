#include "PageDesign.h"
#include "COMMON.h"

//���Ʒ���
void Draw_Box(Box_XY Box, uint32_t Color,int16_t Offset)
{
	LCD_DrawRectangle(Box.X1 + Offset, Box.Y1 + Offset,
			Box.X1 + Box.Width - Offset, Box.Y1 + Box.Height - Offset, Color);
}

//�������
void Fill_Box(Box_XY Box,uint32_t BackColor,int16_t Offset)
{
	LCD_Color_Fill(Box.X1+Offset,Box.Y1+Offset,
			Box.X1+Box.Width-Offset,Box.Y1+Box.Height-Offset,BackColor);
}

//ͨ��Wave���ڵĲ��ֲ������Զ���ȫ��������
void Get_Waveform_Window_Parameters(Wave_Page *Wave)
{
	Wave->Wave_Area.X1 = Wave->Window.X1 + 40;			//���㲨�������X1����
	Wave->Wave_Area.Y1 = Wave->Window.Y1 + 10;			//���㲨�������Y1����
	Wave->Wave_Area.Width = Wave->Window.Width - 65;	//���㲨������Ŀ��
	Wave->Wave_Area.Height = Wave->Window.Height - 35;	//���㲨������ĸ߶�
	//���η�Χ���Ը߶ȵó�ÿ��λ��ѹֵ��������ش�С
	Wave->Precision = Wave->Wave_Area.Height / (float)((1 << Wave->ADC_Bits)*2);
}

//�������δ��ں�����
void Draw_Waveform_Windows(Wave_Page Wave, uint32_t Sample_Rate)
{
	Display_Time_Scale(Wave,Sample_Rate);	//��ʾX���ʱ��̶�
	Draw_Grid_Background(Wave);				//��ʾ���񱳾�
	Display_Voltage_Scale(Wave);			//��ʾY��ĵ�ѹ�̶�
}

//���Ʋ���
void Draw_Waveform(Wave_Page Wave,int16_t *Pre_Wave_Data,int16_t *Wave_Data)
{
	uint16_t i,len;
	uint16_t XAxis_Y;

	len = Wave.Wave_Area.Width;
	XAxis_Y = Wave.Wave_Area.Y1 + Wave.Wave_Area.Height/2;

	//��ȥ�ɲ��β������²���
	for(i=0;i<len-1;i++)
	{
		POINT_COLOR = PAGE_COLOR;
		LCD_DrawWaveLine(i + Wave.Wave_Area.X1, XAxis_Y - (int16_t)(Wave.Precision*Pre_Wave_Data[i]),
				i + 1 + Wave.Wave_Area.X1, XAxis_Y - (int16_t)(Wave.Precision*Pre_Wave_Data[i+1]));
		POINT_COLOR = LCD_YELLOW;
		LCD_DrawWaveLine(i + Wave.Wave_Area.X1, XAxis_Y - (int16_t)(Wave.Precision*Wave_Data[i]),
				i + 1 + Wave.Wave_Area.X1, XAxis_Y - (int16_t)(Wave.Precision*Wave_Data[i+1]));
	}

	LCD_Refresh();//ˢ��ͼ��
}

//�������񱳾�
void Draw_Grid_Background(Wave_Page Wave)
{
	uint8_t i,j;
	//�������񱳾�
	for(i=0;i<11;i++) {
		for(j=0;j<51;j++) {
			LCD_DrawPoint(Wave.Wave_Area.X1 + j*Wave.Wave_Area.Width/50,
					Wave.Wave_Area.Y1 + i*Wave.Wave_Area.Height/10,LCD_WHITE);//���ƺ�����
			LCD_DrawPoint(Wave.Wave_Area.X1 + i*Wave.Wave_Area.Width/10,
					Wave.Wave_Area.Y1 + j*Wave.Wave_Area.Height/50,LCD_WHITE);//����������
			if(i==5) {
				LCD_DrawLine(Wave.Wave_Area.X1 + j*Wave.Wave_Area.Width/50,//���ƺ�����
						Wave.Wave_Area.Y1 + i*Wave.Wave_Area.Height/10 - 1,
						Wave.Wave_Area.X1 + j*Wave.Wave_Area.Width/50,
						Wave.Wave_Area.Y1 + i*Wave.Wave_Area.Height/10 + 1,LCD_WHITE);
				LCD_DrawLine(Wave.Wave_Area.X1 + i*Wave.Wave_Area.Width/10 - 1,//����������
						Wave.Wave_Area.Y1 + j*Wave.Wave_Area.Height/50,
						Wave.Wave_Area.X1 + i*Wave.Wave_Area.Width/10 + 1,
						Wave.Wave_Area.Y1 + j*Wave.Wave_Area.Height/50,LCD_WHITE);
			}
		}
	}

	//LCD_Refresh();//ˢ��ͼ��
}

//��ʾX���ʱ��̶�
void Display_Time_Scale(Wave_Page Wave,uint32_t Sample_Rate)
{
	char str[30];
	uint8_t i;
	uint16_t Byte;
	uint16_t X_Offset;
	uint16_t Y_Offset = 10;
	Box_XY Box_TimeScale;	//�̶�����
	Box_TimeScale.X1 = Wave.Window.X1 + 1;
	Box_TimeScale.Y1 = Wave.Wave_Area.Y1 + Wave.Wave_Area.Height + Y_Offset;
	Box_TimeScale.Width = Wave.Window.Width - 2;
	Box_TimeScale.Height = 12;
	POINT_COLOR = LCD_WHITE;//X������ɫ
	BACK_COLOR = PAGE_COLOR;//��ȵı�����ɫ��ҳ��һ��
	//���ԭ�еĿ̶�
	Fill_Box(Box_TimeScale,PAGE_COLOR, 0);
	//��ʾX���ʱ��̶�
	for(i=0;i<11;i++){
		if(i <= 5)
			sprintf(str,"%gms",(float)(i-5)*Wave.Wave_Area.Width/(Sample_Rate*10));
		else
			sprintf(str,"+%gms",(float)(i-5)*Wave.Wave_Area.Width/(Sample_Rate*10));//�Ӹ�����

		//��ȡ�ַ������ȣ�ʹ�̶ȱ�ע����������ж���
		Byte = strlen(str);
		X_Offset = Byte*3;//һ���ַ������6��������Ҫƫ��һ�룬�������3
		LCD_ShowString(Wave.Wave_Area.X1 + i * Wave.Wave_Area.Width/10 - X_Offset,
				Wave.Wave_Area.Y1 + Wave.Wave_Area.Height + Y_Offset,
				150, 30, 12, 0, (uint8_t*)str);
	}

	LCD_Refresh();//ˢ��ͼ��
}
//��ʾY��ĵ�ѹ�̶�
void Display_Voltage_Scale(Wave_Page Wave)
{
	uint8_t i;
	char str[30];

	//��ʾY��ĵ�ѹ�̶�
	for (i=0;i<11;i++) {
		if (i < 5)
			sprintf(str,"+%.1fV",Wave.Range - i * Wave.Range/5);
		else if(i == 5)
			sprintf(str," %.1fV",Wave.Range - i * Wave.Range/5);
		else
			sprintf(str,"%.1fV",Wave.Range - i * Wave.Range/5);

		LCD_ShowString(Wave.Window.X1+5,
				Wave.Window.Y1+5 + i * Wave.Wave_Area.Height/10,
				150, 30, 12, 0, (uint8_t*)str);
	}

	LCD_Refresh();//ˢ��ͼ��
}

//������ͨ����
void Draw_Normal_Button(Button Button)
{
	uint8_t Byte = 0;
	uint8_t Rows = 0;
	uint8_t i;
	uint16_t X_Offset,Y_Offset;
	uint8_t Border_LineWidth = 2;//�����߿���߿�
	POINT_COLOR = Button.TextColor;
	BACK_COLOR = Button.BackColor;

	//����ɫ�߿�
	for(i=0;i<Border_LineWidth;i++)
		Draw_Box(Button.Box, LCD_WHITE, i);

	//������ɫ�߿�
	for(i=Border_LineWidth;i<Border_LineWidth*2;i++)
		Draw_Box(Button.Box,PAGE_COLOR, i);


	//��䰴������
	Fill_Box(Button.Box,Button.BackColor,Border_LineWidth*2);

	//����Text������
	while (strlen(Button.Text[Rows]))
		Rows++;

	for(i=0;i<Rows;i++){
		Byte = strlen(Button.Text[i]);
		X_Offset = (Button.Box.Width - Byte*Button.TextSize/2)/2;
		Y_Offset = (Button.Box.Height - Button.TextSize*Rows)/(Rows+1) + Button.TextSize*i;
		LCD_ShowString(Button.Box.X1+X_Offset,Button.Box.Y1+Y_Offset,
				150,Button.TextSize+2,Button.TextSize,0,(uint8_t*)Button.Text[i]);
	}

	//LCD_Refresh();//ˢ��ͼ��
}

//������ͨ�ı�
void Draw_Normal_Text(Text Text, char *str)

{
	uint16_t Byte;
	uint16_t X_Offset,Y_Offset;
	Box_XY Box = Text.Box;

	Byte = strlen(str);
	X_Offset = (Text.Box.Width - Byte*Text.TextSize/2)/2;
	Y_Offset = (Text.Box.Height - Text.TextSize)/2;

	POINT_COLOR = Text.TextColor;
	BACK_COLOR = Text.BackColor;
	Box.Width = X_Offset;
	Fill_Box(Box,Text.BackColor,0);//�Ȳ���
	Box.X1 = Text.Box.X1 + Text.Box.Width - X_Offset;
	Fill_Box(Box,Text.BackColor,0);//�Ȳ���
	LCD_ShowString(Text.Box.X1 + X_Offset,Text.Box.Y1 + Y_Offset,
			Text.Box.Width,Text.TextSize+2,Text.TextSize,0,(uint8_t*)str);

	//LCD_Refresh();//ˢ��ͼ��
}

//���ư�����Ч
void Draw_Button_Effect(Button Button)
{
	uint8_t Byte = 0;
	uint8_t Rows = 0;
	uint8_t i;
	uint16_t X_Offset,Y_Offset;
	uint8_t Border_LineWidth = 2;	//�����߿���߿�ֵԽ����Խ��
	uint8_t Effect_Width = 3;	//������Ч��Ч����ֵԽ����ЧԽ����
	POINT_COLOR = Button.TextColor;
	BACK_COLOR = Button.BackColor;

	//������
	for(i=0;i<Border_LineWidth*4;i++)
		Draw_Box(Button.Box, PAGE_COLOR, i);

	//����ɫ�߿�
	for(i=0;i<Border_LineWidth;i++)
		Draw_Box(Button.Box, LCD_WHITE, i+Effect_Width);

	//��䰴������
	Fill_Box(Button.Box,Button.BackColor,Border_LineWidth*2+Effect_Width);

	//����Text������
	while (strlen(Button.Text[Rows]))
		Rows++;

	for(i=0;i<Rows;i++){
		Byte = strlen(Button.Text[i]);
		X_Offset = (Button.Box.Width - Byte*Button.TextSize/2)/2;
		Y_Offset = (Button.Box.Height - Button.TextSize*Rows)/(Rows+1) + Button.TextSize*i;
		LCD_ShowString(Button.Box.X1+X_Offset,Button.Box.Y1+Y_Offset,
				150,Button.TextSize+2,Button.TextSize,0,(uint8_t*)Button.Text[i]);
	}

	LCD_Refresh();//ˢ��ͼ��
}

//�������Ƶı��ߣ�DirectionΪ0��ʾ���ߣ�Ϊ1��ʾ����
void Draw_Mark_Line(uint8_t Direction,uint16_t Point, uint16_t Start,uint16_t End,uint32_t Color)
{
	uint8_t i;
	uint8_t Size = 8;


	if(Direction) {
		LCD_DrawLine(Point,Start,Point,End,Color);//������
		//�����Ǳ�
		for(i=0;i<Size;i++) {
			LCD_DrawLine(Point-i,Start-i,Point+i,Start-i,Color);//����������
			LCD_DrawLine(Point-i,Start-i,Point,Start,Color);//����\��
			LCD_DrawLine(Point,Start,Point+i,Start-i,Color);//����/��
		}
	}
	else {
		LCD_DrawLine(Start,Point,End,Point,Color);//������
		//�����Ǳ�
		for(i=0;i<Size;i++) {
			LCD_DrawLine(End+i,Point-i,End+i,Point+i,Color);//����|��
			LCD_DrawLine(End+i,Point-i,End,Point,Color);//����/��
			LCD_DrawLine(End,Point,End+i,Point+i,Color);//����\��
		}
	}
}
