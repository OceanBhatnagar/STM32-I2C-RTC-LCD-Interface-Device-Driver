/*
 * lcd.c
 *
 *  Created on: Mar 31, 2025
 *      Author: Ocean
 */



#include<lcd.h>
#include<stm32f4xx_hal.h>



// Function to initialize the LCD
void lcd_init(void)
{
    // 4-bit initialization
    HAL_Delay(50);  // Wait for >40ms
    lcd_send_cmd(0x30);
    HAL_Delay(5);   // Wait for >4.1ms
    lcd_send_cmd(0x30);
    HAL_Delay(1);   // Wait for >100us
    lcd_send_cmd(0x30);
    HAL_Delay(10);
    lcd_send_cmd(0x20);  // 4-bit mode
    HAL_Delay(10);

    // Display initialization
    lcd_send_cmd(LCD_CMD_FUNCTION_SET);  // Function set: DL=0 (4-bit mode), N=1 (2-line display), F=0 (5x8 characters)
    HAL_Delay(1);
    lcd_send_cmd(LCD_CMD_DISPLAY_OFF);   // Display off
    HAL_Delay(1);
    lcd_send_cmd(LCD_CMD_CLEAR_DISPLAY); // Clear display
    HAL_Delay(2);
    lcd_send_cmd(LCD_CMD_ENTRY_MODE_SET); // Entry mode set: I/D=1 (increment cursor), S=0 (no shift)
    HAL_Delay(1);
    lcd_send_cmd(LCD_CMD_DISPLAY_ON);    // Display on: D=1, C=0, B=0 (Cursor and blink)
    HAL_Delay(1);
}
//FUNCTION TO SEND COMMAND
HAL_StatusTypeDef lcd_send_cmd(uint8_t cmd)
{
	uint8_t data_u, data_l;
    uint8_t data_t[4];

    data_u = (cmd & 0xF0); // Upper nibble
    data_l = ((cmd << 4) & 0xF0); // Lower nibble

    data_t[0] = data_u | 0x0C; // Enable high, RS low
    data_t[1] = data_u | 0x08; // Enable low, RS low
    data_t[2] = data_l | 0x0C; // Enable high, RS low
    data_t[3] = data_l | 0x08; // Enable low, RS low

    if (HAL_I2C_Master_Transmit(&I2CPIN1, SLAVE_ADDRESS_LCD << 1, (uint8_t *)data_t, 4, HAL_MAX_DELAY) != HAL_OK)
    {
        // Handle error
        return HAL_ERROR;
    }
    return HAL_OK;
}

// Function to send data to the LCD
HAL_StatusTypeDef lcd_send_data(uint8_t data)
{
	uint8_t data_u, data_l;
    uint8_t data_t[4];

    data_u = (data & 0xF0); // Upper nibble
    data_l = ((data << 4) & 0xF0); // Lower nibble
    //UPPER NIBBLE DATA FLOW
    data_t[0] = data_u | 0x0D; // Enable high, RS high
    data_t[1] = data_u | 0x09; // Enable low, RS high
    //LOWER NIBBLE DATA FLOW
    data_t[2] = data_l | 0x0D; // Enable high, RS high
    data_t[3] = data_l | 0x09; // Enable low, RS high

    if (HAL_I2C_Master_Transmit(&I2CPIN1, SLAVE_ADDRESS_LCD << 1, (uint8_t *)data_t, 4, HAL_MAX_DELAY) != HAL_OK)
    {
        // Handle error
        return HAL_ERROR;
    }
    return HAL_OK;
}
//FUNCTION CLEAR DISPLAY
void lcd_clear(void)
{
    if (lcd_send_cmd(LCD_CMD_CLEAR_DISPLAY) != HAL_OK)
    {
        // Handle error
    }
    HAL_Delay(50); // Wait for the command to complete
}

// Function to send a string to the LCD
void lcd_send_string(char *str)
{
    while (*str) lcd_send_data(*str++); // Send each character of the string
}

//CURSOR RETURN TO HOME POSITION
void lcd_display_return_home(void){
	lcd_send_cmd(LCD_CMD_RETURN_HOME);
	HAL_Delay(2);
}

// Function to set the cursor position on the LCD
void lcd_put_cursor(int row, int col)
{
    if (row == 0)
    {
        col |= LCD_CMD_SET_CURSOR; // Set position for row 0   //1000 0000(Set DDRAM ADDRESS)
    }
    else if (row == 1)
    {
        col |= (LCD_CMD_SET_CURSOR | 0x40); // Set position for row 1  //1100 0000(Set DDRAM ADDRESS)
    }

    lcd_send_cmd(col); // Send command to set cursor position
}
