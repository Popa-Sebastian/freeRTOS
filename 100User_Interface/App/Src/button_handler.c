/*
 * button_handler.c
 *
 *  Created on: Oct 20, 2022
 *      Author: sebastian.popa
 */
#include <stdbool.h>

#include "main.h"
#include "button_handler.h"
#include "lcd.h"
#include "log.h"

/* Imports */
extern TaskHandle_t _handle_log_task;
extern TaskHandle_t _handle_display_task;
extern TaskHandle_t _handle_button_task;
extern SemaphoreHandle_t _mutex_display;

/* Static variables */
static sButtonDebounce _upBtn = {0};
static sButtonDebounce _downBtn = {0};
static sButtonDebounce _enterBtn = {0};

/* Static function prototypes */
static void _button_press_log(uint16_t GPIO_Pin);
static bool _is_button_debounced(uint16_t GPIO_Pin);
static void _button_update_cursor_pos(uint16_t GPIO_Pin);

/* Static Function Declaration */
/**
 * @brief logs to print queue button pressed.
 *        calls isr safe api @ref log_msg_fromISR()
 */
static void _button_press_log(uint16_t GPIO_Pin)
{
    switch (GPIO_Pin) {
    case BTN_UP_Pin:
        log_msg("UP button press");
        break;
    case BTN_DN_Pin:
        log_msg("DOWN button press");
        break;
    case BTN_ENT_Pin:
        log_msg("ENTER button press");
        break;
    default:
        log_msg("button press not recognized");
        break;
    }
}

/**
 * @brief software button debouncing
 */
static bool _is_button_debounced(uint16_t GPIO_Pin)
{
    sButtonDebounce *current_button = 0;

    switch (GPIO_Pin)
    {
    case BTN_UP_Pin:
        current_button = &_upBtn;
        break;
    case BTN_DN_Pin:
        current_button = &_downBtn;
        break;
    case BTN_ENT_Pin:
        current_button = &_enterBtn;
        break;
    default:
        configASSERT(0);
    }

    current_button->currentButtonPress = xTaskGetTickCount();

    if((current_button->currentButtonPress - current_button->lastButtonPress) > BUTTON_PRESS_DEBOUNCE_MS)
    {
        current_button->lastButtonPress = current_button->currentButtonPress;
        return true;
    }

    return false;
}

/**
 * @brief updates menu instance cursor position
 */
static void _button_update_cursor_pos(uint16_t GPIO_Pin)
{
    xSemaphoreTake(_mutex_display, portMAX_DELAY);
    sMenu *menu = LCD_GetMenuInstance();

    switch (GPIO_Pin) {
    case BTN_UP_Pin:
        if (_is_button_debounced(GPIO_Pin))
        {
            menu->cursorPos = menu->cursorPos > 0 ? menu->cursorPos - 1 : LCD_MAX_OPTIONS - 1;
        }
        break;
    case BTN_DN_Pin:
        if (_is_button_debounced(GPIO_Pin))
        {
            menu->cursorPos = (menu->cursorPos + 1) % LCD_MAX_OPTIONS;
        }
        break;
    case BTN_ENT_Pin:
        if (_is_button_debounced(GPIO_Pin))
        {
            __NOP();
        }
        break;
    default:
        break;
    }

    xSemaphoreGive(_mutex_display);
}

/* Public Function Declarations */
void button_press_isr_handler(uint16_t GPIO_Pin)
{
    BaseType_t xHigherPriorityTaskWoken;

    // Notify display task
    xTaskNotifyFromISR(_handle_button_task, (uint32_t)GPIO_Pin, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void button_handler(uint16_t GPIO_Pin)
{
    _button_update_cursor_pos(GPIO_Pin);
    _button_press_log(GPIO_Pin);
}

/* End of File */
