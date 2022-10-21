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

/* Static variables */
static sButtonDebounce _upBtn = {0};
static sButtonDebounce _downBtn = {0};
static sButtonDebounce _enterBtn = {0};

/* Static function prototypes */
static void _button_press_log(uint16_t GPIO_Pin);
static bool _is_button_debounced(uint16_t GPIO_Pin);
static void _button_update_cursor_pos(uint16_t GPIO_Pin);

/* Static Function Declaration */
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

static bool _is_button_debounced(uint16_t GPIO_Pin)
{
    uint32_t tick = xTaskGetTickCount();
    bool ret = false;

    switch (GPIO_Pin)
    {
    case BTN_UP_Pin:
    {
        _upBtn.currentButtonPress = tick;
        ret = ((_upBtn.currentButtonPress - _upBtn.lastButtonPress) > BUTTON_PRESS_DEBOUNCE_MS);
        _upBtn.lastButtonPress = _upBtn.currentButtonPress;
    }
    break;
    case BTN_DN_Pin:
    {
        _downBtn.currentButtonPress = tick;
        ret = ((_downBtn.currentButtonPress - _downBtn.lastButtonPress) > BUTTON_PRESS_DEBOUNCE_MS);
        _downBtn.lastButtonPress = _downBtn.currentButtonPress;
    }
    break;
    case BTN_ENT_Pin:
    {
        _enterBtn.currentButtonPress = tick;
        ret = ((_enterBtn.currentButtonPress - _enterBtn.lastButtonPress) > BUTTON_PRESS_DEBOUNCE_MS);
        _enterBtn.lastButtonPress = _enterBtn.currentButtonPress;
    }
    break;
    }

    return ret;
}

static void _button_update_cursor_pos(uint16_t GPIO_Pin)
{
    sMenu *menu = LCD_GetMenuInstance();

    switch (GPIO_Pin) {
    case BTN_UP_Pin:
        if (_is_button_debounced(GPIO_Pin))
        {
            menu->cursorPos = menu->cursorPos > 0 ? menu->cursorPos - 1 : 0;
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
}

/* Public Function Declarations */
void button_press_isr_handler(uint16_t GPIO_Pin)
{
    BaseType_t xHigherPriorityTaskWoken;

    _button_press_log(GPIO_Pin);
    _button_update_cursor_pos(GPIO_Pin);

    // Notify display task
    xTaskNotifyFromISR(_handle_display_task, 0, eNoAction, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/* End of File */
