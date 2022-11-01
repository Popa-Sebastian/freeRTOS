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
typedef enum
{
    MAIN_MENU = 0x00,
    TEMPERATURE_MENU = 0x01,
    LED_MENU = 0x02,
    OP_TIME_MENU = 0x03,
    LOG_MENU = 0x04,

    MENU_COUNT,
} eMenuOptions;

/* Global variables */
typedef struct
{
    uint8_t menuState;
    char menuText[LCD_MAX_LENGTH + 1];
    char temperature[TEMP_MAX_SIZE + 1];
    char optionText[LCD_MAX_OPTIONS][LCD_MAX_LENGTH];
    uint8_t cursorPos;
    uint8_t menuMaxOptions;
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

void LCD_SwitchMenu(eMenuOptions option);

#endif /* INC_LCD_H_ */

/* End of File */
