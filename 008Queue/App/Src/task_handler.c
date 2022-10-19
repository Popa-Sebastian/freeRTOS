/**
  ******************************************************************************
  * @file           : task_handler.c
  * @brief          :
  * @author         : Sebastian Popa, sebastian.popa@arobs.com
  * @date           : 2022
  ******************************************************************************
  */
/* Includes */
#include <string.h>

#include "main.h"
#include "task_handler.h"
#include "led_effect.h"

/* Imported data */
// HAL types
extern UART_HandleTypeDef huart1;

// Queues
extern QueueHandle_t q_data;
extern QueueHandle_t q_print;

// Tasks
extern TaskHandle_t _handle_menu_task;
extern TaskHandle_t _handle_cmd_task;
extern TaskHandle_t _handle_print_task;
extern TaskHandle_t _handle_led_task;
extern TaskHandle_t _handle_rtc_task;

/* Static Variable Declarations */
state_t _curr_state = MainMenu;

/* Static function prototypes */
static int extract_command(command_t *cmd);
static void proccess_command(command_t *cmd);

/* Function Declarations */
void menu_task(void *parameters)
{
    uint32_t cmd_addr;
    command_t *p_cmd;
    uint8_t option;

    const char * msg_menu =
            "\r\n"
            "=================\r\n"
            "|      Menu      |\r\n"
            "==================\r\n"
            "LED effect: 0\r\n"
            "Date and time: 1\r\n"
            "Exit: 2\r\n"
            "Input: ";

    const char * msg_inv = "Invalid message\r\n";

    while(1)
    {
        // Print Main Menu
        xQueueSend(q_print, &msg_menu, portMAX_DELAY);

        // Get user command by address using the notify value
        xTaskNotifyWait(0, 0, &cmd_addr, portMAX_DELAY);

        // Get pointer to the user command
        p_cmd = (command_t*)cmd_addr;

        if(p_cmd->len == 1)
        {
            option = p_cmd->payload[0] - '0';
            switch (option)
            {
            case 0:
                _curr_state = LedEffect;
                xTaskNotify(_handle_led_task, 0, eNoAction);
                break;
            case 1:
                _curr_state = RtcMenu;
                xTaskNotify(_handle_rtc_task, 0, eNoAction);
                break;
            case 2: //exit
                break;
            default:
                xQueueSend(q_print, &msg_inv, portMAX_DELAY);
                break;
            }
        }
        else
        {
            xQueueSend(q_print, &msg_inv, portMAX_DELAY);
            continue;
        }

        // Wait for a new entry to Main Menu, triggered by another task
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
    }
}

void cmd_task(void *parameters)
{
    BaseType_t ret;
    command_t command_msg = {0};

    while(1)
    {
        // Wait for command
        ret = xTaskNotifyWait(0x00, 0x00, NULL, portMAX_DELAY);
        if (ret == pdTRUE)
        {
            // Process command and Notify relevant task
            proccess_command(&command_msg);
        }
    }
}

void print_task(void *parameters)
{
    uint32_t *msg;
    while(1)
    {
        xQueueReceive(q_print, &msg, portMAX_DELAY);
        HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen((char*)msg), HAL_MAX_DELAY);
    }
}

void led_task(void *parameters)
{
    uint32_t cmd_addr;
    command_t *p_cmd;
    uint8_t option;

    const char * msg_menu =
            "\r\n"
            "=================\r\n"
            "|      LED       |\r\n"
            "==================\r\n"
            "0 = turn off\r\n"
            "1 = turn on\r\n"
            "Input: ";

    const char * msg_inv = "Invalid message\r\n";

    while(1)
    {
        // Wait for entry in LED Menu
        xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);

        // Print Main Menu
        xQueueSend(q_print, &msg_menu, portMAX_DELAY);

        // Get user command by address using the notify value
        xTaskNotifyWait(0, 0, &cmd_addr, portMAX_DELAY);

        // Get pointer to the user command
        p_cmd = (command_t*)cmd_addr;

        if(p_cmd->len == 1)
        {
           option = p_cmd->payload[0] - '0';
           switch (option)
           {
           case 0:
               led_off();
               break;
           case 1:
               led_on();
               break;
           default:
               xQueueSend(q_print, &msg_inv, portMAX_DELAY);
               break;
           }
        }
        else
        {
           xQueueSend(q_print, &msg_inv, portMAX_DELAY);
        }

        // Go to Main Menu
        _curr_state = MainMenu;
        xTaskNotify(_handle_menu_task, 0, eNoAction);
    }
}

void rtc_task(void *parameters)
{
    while(1)
    {

    }
}

static int extract_command(command_t *cmd)
{
    uint8_t item;
    BaseType_t status;

    // Check for messages
    status = uxQueueMessagesWaiting(q_data);
    if(!status)
    {
        return -1;
    }

    uint8_t index = 0;
    do
    {
        status = xQueueReceive(q_data, &item, 0);
        if (status == pdTRUE)
        {
            cmd->payload[index] = item;
            index++;
        }
    } while (item != '\n');

    cmd->payload[index - 1] = '\0';
    cmd->len = index -1;

    return 0;
}

static void proccess_command(command_t *cmd)
{
    extract_command(cmd);

    switch(_curr_state)
    {
    case MainMenu:
        xTaskNotify(_handle_menu_task, (uint32_t)cmd, eSetValueWithOverwrite);
        break;
    case LedEffect:
        xTaskNotify(_handle_led_task, (uint32_t)cmd, eSetValueWithOverwrite);
        break;
    case RtcMenu:
    case RtcTimeConfig:
    case RtcDateConfig:
    case RtcReport:
        xTaskNotify(_handle_rtc_task, (uint32_t)cmd, eSetValueWithOverwrite);
        break;
    }
}
/* End of File */
