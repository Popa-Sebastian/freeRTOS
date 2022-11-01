/**
  ******************************************************************************
  * @file           : op_time.c
  * @brief          :
  * @author         : Sebastian Popa, sebastian.popa@raptor-technologies.ro
  * @date           : Nov 1, 2022
  ******************************************************************************
  */
/* Includes */
#include <stdint.h>
#include <stdio.h>

#include "main.h"
#include "lcd.h"
#include "log.h"

extern SemaphoreHandle_t _mutex_display;
/* Static Variable Declarations */

/* Static function prototypes */

/* Function Declarations */
void OPTIME_UpdateOpTime(void)
{
    TickType_t tick = xTaskGetTickCount();
    uint32_t ms = tick % 1000;
    uint32_t s = tick / 1000;
    uint32_t m = s / 60;

    xSemaphoreTake(_mutex_display, portMAX_DELAY);

    sMenu* menu = LCD_GetMenuInstance();
    sprintf(menu->optionText[0], "%02lu:%02lu.%01lu ", m % 60, s % 60, ms / 100);

    xSemaphoreGive(_mutex_display);
}

/* End of File */
