#ifndef ACZ702_LIB_SCU_GIC_H_
#define ACZ702_LIB_SCU_GIC_H_


#include "COMMON.h"


#define	HIGH_Level_Sensitive	0x01	//高电平敏感(1)
#define	Rising_Edge_Sensitive	0x03	//上升沿敏感(0->1)

XScuGic ScuGic;	//通用中断控制器

void ScuGic_Init();
void Set_ScuGic_Link(uint16_t IntrId, uint8_t Priority, uint8_t Trigger,
		Xil_InterruptHandler Handler, void *CallBackRef);

#endif /* ACZ702_LIB_SCU_GIC_H_ */
