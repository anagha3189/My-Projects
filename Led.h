#ifndef __LED_TASK_H__
#define __LED_TASK_H__


#define LEDTASKSTACKSIZE        128        
#define LED_TOGGLE_DELAY        250

extern void LED_Init(void);
extern void LEDTask(void *arg);
void LED_On(void);
void LED_Off(void);

#endif
