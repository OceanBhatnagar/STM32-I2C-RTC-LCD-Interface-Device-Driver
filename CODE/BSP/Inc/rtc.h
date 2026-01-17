/*
 * rtc.h
 *
 *  Created on: Mar 31, 2025
 *      Author: Ocean
 */

#ifndef INC_RTC_H_
#define INC_RTC_H_

#include "stm32f4xx_hal.h"

/* Link to the handle defined in main.c */
extern I2C_HandleTypeDef hi2c2;

/* RTC Registers */
#define ADDR_SEC        0x00
#define ADDR_MIN        0x01
#define ADDR_HRS        0x02
#define ADDR_DAY        0x03
#define ADDR_DATE       0x04
#define ADDR_MONTH      0x05
#define ADDR_YEAR       0x06

/* Time Formats */
#define TIME_FORMAT_12HRS_AM  0
#define TIME_FORMAT_12HRS_PM  1
#define TIME_FORMAT_24HRS     2

/* I2C Address (Must be shifted left by 1 for STM32 HAL) */
#define DS1307_I2C_ADDR       (0x68 << 1)

/* Data Structures */
typedef struct {
    uint8_t date;
    uint8_t month;
    uint8_t year;
    uint8_t day;
} RTC_date_t;

typedef struct {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t time_format;
} RTC_time_t;

/* Function Prototypes */
uint8_t ds1307_init(void);
void ds1307_set_current_time(RTC_time_t *rtc_time);
void ds1307_get_current_time(RTC_time_t *rtc_time);
void ds1307_set_current_date(RTC_date_t *rtc_date);
void ds1307_get_current_date(RTC_date_t *rtc_date);

#endif /* INC_RTC_H_ */



