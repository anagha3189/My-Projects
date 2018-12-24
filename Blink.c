#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <timers.h>
#include "inc/tm4c123gh6pm.h"
#include "Blink.h"
#include "UARTSerial.h"
#include "LED.h"


void LEDTask(void *arg)
{
	(void)arg;
	while(1)
	{
		LED_On();
		vTaskDelay(LED_TOGGLE_DELAY);
		LED_Off();
		vTaskDelay(LED_TOGGLE_DELAY);
	}
}

void LEDsTask(void *arg)
{
	
	(void)arg;
    
  Color_t currentColor = GREEN;
    
	while(1) 
  {
        if(xQueueReceive(colorQueue,&currentColor,0) == pdTRUE)
        {
            RED_Write(1); BLUE_Write(1); GREEN_Write(1);
        }
        if(currentColor == RED)
            RED_Write(~RED_Read());
        else if (currentColor == BLUE)
            BLUE_Write(~BLUE_Read());
        else
            GREEN_Write(~GREEN_Read());
        
        vTaskDelay(500);
	}
	
}

void UART_Poll_Task(void *arg)
{
	 (void)arg;
    static char buff[500];
    UART0_PutString("Started UART\n\r");
    char c;
    
	while(1) 
	{
        c = UART0_InChar();
        switch(c)
        {
            case 'a':
            UART0_PutString("Working\n\r");
            break;
            
            case 't':
                UART0_PutString("********************************************\n\r");
                UART0_PutString("Task          State   Prio    Stack    Num\n\r"); 
                UART0_PutString("********************************************\n\r");
                //vTaskList(buff);
                UART0_PutString(buff);
                UART0_PutString("*********************************************\n\r");
            break;
        }
        taskYIELD();
    }
}

void UART_Intr_Task(void* arg)
{
	  (void)arg;
    static char buff[500];
    UART1_PutString("Started UART\n\r");
    char c;
    Color_t tempColor;
    
	  while(1) 
		{    
        xSemaphoreTake(uartSemaphore,portMAX_DELAY);
        while( (c = UART1_InChar()) )
        {
            switch(c)
            {
                case 'a':
                UART1_PutString("Working\n\r");
                break;
                
                case 't':
                    UART1_PutString("********************************************\n\r");
                    UART1_PutString("Task          State   Prio    Stack    Num\n\r"); 
                    UART1_PutString("********************************************\n\r");
                    //vTaskList(buff);
                    UART1_PutString(buff);
                    UART1_PutString("*********************************************\n\r");
       
                break;
                    
                case 'r':
                    tempColor = RED;
                    if(xQueueSend(colorQueue,&tempColor,0) != pdTRUE)
                        UART1_PutString("queue error\n\r");
                break;
                    
                case 'b':
                    tempColor = BLUE;
                    if(xQueueSend(colorQueue,&tempColor,0) != pdTRUE)
                        UART1_PutString("queue error\n\r");
                break;

                case 'g':
                    tempColor = GREEN;
                    if(xQueueSend(colorQueue,&tempColor,0) != pdTRUE)
                        UART1_PutString("queue error\n\r");
                break;
            }
        }
        UART_SetRxInterruptMode(UART_INTR_RX_NOT_EMPTY);
    }
}




