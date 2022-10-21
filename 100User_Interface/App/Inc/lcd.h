/**
  ******************************************************************************
  * @file           : lcd.h
  * @brief          : Display functions for NOKIA_5110 LCD Display
  * @author         : Sebastian Popa, sebastian.popa@raptor-technologies.ro
  * @date           : Oct 20, 2022
  ******************************************************************************
  */

#ifndef INC_LCD_H_
#define INC_LCD_H_

/* Includes */

/* Defines */
#define LCD_MAX_LENGTH      14
#define LCD_MAX_HEIGHT      6
#define LCD_MAX_OPTIONS     4
#define TEMP_MAX_SIZE       5

#define OPTION_ROW_OFFSET   2

#define LCD_CHAR_PX_WIDTH   6
#define LCD_CHAR_PX_HEIGHT  8

/* Enums */

/* Global variables */
typedef struct
{
    char menuText[LCD_MAX_LENGTH + 1];
    char temperature[TEMP_MAX_SIZE + 1];
    char optionText[LCD_MAX_OPTIONS][LCD_MAX_LENGTH];
    uint8_t cursorPos;
} sMenu;

/* Public Function Declaration */
/**
 * @brief Initializes LCD Nokia Display
 */
void LCD_Init(void);

/**
 * @brief Prints to Nokia Display the Menu context based on _menuInstance struct
 */
void LCD_Display(void);

/**
 * @brief Exports the _menuInstance struct
 */
sMenu* LCD_GetMenuInstance();

#endif /* INC_LCD_H_ */

/* End of File */
