// blinky.h
#ifndef BLINKY_H
#define BLINKY_H
#include "stm32f446xx.h"
#include "qpc.h"

void Green_LED_Off(void);   // prototype only
void Green_LED_On(void);   // prototype only
void Green_LED_Toggle(void);   // prototype only

void Yellow_LED_Off(void);   // prototype only
void Yellow_LED_On(void);   // prototype only
void Yellow_LED_Toggle(void);   // prototype only

void Red_LED_Off(void);   // prototype only
void Red_LED_On(void);   // prototype only
void Red_LED_Toggle(void);   // prototype only

void SendMorseCode(uint32_t bitmask);

#endif
