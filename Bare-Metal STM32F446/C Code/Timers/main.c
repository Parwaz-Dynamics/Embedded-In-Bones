#include "SystemClock_Config.h"

static volatile uint32_t tickms = 0;
void SysTick_Handler(void);
void delay(uint32_t time);

void GPIO_Init(void);

int main(void)
{
	SystemClock_Config();
	SysTick_Config(180000);
	while(1)
	{
		delay(1000);
	}
}

void SysTick_Handler(void)
{
	tickms++;
}

void delay(uint32_t time)
{
	uint32_t start_time = tickms;
	while(tickms - start_time < time);
}

void GPIO_Init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
}
