#include "blinky.h"

void Green_LED_Off(void)
{
    GPIOA->BSRR |= ((1<<5)<<16);
}

void Green_LED_On(void)
{
    GPIOA->BSRR |= (1<<5);
}

void Yellow_LED_Off(void)
{
    GPIOA->BSRR |= ((1<<6)<<16);
}

void Yellow_LED_On(void)
{
    GPIOA->BSRR |= (1<<6);
}

void Red_LED_Off(void)
{
    GPIOA->BSRR |= ((1<<7)<<16);
}

void Red_LED_On(void)
{
    GPIOA->BSRR |= (1<<7);
}