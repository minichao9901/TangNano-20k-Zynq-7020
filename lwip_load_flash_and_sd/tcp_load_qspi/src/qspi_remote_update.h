#ifndef SRC_QSPI_REMOTE_UPDATE_H_
#define SRC_QSPI_REMOTE_UPDATE_H_

#include "xparameters.h"
#include "xtime_l.h"
#include "xstatus.h"
#include <stdio.h>

//�������˿�
#define SER_PORT            6789
//���յ�����ļ���С16MB
#define MAX_FLASH_LEN       16*1024*1024

int qspi_init();
int qspi_update(u32 total_bytes, const u8 *flash_data);
void process_print(u8 percent);
void sent_msg(const char *msg);
float  get_time_s();

#endif
