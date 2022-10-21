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

/* Global Variable Declarations */
static sMenu _menuInstance =
{
        .menuText = "Main",
        .temperature = "23.5C",
        .optionText =
        {
                "Option 1",
                "Option 2",
                "Option 3",
                "Option 4",
        },
        .cursorPos = 0
};

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
    // Header Row
    LCD_DRIVER_Write_String(0, 0, _menuInstance.menuText);
    LCD_DRIVER_Write_String((9 * LCD_CHAR_PX_WIDTH ), 0, _menuInstance.temperature);

    // Option Rows
    for (uint8_t rowIndex = 0; rowIndex < LCD_MAX_OPTIONS; rowIndex++)
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
}

sMenu* LCD_GetMenuInstance()
{
    return &_menuInstance;
}

/* End of File */
