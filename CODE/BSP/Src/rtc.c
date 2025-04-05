//FUNCTION PROTOTYPES
#include<stm32f4xx_hal.h>
#include<rtc.h>
#include<stdlib.h>
#include<string.h>

static void i2c_pin_config(void);
static void i2c_config(void);
static uint8_t ds1307_read(uint8_t reg_addr);
static void ds1307_write(uint8_t value,uint8_t reg_addr);
static uint8_t bcd_binary(uint8_t value);
static uint8_t binary_bcd(uint8_t value);

I2C_HandleTypeDef I2CPIN;

uint8_t ds1307_init(void){
	//INITIALIZE I2C PINS
	i2c_pin_config();
	//INITIALIZE I2C PERIPHERAL
	i2c_config();
	//ENABLE I2C PERIPHERAL
	__HAL_RCC_I2C2_CLK_ENABLE();

	//MAKE CLOCK HALT=0
	ds1307_write(0x00,ADDR_SEC);

	//READ
	uint8_t clock_state=ds1307_read(ADDR_SEC);

	return ((clock_state >> 7) & 0x1);
}

void ds1307_set_current_time(RTC_time_t *rtc_time){
	uint8_t seconds,hrs;
	seconds=binary_bcd(rtc_time->seconds);
	seconds &=~(1<<7);
	ds1307_write(seconds,ADDR_SEC);

	ds1307_write(binary_bcd(rtc_time->minutes),ADDR_MIN);

	hrs=binary_bcd(rtc_time->hours);
	if(rtc_time->time_format==TIME_FORMAT_24HRS){
		hrs&=~(1<<6);
	}
	else{
		hrs |=(1<<6);
		hrs=(rtc_time->time_format==TIME_FORMAT_12HRS_PM) ? hrs |(1<<5):hrs & ~(1<<5);
	}
	ds1307_write(hrs,ADDR_HRS);

}
void ds1307_get_current_time(RTC_time_t *rtc_time){
	uint8_t seconds,hrs;

    seconds=ds1307_read(ADDR_SEC);
    seconds &=~(1<<7);
    rtc_time->seconds=bcd_binary(seconds);
    rtc_time->minutes=bcd_binary(ds1307_read(ADDR_MIN));

    hrs=ds1307_read(ADDR_HRS);
    if(hrs & (1<<6)){
    	//12HR FORMAT
    	//CORRECTED
    	rtc_time->time_format =  !((hrs & ( 1 << 5)) == 0) ;
    	hrs &=~(1<<5);
    	hrs&=~(1<<6);
    }else{
    	//24 FORMAT
    	rtc_time->time_format=TIME_FORMAT_24HRS;
    }
    rtc_time->hours=bcd_binary(hrs);

}

void ds1307_set_current_date(RTC_date_t *date){
	ds1307_write(binary_bcd(date->day),ADDR_DAY);
	ds1307_write(binary_bcd(date->date),ADDR_DATE);
    ds1307_write(binary_bcd(date->month),ADDR_MONTH);
    ds1307_write(binary_bcd(date->year),ADDR_YEAR);

}
void ds1307_get_current_date(RTC_date_t *rtc_date){
	rtc_date->date=bcd_binary(ds1307_read(ADDR_DATE));
	rtc_date->day=bcd_binary(ds1307_read(ADDR_DAY));
	rtc_date->month=bcd_binary(ds1307_read(ADDR_MONTH));
	rtc_date->year=bcd_binary(ds1307_read(ADDR_YEAR));
}

static void i2c_pin_config(void){
	GPIO_InitTypeDef i2c_sda = {0}, i2c_scl = {0};

	__HAL_RCC_GPIOB_CLK_ENABLE();

	//SDA 9  SCL 10

	i2c_sda.Alternate=GPIO_AF4_I2C2; ;
	i2c_sda.Mode=GPIO_MODE_AF_PP;
	i2c_sda.Pin=DS1307_I2C_SDA_PIN;
	i2c_sda.Pull=DS1307_I2C_PUPD;
	i2c_sda.Speed=DS1307_I2C_SPEED;
	HAL_GPIO_Init(DS1307_I2C_GPIO_PORT, &i2c_sda);

	i2c_scl.Alternate=GPIO_AF4_I2C2;
	i2c_scl.Mode=GPIO_MODE_AF_PP;
	i2c_scl.Pin=DS1307_I2C_SCL_PIN;
	i2c_scl.Pull=DS1307_I2C_PUPD;
	i2c_scl.Speed=DS1307_I2C_SPEED;

	// Initialize GPIO pins
    HAL_GPIO_Init(DS1307_I2C_GPIO_PORT, &i2c_scl);
}


static void i2c_config(void){
    __HAL_RCC_I2C2_CLK_ENABLE(); // Move here
    I2CPIN.Instance = DS1307_I2C;
    I2CPIN.Init.ClockSpeed = DS1307_I2C_SPEED;
    I2CPIN.Init.DutyCycle = I2C_DUTYCYCLE_2;
    I2CPIN.Init.OwnAddress1 = 0;
    I2CPIN.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    I2CPIN.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    I2CPIN.Init.OwnAddress2 = 0;
    I2CPIN.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    I2CPIN.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    HAL_I2C_Init(&I2CPIN);
}
static void ds1307_write(uint8_t value,uint8_t reg_addr){
	uint8_t tx[2];
	tx[0]=reg_addr;
	tx[1]=value;
	HAL_I2C_Master_Transmit(&I2CPIN,DS1307_I2C_ADDR,tx,2,HAL_MAX_DELAY);
}

static uint8_t ds1307_read(uint8_t reg_addr){
	uint8_t data=0;
	HAL_I2C_Master_Transmit(&I2CPIN, DS1307_I2C_ADDR, &reg_addr, 1,HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(&I2CPIN, DS1307_I2C_ADDR, &data, 1, HAL_MAX_DELAY);
	return data;
}

static uint8_t binary_bcd(uint8_t value){
	uint8_t m,n;
	uint8_t bcd;

	bcd=value;
	if(value>=10){
		m=value/10;
		n=value%10;
		bcd=(uint8_t)((m<<4)|n);
	}
	return bcd;
}

static uint8_t bcd_binary(uint8_t value){
	uint8_t m,n;

	m=(uint8_t)((value>>4)*10);
	n=value & (uint8_t)0x0F;
	return (m+n);
}
