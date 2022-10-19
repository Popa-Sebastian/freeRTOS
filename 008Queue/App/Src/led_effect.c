/**
  ******************************************************************************
  * @file           : led_effect.c
  * @brief          :
  * @author         : Sebastian Popa, sebastian.popa@arobs.com
  * @date           : 2022
  ******************************************************************************
  */
/* Includes */
#include "main.h"
#include "led_effect.h"

// Timers
extern TimerHandle_t _handle_led_timer;
/* Module Log_Level Priority */

/* Static Variable Declarations */

/* Static function prototypes */

/* Function Declarations */
void led_off(void)
{
    HAL_GPIO_WritePin(LEDR_GPIO_Port, LEDR_Pin, GPIO_PIN_RESET);
    xTimerStop(_handle_led_timer, portMAX_DELAY);
}

void led_on(void)
{
    xTimerStart(_handle_led_timer, portMAX_DELAY);
}

void toggle_led(void)
{
    HAL_GPIO_TogglePin(LEDR_GPIO_Port, LEDR_Pin);
}

/* End of File */
