#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <timers.h>
#include "inc/tm4c123gh6pm.h"
#include "Blink.h"

int main()
{

	#ifdef POLL
	LED_Init();
	UART0_Init();
	#endif
	
	#ifdef INTR
	LEDs_Init();
	UART1_Init();
	#endif
	
	#ifdef POLL
	/* Create the LED task. */
  if(xTaskCreate(LEDTask, (const portCHAR *)"Blink LED", LEDTASKSTACKSIZE, NULL, 1, NULL) != pdTRUE)
  {
        return(1);
  }
	/* Create UART Poll Task which will control the serial port */
  if(xTaskCreate(UART_Poll_Task,  (const portCHAR *)"UART0", UARTTASKSTACKSIZE, NULL, 1, NULL) != pdTRUE)
	{
			  return(1);
	}
	#endif
	
	#ifdef INTR
	colorQueue = xQueueCreate(1, sizeof(Color_t)); // 1 item queue that can hold colors
	/* Create the LED task. */
  if(xTaskCreate(LEDsTask, (const portCHAR *)"Blink LEDs", LEDTASKSTACKSIZE, NULL, 1, NULL) != pdTRUE)
  {
        return(1);
  }
	/* Create UART Interrupt task which will control the serial port */
	if(xTaskCreate(UART_Intr_Task,  (const portCHAR *)"UART1", UARTTASKSTACKSIZE, NULL, 1, NULL) != pdTRUE)
	{
			  return(1);
	}
	#endif
	
	vTaskStartScheduler();
	while(1);
}








































