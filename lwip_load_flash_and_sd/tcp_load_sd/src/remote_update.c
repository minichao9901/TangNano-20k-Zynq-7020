/*
 * remote_update.c
 *
 *  Created on: 2024年2月2日
 *      Author: minichao
 */

#include "remote_update.h"

#include "xparameters.h"

#include "ff.h"

#include "string.h"

#include <stdio.h>

#include "lwip/err.h"

#include "lwip/tcp.h"

#include "xil_printf.h"

u8 start_update_flag = 0;

u8 rxbuffer[MAX_FLASH_LEN];

u32 total_bytes = 0;

#define FILE_NAME "BOOT.bin"

struct tcp_pcb *c_pcb;

FATFS fs;

void print_app_header()

{

	xil_printf("-----SD remote update demo------\n");

}

//挂载sd卡

void sd_mount() {

	FRESULT status;

	BYTE work[FF_MAX_SS];

//挂载sd卡，注册文件系统对象

	status = f_mount(&fs, "", 1);

	if (status != FR_OK) {

		printf("%d\n", status);

		printf("It isn't FAT format\n");

		f_mkfs("", FM_FAT32, 0, work, sizeof work);

		f_mount(&fs, "", 1);

	}

}

//写数据

void sd_write_data(u8 wr_dat[], u32 wr_len) {

	FIL fil;

	UINT bw;

//创建或者打开文件

	f_open(&fil, FILE_NAME, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);

//移动读写指针

	f_lseek(&fil, 0);

//写数据

	f_write(&fil, wr_dat, wr_len, &bw);

//关闭文件

	f_close(&fil);

}

//将接收到的BOOT.bin文件写入到SD中

int transfer_data()

{

	char msg[60];

	if (start_update_flag) {

		xil_printf("\r\nStart SD Update!\r\n");

		xil_printf("file size of BOOT.bin is %lu Bytes\r\n", total_bytes);

		sprintf(msg, "file size of BOOT.bin is %lu Bytes\r\n", total_bytes);

		sent_msg(msg);

		sd_write_data(rxbuffer, total_bytes);

		xil_printf("SD Update finish!\n");

		total_bytes = 0;

	}

	start_update_flag = 0;

	return 0;

}

//向客户端回送信息

void sent_msg(const char *msg)

{

	err_t err;

	tcp_nagle_disable(c_pcb);

	if (tcp_sndbuf(c_pcb) > strlen(msg)) {

		err = tcp_write(c_pcb, msg, strlen(msg), TCP_WRITE_FLAG_COPY);

		if (err != ERR_OK)

			xil_printf("tcp_server: Error on tcp_write: %d\r\n", err);

		err = tcp_output(c_pcb);

		if (err != ERR_OK)

			xil_printf("tcp_server: Error on tcp_output: %d\r\n", err);

	} else

		xil_printf("no space in tcp_sndbuf\r\n");

}

//接收回调函数

static err_t recv_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p,
		err_t err)

{

	struct pbuf *q;

	if (!p) {

		tcp_close(tpcb);

		tcp_recv(tpcb, NULL);

		xil_printf("tcp connection closed\r\n");

		return ERR_OK;

	}

	q = p;

	if (q->tot_len == 6 && !(memcmp("update", p->payload, 6))) {

		start_update_flag = 1;

		sent_msg("\r\nStart SD Update\r\n");

	} else if (q->tot_len == 5 && !(memcmp("clear", p->payload, 5))) {

		start_update_flag = 0;

		total_bytes = 0;

		sent_msg("Clear received data\r\n");

		xil_printf("Clear received data\r\n");

	} else {

		while (q->tot_len != q->len) {

			memcpy(&rxbuffer[total_bytes], q->payload, q->len);

			total_bytes += q->len;

			q = q->next;

		}

		memcpy(&rxbuffer[total_bytes], q->payload, q->len);

		total_bytes += q->len;

	}

	tcp_recved(tpcb, p->tot_len);

	pbuf_free(p);

	return ERR_OK;

}

err_t accept_callback(void *arg, struct tcp_pcb *newpcb, err_t err)

{

	xil_printf("tcp_server: Connection Accepted\r\n");

	c_pcb = newpcb; //保存连接的客户端PCB

//设置接收回调

	tcp_recv(c_pcb, recv_callback);

	tcp_arg(c_pcb, NULL);

	return ERR_OK;

}

int start_application()
{

	struct tcp_pcb *pcb;

	err_t err;

//挂载SD卡

	sd_mount();

	xil_printf("Successfully init SD\r\n");

	print_app_header();

//创建TCP PCB

	pcb = tcp_new_ip_type(IPADDR_TYPE_ANY);

	if (!pcb) {

		xil_printf("Error creating PCB. Out of Memory\n\r");

		return -1;

	}

//绑定端口号

	err = tcp_bind(pcb, IP_ANY_TYPE, SER_PORT);

	if (err != ERR_OK) {

		xil_printf("Unable to bind to port %d: err = %d\n\r", SER_PORT, err);

		return -2;

	}

//此处不需要回调函数的任何参数

	tcp_arg(pcb, NULL);

//侦听连接

	pcb = tcp_listen(pcb);

	if (!pcb) {

		xil_printf("Out of memory while tcp_listen\n\r");

		return -3;

	}

//指定用于传入连接的回调

	tcp_accept(pcb, accept_callback);

	xil_printf("TCP server started @ port %d\n\r", SER_PORT);

	return 0;

}
