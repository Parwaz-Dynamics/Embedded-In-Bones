// bsp.h
#ifndef BSP_H
#define BSP_H
#include "stm32f446xx.h"


// System Clock
void SystemClock_Config(void);

// Sys Tick

// GPIO
void GPIO_Init(void);

// USART

// SPI

// I2C

// OS
void QF_onCleanup(void);

void QF_onStartup(void);

void QXK_onIdle(void);

#endif
