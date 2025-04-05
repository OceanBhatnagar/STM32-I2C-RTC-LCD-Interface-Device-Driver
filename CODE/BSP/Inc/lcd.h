/*
 * lcd.h
 *
 *  Created on: Mar 31, 2025
 *      Author: Ocean
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_




#include<stdint.h>
#include<stm32f4xx_hal.h>
#define SLAVE_ADDRESS_LCD 0x27

// LCD command definitions        //DB:7654 3210
#define LCD_CMD_CLEAR_DISPLAY 0x01   //0000 0001
#define LCD_CMD_RETURN_HOME 0x02     //0000 001X
#define LCD_CMD_ENTRY_MODE_SET 0x06  //0000 0110
#define LCD_CMD_DISPLAY_ON 0x0C
#define LCD_CMD_DISPLAY_OFF 0x08
#define LCD_CMD_FUNCTION_SET 0x28
#define LCD_CMD_SET_CURSOR 0x80
#define LCD_CMD_INIT_8_BIT_MODE 0x30
#define LCD_CMD_INIT_4_BIT_MODE 0x20

//FUNCTION PROTOTYPE

void lcd_init(void);

HAL_StatusTypeDef lcd_send_cmd(uint8_t cmd);
void lcd_send_string(char *str);
void lcd_clear(void);
void lcd_put_cursor(int row,int col);
void lcd_display_return_home(void);

// Set the I2C handle to be used
extern I2C_HandleTypeDef hi2c1;
#define I2CPIN1 hi2c1

#endif /* INC_LCD_H_ */
