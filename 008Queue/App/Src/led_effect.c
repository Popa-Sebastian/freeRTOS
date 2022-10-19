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

/* Module Log_Level Priority */

/* Static Variable Declarations */

/* Static function prototypes */

/* Function Declarations */
void led_on(void)
{
    HAL_GPIO_TogglePin(LEDR_GPIO_Port, LEDR_Pin);
}

/* End of File */
