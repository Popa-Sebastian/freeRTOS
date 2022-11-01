/**
  ******************************************************************************
  * @file           : lcd.c
  * @brief          : Display functions for NOKIA_5110 LCD Display
  * @author         : Sebastian Popa, sebastian.popa@raptor-technologies.ro
  * @date           : Oct 20, 2022
  ******************************************************************************
  */

/* Includes */
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "nokia_5110.h"
#include "lcd.h"
#include "log.h"

extern SemaphoreHandle_t _mutex_display;

/* Global Variable Declarations */
static const sMenu _menuInfo[MENU_COUNT] =
{
    {
        .menuState = MAIN_MENU,
        .menuText = "Main",
        .optionText =
        {
                "Temperature",
                "LEDs",
                "Op Time",
                "Logging",
        },
        .cursorPos = 0,
        .menuMaxOptions = 4,
    },

    {
        .menuState = TEMPERATURE_MENU,
        .menuText = "Temp",
        .optionText =
        {
                "Celsius",
                "Farenheit",
                "Back",
        },
        .cursorPos = 0,
        .menuMaxOptions = 3,
    },

    {
        .menuState = LED_MENU,
        .menuText = "LEDs",
        .optionText =
        {
                "Pattern 1",
                "Pattern 2",
                "Pattern 3",
                "Back",
        },
        .cursorPos = 0,
        .menuMaxOptions = 4,
    },

    {
        .menuState = OP_TIME_MENU,
        .menuText = "Time",
        .optionText =
        {
                "",
                "Back",
        },
        .cursorPos = 1,
        .menuMaxOptions = 2,
    },

    {
        .menuState = LOG_MENU,
        .menuText = "Log",
        .optionText =
        {
                "Enable",
                "Disable",
                "Back",
        },
        .cursorPos = 0,
        .menuMaxOptions = 3,
    },
};

static sMenu _menuInstance = _menuInfo[MAIN_MENU];

/* Static function prototypes */
static void _lcd_display_menu(void);

/* Function Declarations */
void LCD_Init(void)
{
    LCD_DRIVER_Init();
}

void LCD_Display(void)
{
    _lcd_display_menu();
}

/* Static functions */
/**
 * @brief displays current menu
 */
static void _lcd_display_menu(void)
{
    xSemaphoreTake(_mutex_display, portMAX_DELAY);

    LCD_DRIVER_Clear();

    // Header Row
    LCD_DRIVER_Write_String(0, 0, _menuInstance.menuText);
    LCD_DRIVER_Write_String((9 * LCD_CHAR_PX_WIDTH ), 0, _menuInstance.temperature);

    // Option Rows
    for (uint8_t rowIndex = 0; rowIndex < _menuInstance.menuMaxOptions; rowIndex++)
    {
        // Display Cursor
        if (_menuInstance.cursorPos == rowIndex)
        {
            LCD_DRIVER_Write_String(0, rowIndex + OPTION_ROW_OFFSET, ">");
        }
        else
        {
            LCD_DRIVER_Write_String(0, rowIndex + OPTION_ROW_OFFSET, " ");
        }

        // Display Option Text
        LCD_DRIVER_Write_String(LCD_CHAR_PX_WIDTH, rowIndex + OPTION_ROW_OFFSET, _menuInstance.optionText[rowIndex]);
    }

    xSemaphoreGive(_mutex_display);
}

void LCD_SwitchMenu(eMenuOptions option)
{
    memset(&_menuInstance, 0x00, sizeof(sMenu));
    memcpy(&_menuInstance, &_menuInfo[option], sizeof(sMenu));
}

sMenu* LCD_GetMenuInstance()
{
    return &_menuInstance;
}


/* End of File */
