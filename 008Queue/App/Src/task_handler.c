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

/* Imported data */
extern QueueHandle_t q_data;

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
    while(1)
    {

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
    while(1)
    {

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
