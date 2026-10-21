
#include "bsp.h"

void GPIO_Init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER &= ~((1<<10) | (1<<11));
	GPIOA->MODER |= (1<<10) | (0<<11);
}
