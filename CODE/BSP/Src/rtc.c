#include "rtc.h"
#include <stdlib.h>
#include <string.h>

/* Private Helper Functions */
static uint8_t ds1307_read(uint8_t reg_addr);
static void ds1307_write(uint8_t value, uint8_t reg_addr);
static uint8_t bcd_binary(uint8_t value);
static uint8_t binary_bcd(uint8_t value);

/**
  * @brief  Initializes the DS1307.
  * Note: Hardware init (I2C/GPIO) is done in main.c/hal_msp.c
  * @retval 0 if CH bit is 0 (Running), 1 if CH bit was 1 (Halted)
  */
uint8_t ds1307_init(void) {
    // 1. Configure the Main Control Register if needed (Enable Square Wave, etc.)
    // For basic timekeeping, we ensure the Clock Halt (CH) bit is 0.

    // Write 0x00 to Register 0x00 to clear the CH bit (Bit 7) and start the oscillator
    // This preserves the seconds count if we mask it, but here we just start it.

    // Check if device is ready
    if(HAL_I2C_IsDeviceReady(&hi2c2, DS1307_I2C_ADDR, 2, 100) != HAL_OK) {
        return 1; // Device not found
    }

    uint8_t clock_state = ds1307_read(ADDR_SEC);

    // If Clock Halt bit (Bit 7) is 1, the clock is stopped.
    if ((clock_state >> 7) & 1) {
        // Clock is halted, start it by writing 0 to bit 7
        ds1307_write(clock_state & ~(1 << 7), ADDR_SEC);
        return 1; // Was halted
    }

    return 0; // Running
}

void ds1307_set_current_time(RTC_time_t *rtc_time) {
    uint8_t seconds, hrs;

    seconds = binary_bcd(rtc_time->seconds);
    // Ensure CH bit (Bit 7) is 0 so clock keeps running
    seconds &= ~(1 << 7);
    ds1307_write(seconds, ADDR_SEC);

    ds1307_write(binary_bcd(rtc_time->minutes), ADDR_MIN);

    hrs = binary_bcd(rtc_time->hours);
    if (rtc_time->time_format == TIME_FORMAT_24HRS) {
        hrs &= ~(1 << 6);
    } else {
        hrs |= (1 << 6);
        hrs = (rtc_time->time_format == TIME_FORMAT_12HRS_PM) ? hrs | (1 << 5) : hrs & ~(1 << 5);
    }
    ds1307_write(hrs, ADDR_HRS);
}

void ds1307_get_current_time(RTC_time_t *rtc_time) {
    uint8_t seconds, hrs;

    seconds = ds1307_read(ADDR_SEC);
    seconds &= ~(1 << 7); // Mask out CH bit
    rtc_time->seconds = bcd_binary(seconds);

    rtc_time->minutes = bcd_binary(ds1307_read(ADDR_MIN));

    hrs = ds1307_read(ADDR_HRS);
    if (hrs & (1 << 6)) {
        // 12HR FORMAT
        rtc_time->time_format = !((hrs & (1 << 5)) == 0);
        hrs &= ~(1 << 5); // Clear AM/PM bit
        hrs &= ~(1 << 6); // Clear 12/24 bit
    } else {
        // 24HR FORMAT
        rtc_time->time_format = TIME_FORMAT_24HRS;
    }
    rtc_time->hours = bcd_binary(hrs);
}

void ds1307_set_current_date(RTC_date_t *date) {
    ds1307_write(binary_bcd(date->day), ADDR_DAY);
    ds1307_write(binary_bcd(date->date), ADDR_DATE);
    ds1307_write(binary_bcd(date->month), ADDR_MONTH);
    ds1307_write(binary_bcd(date->year), ADDR_YEAR);
}

void ds1307_get_current_date(RTC_date_t *rtc_date) {
    rtc_date->date = bcd_binary(ds1307_read(ADDR_DATE));
    rtc_date->day = bcd_binary(ds1307_read(ADDR_DAY));
    rtc_date->month = bcd_binary(ds1307_read(ADDR_MONTH));
    rtc_date->year = bcd_binary(ds1307_read(ADDR_YEAR));
}

/* ================================================================= */
/* LOW LEVEL FUNCTIONS ADAPTED FOR MAIN.C HI2C2 HANDLE              */
/* ================================================================= */

static void ds1307_write(uint8_t value, uint8_t reg_addr) {
    uint8_t tx[2];
    tx[0] = reg_addr;
    tx[1] = value;
    // Use hi2c2 directly. Timeout set to 100ms instead of Infinite
    HAL_I2C_Master_Transmit(&hi2c2, DS1307_I2C_ADDR, tx, 2, 100);
}

static uint8_t ds1307_read(uint8_t reg_addr) {
    uint8_t data = 0;

    // 1. Send Register Address
    if (HAL_I2C_Master_Transmit(&hi2c2, DS1307_I2C_ADDR, &reg_addr, 1, 100) != HAL_OK) {
        // If this fails, the RTC is NOT responding (wiring or pull-up issue)
        return 0xFF; // Return 255 to indicate Error (instead of 0)
    }

    // 2. Read Data
    if (HAL_I2C_Master_Receive(&hi2c2, DS1307_I2C_ADDR, &data, 1, 100) != HAL_OK) {
        return 0xFF; // Return 255 to indicate Error
    }

    return data;
}

static uint8_t binary_bcd(uint8_t value) {
    uint8_t m, n;
    uint8_t bcd;

    bcd = value;
    if (value >= 10) {
        m = value / 10;
        n = value % 10;
        bcd = (uint8_t)((m << 4) | n);
    }
    return bcd;
}

static uint8_t bcd_binary(uint8_t value) {
    uint8_t m, n;
    m = (uint8_t)((value >> 4) * 10);
    n = value & (uint8_t)0x0F;
    return (m + n);
}

