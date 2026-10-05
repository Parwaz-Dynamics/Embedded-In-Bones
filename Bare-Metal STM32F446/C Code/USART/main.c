
#include "stm32f446xx.h"
#include "SystemClock_Config.h"

extern volatile uint32_t tickms;
volatile uint32_t tickms = 0;

void delay(uint32_t time)
{
	uint32_t current = tickms;
	while(tickms - current < time);
}

void GPIOA_Init()
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	
	GPIOA->MODER |= (1 << 10) | (0 << 11);
}

int main(void)
{
	SystemClock_Config();
	SysTick_Config(180000);
	GPIOA_Init();
	
	while(1)
	{
		delay(1000);
		GPIOA->ODR ^= 1<<5;
	}
	
}
