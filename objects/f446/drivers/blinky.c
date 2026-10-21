#include "blinky.h"

void LED_Toggle(void)
{
    GPIOA->ODR ^= (1<<5);
}

void LED_Off(void)
{
    GPIOA->BSRR |= ((1<<5)<<16);
}

void LED_On(void)
{
    GPIOA->BSRR |= (1<<5);
}