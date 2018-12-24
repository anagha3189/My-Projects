#ifndef __BLINK_H__
#define __BLINK_H__

#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <timers.h>

QueueHandle_t colorQueue;
SemaphoreHandle_t uartSemaphore;

typedef enum Color_t {
    RED,
    GREEN,
    BLUE
} Color_t;

#endif