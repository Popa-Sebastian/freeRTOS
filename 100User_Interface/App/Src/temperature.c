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
    sMenu* menu = LCD_GetMenuInstance();

    TEMPSENSOR_ReadTempData(&outputValue);
    sprintf(menu->temperature, "%d.%dC", (uint16_t) (outputValue / 10), (uint8_t)(outputValue % 10));
    log_msg(menu->temperature);
}

/* End of File */
