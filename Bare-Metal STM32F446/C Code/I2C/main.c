#include "stm32f446xx.h"
#include "SystemClock_Config.h"

extern volatile uint32_t tickms;
volatile uint32_t tickms = 0;

// System Tick

void SysTick_Handler(void)
{
	tickms++;
}

void delay(uint32_t ms)
{
	uint32_t start = tickms;
	
	while(tickms - start < ms);
}

// GPIO
static void GPIOA_Init()
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	
	GPIOA->MODER &= ~(0x3U << 10);
	GPIOA->MODER |= (0x2U << 10);	
}

int main(void)
{
	SystemClock_Config();
	SysTick_Config(180000);
	GPIOA_Init();
	
	while(1)
	{
		GPIOA->ODR ^= (0x1U << 5);
		
		delay(1000);
	}
}
