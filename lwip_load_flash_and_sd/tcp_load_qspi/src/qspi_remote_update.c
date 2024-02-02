#include <stdio.h>
#include <string.h>
#include "lwip/err.h"
#include "lwip/tcp.h"
#include "qspi_remote_update.h"
#include "xil_printf.h"

u8 start_update_flag = 0;
u8 rxbuffer[MAX_FLASH_LEN];
u32 total_bytes = 0;
struct tcp_pcb *c_pcb;

void print_app_header()
{
    xil_printf("\r\n-----QSPI remote update ------\r\n");
}

//进度打印
void process_print(u8 percent)
{
    switch (percent) {
    case 0:
        sent_msg("0%..");
        xil_printf("0%%..");
        break;
    case 1:
        sent_msg("10%..");
        xil_printf("10%%..");
        break;
    case 2:
        sent_msg("20%..");
        xil_printf("20%%..");
        break;
    case 3:
        sent_msg("30%..");
        xil_printf("30%%..");
        break;
    case 4:
        sent_msg("40%..");
        xil_printf("40%%..");
        break;
    case 5:
        sent_msg("50%..");
        xil_printf("50%%..");
        break;
    case 6:
        sent_msg("60%..");
        xil_printf("60%%..");
        break;
    case 7:
        sent_msg("70%..");
        xil_printf("70%%..");
        break;
    case 8:
        sent_msg("80%..");
        xil_printf("80%%..");
        break;
    case 9:
        sent_msg("90%..");
        xil_printf("90%%..");
        break;
    case 10:
        sent_msg("100%\r\n");
        xil_printf("100%%\r\n");
    default:
        break;
    }
}

//将接收到的BOOT.bin文件写入到QSPI中
int transfer_data()
{
    char msg[60];
    if (start_update_flag) {
        xil_printf("\r\nStart QSPI Update!\r\n");
        xil_printf("file size of BOOT.bin is %lu Bytes\r\n", total_bytes);
        sprintf(msg, "file size of BOOT.bin is %lu Bytes\r\n",total_bytes);
        sent_msg(msg);
        if (qspi_update(total_bytes, rxbuffer) != XST_SUCCESS){
            sent_msg("Update Qspi Error!\r\n");
            xil_printf("Update Qspi Error!\r\n");
        }
        else
            total_bytes = 0;
    }

    start_update_flag = 0;

    return 0;
}

//获取当前时间（单位秒sec）
float get_time_s()
{
    XTime tCur = 0;
    XTime_GetTime(&tCur);
    return (tCur / (float) COUNTS_PER_SECOND);
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
static err_t recv_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
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
        sent_msg("\r\nStart QSPI Update\r\n");
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
    c_pcb = newpcb;             //保存连接的客户端PCB
    //设置接收回调
    tcp_recv(c_pcb, recv_callback);
    tcp_arg(c_pcb, NULL);

    return ERR_OK;
}

int start_application()
{
    struct tcp_pcb *pcb;
    err_t err;

    //Qspi初始化
    err = qspi_init();
    if (err != XST_SUCCESS) {
        xil_printf("QSPI init Failed\r\n");
        return XST_FAILURE;
    }
    xil_printf("Successfully init QSPI\r\n");

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
