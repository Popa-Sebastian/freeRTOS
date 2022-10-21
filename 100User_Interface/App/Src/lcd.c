/**
  ******************************************************************************
  * @file           : lcd.c
  * @brief          :
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

/* Static Variable Declarations */
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
static void _lcd_display_test(void);

/* Function Declarations */
void LCD_Init(void)
{
    LCD_DRIVER_Init();
}

void LCD_Display(void)
{
    _lcd_display_menu();
}

__unused static void _lcd_display_test(void)
{
    static uint32_t count = 0;
    static char msg[20];
    LCD_DRIVER_Write_String(0, 0, "0 Hello!");

    sprintf(msg, "1 %lu", count);

    LCD_DRIVER_Write_String(0, 1, msg);
    count++;

    LCD_DRIVER_Write_String(0, 2, "2");
    LCD_DRIVER_Write_String(0, 3, "3");
    LCD_DRIVER_Write_String(0, 4, "abcdefghijklmn");
    LCD_DRIVER_Write_String(0, 5, "0123456789ABCD");
}

__unused static void _lcd_display_menu(void)
{
    // Header Row
    LCD_DRIVER_Write_String(0, 0, _menuInstance.menuText);
    LCD_DRIVER_Write_String((9 * LCD_CHAR_PX_WIDTH ), 0, _menuInstance.temperature);

    // Option Rows
    for (uint8_t rowIndex = 0; rowIndex < LCD_MAX_OPTIONS; rowIndex++)
    {
        if (_menuInstance.cursorPos == rowIndex)
        {
            LCD_DRIVER_Write_String(0, rowIndex + OPTION_ROW_OFFSET, ">");
        }
        else
        {
            LCD_DRIVER_Write_String(0, rowIndex + OPTION_ROW_OFFSET, " ");
        }

        LCD_DRIVER_Write_String(LCD_CHAR_PX_WIDTH, rowIndex + OPTION_ROW_OFFSET, _menuInstance.optionText[rowIndex]);
    }
}

/* End of File */
