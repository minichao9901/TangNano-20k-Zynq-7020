#ifndef SRC_LOG_H_
#define SRC_LOG_H_

#include "COMMON.h"
#include "xuartps.h"
#include "TTC.h"


#define	LOG_DEBUG	1
#define DEBUG_UART_INT_ID		XPS_UART1_INT_ID
#define DEBUG_UART_DEV_ID		XPAR_PS7_UART_1_DEVICE_ID
#define	DEBUG_UART_BAUDRATE		115200
#define	LOGW(x, ...)			do{ \
									if(LOG_DEBUG) { \
										p_log = log_text + log_cnt; \
										sprintf(p_log,x,##__VA_ARGS__); \
										log_cnt += strlen(log_text + log_cnt); \
									} \
								} while (0)


extern char log_text[4096];
extern char *p_log;
extern uint32_t log_cnt;
extern XUartPs Debug_UART;

void Log_Debug_Init();
void Debug_UART_IRQ_Handler(void *CallBackRef);
void Debug_LOG_IRQ_Handler(void *CallBackRef);
void LOGP();
void Debug_UART_Init();
void Debug_UART_SendBuffer(XUartPs *InstancePtr);


#endif /* SRC_LOG_H_ */
