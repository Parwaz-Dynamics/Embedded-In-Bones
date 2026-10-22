#include "main.h"

/* Register addresses (STM32F446) */
#define RCC_AHB1ENR   (*(volatile uint32_t *)0x40023830)
#define GPIOA_MODER   (*(volatile uint32_t *)0x40020000)
#define GPIOA_ODR     (*(volatile uint32_t *)0x40020014)

/* ST's startup calls these before main; empty stubs are enough for now. */
void SystemInit(void) {}
void _init(void)      {}
void _fini(void)      {}

void delay(volatile uint32_t count)
{
    while (count--)
    {
    }
}

int main(void)
{
    RCC_AHB1ENR |= (1 << 0); /* Enable GPIOA clock */
    GPIOA_MODER |= (1 << 10); /* Set PA5 as output */
    while (1)
    {
        /* nothing yet */
        delay(1000000);
        GPIOA_ODR ^= (1 << 5);   /* Toggle PA5 */
    }
}