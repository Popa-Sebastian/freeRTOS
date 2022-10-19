/**
  ******************************************************************************
  * @file           : task_handler.c
  * @brief          :
  * @author         : Sebastian Popa, sebastian.popa@arobs.com
  * @date           : 2022
  ******************************************************************************
  */
/* Includes */
#include "main.h"
#include "task_handler.h"
#include "led_effect.h"

/* Imported data */
extern QueueHandle_t q_data;
extern QueueHandle_t q_print;

extern TaskHandle_t _handle_menu_task;
extern TaskHandle_t _handle_cmd_task;
extern TaskHandle_t _handle_print_task;
extern TaskHandle_t _handle_led_task;
extern TaskHandle_t _handle_rtc_task;

extern TimerHandle_t _handle_led_timer;

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

    const char * msg_menu = "=================\n"
                            "|      Menu      |\n"
                            "==================\n"
                            "LED effect: 0\n"
                            "Date and time: 1\n"
                            "Exit: 2\n"
                            "Input: ";

    const char * msg_inv = "Invalid message\n";

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
    while(1)
    {

    }
}

void led_task(void *parameters)
{
    uint32_t cmd_addr;
    command_t *p_cmd;
    uint8_t option;

    const char * msg_menu = "=================\n"
                            "|      LED       |\n"
                            "==================\n"
                            "0 = turn off\n"
                            "1 = turn on\n"
                            "Input: ";
    const char * msg_inv = "Invalid message\n";

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
               xTimerStop(_handle_led_timer, portMAX_DELAY);
               break;
           case 1:
               xTimerStart(_handle_led_timer, portMAX_DELAY);
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
