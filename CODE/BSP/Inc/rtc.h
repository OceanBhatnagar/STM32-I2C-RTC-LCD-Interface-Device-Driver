/*
 * rtc.h
 *
 *  Created on: Mar 31, 2025
 *      Author: Ocean
 */

#ifndef INC_RTC_H_
#define INC_RTC_H_

#include<stm32f4xx_hal.h>
#define DS1307_I2C I2C2
#define DS1307_I2C_SDA_PIN  GPIO_PIN_11
#define DS1307_I2C_SCL_PIN   GPIO_PIN_10
#define DS1307_I2C_GPIO_PORT  GPIOB
#define DS1307_I2C_SPEED  100000
#define DS1307_I2C_PUPD    GPIO_PULLUP


#define ADDR_SEC    0x00
#define ADDR_MIN     0x01
#define ADDR_HRS     0x02
#define ADDR_DAY     0x03
#define ADDR_DATE    0x04
#define ADDR_MONTH   0x05
#define ADDR_YEAR    0x06

#define TIME_FORMAT_12HRS_AM  0
#define TIME_FORMAT_12HRS_PM  1
#define TIME_FORMAT_24HRS     2

#define DAY_FORMAT_SUNDAY     1
#define DAY_FORMAT_MONDAY     2
#define DAY_FORMAT_TUESDAY    3
#define DAY_FORMAT_WEDNESDAY  4
#define DAY_FORMAT_THURSDAY   5
#define DAY_FORMAT_FRIDAY     6
#define DAY_FORMAT_SATURSDAY  7

#define DS1307_I2C_ADDR    0x68

typedef struct{
	uint8_t date;
	uint8_t month;
	uint8_t year;
	uint8_t day;
}RTC_date_t;

typedef struct
{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t year;
	uint8_t time_format;
}RTC_time_t;

//FUNCTION PROTOTYPES

uint8_t ds1307_init(void);
void ds1307_set_current_time(RTC_time_t *rtc_time);

void ds1307_get_current_time(RTC_time_t *rtc_time);

void ds1307_set_current_date(RTC_date_t *rtc_date);

void ds1307_get_current_date(RTC_date_t *rtc_date);




#endif /* INC_RTC_H_ */
