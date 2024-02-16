#include "Digital_Tube_8Bits.h"
#include "sleep.h"

//���ڴ洢����ܵĶ�ѡ����ֵ������Ϊ0,1,2,3,4,5,6,7,8,9,-,.
const uint8_t Seg_Code[12] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xBF,0x7F};

int main(void)
{
    uint8_t i;

    while(1) {
        for(i=0;i<8;i++) {
            DT8_Display_Ctr((0xFF << (7-i)));//�������ʾ����
            Set_DT8_Display_Val((7-i), Seg_Code[i]);//�������ʾ
            sleep(1);
        }
    }

    return 0;
}
