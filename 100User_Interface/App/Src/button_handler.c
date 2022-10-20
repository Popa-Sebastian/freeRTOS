/*
 * button_handler.c
 *
 *  Created on: Oct 20, 2022
 *      Author: sebastian.popa
 */

#include "main.h"
#include "log.h"

/* Static Variable Declarations */

/* Static function prototypes */
static void _button_press_log(uint16_t GPIO_Pin)
{
    switch (GPIO_Pin) {
    case BTN_UP_Pin:
        log_msg_fromISR("UP button press");
        break;
    case BTN_DN_Pin:
        log_msg_fromISR("DOWN button press");
        break;
    case BTN_ENT_Pin:
        log_msg_fromISR("ENTER button press");
        break;
    default:
        log_msg_fromISR("button press not recognized");
        break;
    }
}

/* Function Declarations */
void button_press_isr_handler(uint16_t GPIO_Pin)
{
    _button_press_log(GPIO_Pin);
}

/* End of File */
