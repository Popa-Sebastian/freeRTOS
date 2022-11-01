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
#include "temperature.h"
#include "lcd.h"
#include "log.h"

extern SemaphoreHandle_t _mutex_display;

/* Static Variable Declarations */
static eTempUnit _tempUnit = TEMP_CELSIUS;

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
    if(_tempUnit == TEMP_CELSIUS)
    {
        sprintf(menu->temperature, "%d.%dC", (uint16_t) (outputValue / 10), (uint8_t)(outputValue % 10));
    }
    else
    {
        uint16_t farenheitVal = (outputValue * 9 / 5) + 320;
        sprintf(menu->temperature, "%dF", (uint16_t) (farenheitVal / 10));
    }

    strcpy(current_temperature, menu->temperature);

    xSemaphoreGive(_mutex_display);

    log_msg(current_temperature);
}

void TEMP_SetTemperatureUnit(eTempUnit unit)
{
    if (unit > TEMP_FARENHEIT)
    {
        return;
    }

    _tempUnit = unit;
}

/* End of File */
