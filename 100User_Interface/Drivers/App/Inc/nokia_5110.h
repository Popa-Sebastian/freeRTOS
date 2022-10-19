/*
 * nokia_5110.h
 *
 *  Created on: Oct 19, 2022
 *      Author: sebastian.popa
 */

#ifndef APP_INC_NOKIA_5110_H_
#define APP_INC_NOKIA_5110_H_

#include <stdint.h>

#define LCD_RESET_PIN GPIO_PIN_0 // LCD reset pin.
#define LCD_RESET_PORT GPIOA

#define LCD_CE_PIN GPIO_PIN_1 // Use hardware SPI NSS pin or define here.
#define LCD_CE_PORT GPIOA

#define LCD_DC_PIN GPIO_PIN_1 // Data_Command select pin.
#define LCD_DC_PORT GPIOC

#define LCD_SPI_INTERFACE hspi2 // LCD SPI interface.

#define LCD_MAX_TIMEOUT_TICKS 1000

void LCD_Init(void);
void LCD_Reset(void);
void LCD_Write_Command(uint8_t cmd);
void LCD_Write_Data(uint8_t data);
void LCD_Set_Postion(uint8_t PosX, uint8_t PosY);
void LCD_Clear(void);
void LCD_Write_Char(uint8_t ch);
void LCD_Write_String(uint8_t PosX, uint8_t PosY, char * str);
void LCD_Callback(void);

#endif /* APP_INC_NOKIA_5110_H_ */
