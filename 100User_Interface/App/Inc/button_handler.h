/*
 * button_handler.h
 *
 *  Created on: Oct 20, 2022
 *      Author: sebastian.popa
 */

#ifndef INC_BUTTON_HANDLER_H_
#define INC_BUTTON_HANDLER_H_

/* Includes */

/* Defines */
#define BUTTON_PRESS_DEBOUNCE_MS    200

/* Structs */
typedef struct
{
    uint32_t lastButtonPress;
    uint32_t currentButtonPress;
} sButtonDebounce;

/* Enums */

/* Global variables */

/* Public Function Declaration */
void button_press_isr_handler(uint16_t GPIO_Pin);

#endif /* INC_BUTTON_HANDLER_H_ */
