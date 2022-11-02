/**
  ******************************************************************************
  * @file           : temperature.h
  * @brief          : Temperature sensor API
  * @author         : Sebastian Popa, sebastian.popa@raptor-technologies.ro
  * @date           : Oct 21, 2022
  ******************************************************************************
  */

#ifndef INC_TEMPERATURE_H_
#define INC_TEMPERATURE_H_

/* Includes */

/* Defines */

/* Enums */
typedef enum
{
    TEMP_CELSIUS = 0x00,
    TEMP_FARENHEIT = 0x01,
} eTempUnit;

/* Global variables */

/* Public Function Declaration */
void TEMP_Init(void);
void TEMP_GetTemperature(void);
void TEMP_SetTemperatureUnit(eTempUnit unit);

#endif /* INC_TEMPERATURE_H_ */

/* End of File */
