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

/* Function Declarations */
void log_msg(char const* fmt, ...)
{
    static char textmsg[40];
    static char message[80];
    static char *msg = message;

    // Get time stamp
    TickType_t tick = xTaskGetTickCount();
    uint32_t ms = tick % 1000;
    uint32_t s = tick / 1000;
    uint32_t m = s / 60;
    sprintf(message, "%02lu:%02lu.%03lu ", m % 60, s % 60, ms);

    // Get text
    va_list arg;
    va_start(arg, fmt);
    vsprintf((char*)textmsg, fmt, arg);
    va_end(arg);
    strcat(message, textmsg);

    // Append end line
    strcat(message, "\r\n");

    // Send to print task
    xQueueSend(_q_log, &msg, portMAX_DELAY);
}

/* End of File */
