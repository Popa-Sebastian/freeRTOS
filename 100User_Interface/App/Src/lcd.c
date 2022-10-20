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
#include "log.h"

/* Static Variable Declarations */

/* Static function prototypes */
static void _lcd_display_test(void);

/* Function Declarations */
void LCD_Init(void)
{
    LCD_DRIVER_Init();
}

void LCD_Display(void)
{
    _lcd_display_test();
}

static void _lcd_display_test(void)
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

/* End of File */
