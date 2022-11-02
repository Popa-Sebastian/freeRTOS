/*
 * log.c
 *
 *      Author: sebastian.popa
 */

/* Includes */
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "main.h"
#include "log.h"

/* Defines */

/* Imports */
extern QueueHandle_t _q_log;
extern SemaphoreHandle_t _mutex_log;

static char _textmsg[40];
static char _message[80];

static bool _isPrintEnabled = true;

static void _create_msg(char const* fmt, ...)
{
    // Get time stamp
    TickType_t tick = xTaskGetTickCount();
    uint32_t ms = tick % 1000;
    uint32_t s = tick / 1000;
    uint32_t m = s / 60;
    sprintf(_message, "%02lu:%02lu.%03lu ", m % 60, s % 60, ms);

    // Get text
    va_list arg;
    va_start(arg, fmt);
    vsprintf((char*)_textmsg, fmt, arg);
    va_end(arg);
    strcat(_message, _textmsg);

    // Append end line
    strcat(_message, "\r\n");
}


/* Function Declarations */
void log_msg(char const* fmt, ...)
{
    if(_isPrintEnabled)
    {
        xSemaphoreTake(_mutex_log, portMAX_DELAY);

        static char *msg = _message;

        va_list arg;
        va_start(arg, fmt);
        _create_msg(fmt, arg);
        va_end(arg);

        // Send to print task
        xQueueSend(_q_log, &msg, portMAX_DELAY);

        xSemaphoreGive(_mutex_log);
    }
}

void log_msg_fromISR(char const* fmt, ...)
{
    if(_isPrintEnabled)
    {
        static char *msg = _message;

        va_list arg;
        va_start(arg, fmt);
        _create_msg(fmt, arg);
        va_end(arg);

        // Send to print task
        xQueueSendFromISR(_q_log, &msg, NULL);
    }
}

void log_setPrintState(bool state)
{
    portENTER_CRITICAL();
    _isPrintEnabled = state;
    portEXIT_CRITICAL();
}

/* End of File */
