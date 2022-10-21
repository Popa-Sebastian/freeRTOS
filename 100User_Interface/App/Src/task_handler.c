/**
  ******************************************************************************
  * @file           : task_handler.c
  * @brief          :
  * @author         : Sebastian Popa, sebastian.popa@arobs.com
  * @date           : 2022
  ******************************************************************************
  */
/* Includes */
#include <string.h>

#include "main.h"
#include "task_handler.h"
#include "lcd.h"
#include "log.h"

/* Imported data */
extern TaskHandle_t _handle_log_task;
extern TaskHandle_t _handle_display_task;

/* Queue handles */
extern QueueHandle_t _q_log;

/* HAL Types */
extern UART_HandleTypeDef huart1;
#define UART_HANDLER    huart1

/* Static Variable Declarations */

/* Static function prototypes */

/* Function Declarations */
void log_task(void *parameters)
{
    uint32_t *msg;
    while(1)
    {
        xQueueReceive(_q_log, &msg, portMAX_DELAY);
        HAL_UART_Transmit(&UART_HANDLER, (uint8_t*)msg, strlen((char*)msg), HAL_MAX_DELAY);
    }
}

void display_task(void *parameters)
{
    while(1)
    {
        LCD_Display();
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
    }
}

/* End of File */
