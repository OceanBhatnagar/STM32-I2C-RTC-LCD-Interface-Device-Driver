# I2C Communication with DS1307 RTC and I2C 16x2 LCD using STM32

This repository contains the source code and documentation for **I2C Communication with DS1307 RTC and I2C 16x2 LCD using STM32**,  
where an STM32F410RB microcontroller interfaces with a DS1307 Real-Time Clock (RTC) and an I2C-based 16x2 LCD module.  
The project demonstrates I2C communication using STM32 HAL, along with custom Board Support Package (BSP) drivers for the RTC and LCD peripherals.

## Features  
- **DS1307 RTC Interface**: Communicates with the DS1307 module over I2C to read and display real-time clock data.  
- **I2C 16x2 LCD Display**: Time and date information from the RTC is displayed on the I2C-enabled LCD screen.  
- **Custom BSP Drivers**: Developed BSP files for both RTC and LCD modules to abstract low-level HAL I2C interactions.  
- **Dual I2C Usage**: Configured **I2C1** and **I2C2** interfaces using STM32 Pinout Configuration for peripheral separation and clarity.  
- **Modular Code Structure**: HAL-generated base code with user-defined application logic layered on top.  

## Components Used  
- **STM32 Nucleo Board** (STM32F410RB)  
- **DS1307 RTC Module**  
- **I2C 16x2 LCD Module** (with PCF8574 I2C backpack) 
- **Breadboard** and **Jumper Wires**  

## Tools Used  
- **STM32CubeIDE**: For peripheral configuration, code generation, and firmware development  
- **STM32CubeMX**: Used for pin mapping and enabling I2C peripherals  

## Project Structure  
- **CODE/BSP**: BSP driver for DS1307 RTC communication via I2C  and for interfacing with the 16x2 I2C LCD  
- **CODE/Src & CODE/Inc**: Application layer that reads time from RTC and updates LCD display  
- **Project Photograph**:Demonstrates hardware setup, wiring, and actual test scenario 




