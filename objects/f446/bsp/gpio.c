
#include "bsp.h"

void GPIO_Init(void)
{
	// LED A5 -> Green
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
		GPIOA->MODER &= ~((1<<10) | (1<<11));
		GPIOA->MODER |= (1<<10) | (0<<11);
	
	// LED A6 -> Yellow
		GPIOA->MODER &= ~((1<<12) | (1<<13));
		GPIOA->MODER |= (1<<12) | (0<<13);
	
	// LED A7 -> Red
		GPIOA->MODER &= ~((1<<14) | (1<<15));
		GPIOA->MODER |= (1<<14) | (0<<15);
	
	
}
