#ifndef REMOTE_UPDATE_H_

#define REMOTE_UPDATE_H_

#include "xparameters.h"

#include "xtime_l.h"

#include "xstatus.h"

#include <stdio.h>

//�������˿�

#define SER_PORT 5678

//���յ�����ļ���С16MB

#define MAX_FLASH_LEN 16*1024*1024

void sent_msg(const char *msg);
int start_application();
void print_app_header();

#endif
