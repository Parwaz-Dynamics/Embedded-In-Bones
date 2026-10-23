// bsp.h
#ifndef BSP_H
#define BSP_H
#include "stm32f446xx.h"


// System Clock
void SystemClock_Config(void);

// Sys Tick
uint32_t get_tick(void);
void delay(uint32_t ms);

// GPIO
void GPIO_Init(void);

// USART

// SPI

// I2C

#endif
