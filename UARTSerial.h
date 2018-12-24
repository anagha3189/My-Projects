#ifndef __UART_TASK_H__
#define __UART_TASK_H__

#define UARTTASKSTACKSIZE 256
#define UARTTASKPRIORITY   

extern void UART_Init(void);
extern void UARTTask(void *arg);

#endif
