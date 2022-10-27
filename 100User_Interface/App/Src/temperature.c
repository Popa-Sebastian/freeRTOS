/**
  ******************************************************************************
  * @file           : temperature.c
  * @brief          : Temperature sensor API
  * @author         : Sebastian Popa, sebastian.popa@raptor-technologies.ro
  * @date           : Oct 21, 2022
  ******************************************************************************
  */

/* Includes */
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "main.h"
#include "temp_sensor.h"
#include "lcd.h"
#include "log.h"

extern SemaphoreHandle_t _mutex_display;

/* Static Variable Declarations */

/* Static function prototypes */

/* Function Declarations */
void TEMP_Init(void)
{
    TEMPSENSOR_Init();
}

void TEMP_GetTemperature(void)
{
    int16_t outputValue;
    TEMPSENSOR_ReadTempData(&outputValue);
    char current_temperature[6];

    xSemaphoreTake(_mutex_display, portMAX_DELAY);

    sMenu* menu = LCD_GetMenuInstance();
    sprintf(menu->temperature, "%d.%dC", (uint16_t) (outputValue / 10), (uint8_t)(outputValue % 10));

    strcpy(current_temperature, menu->temperature);

    xSemaphoreGive(_mutex_display);

    log_msg(current_temperature);
}

/* End of File */
