
#include "stm32f446xx.h"
#include "SystemClock_Config.h"

extern volatile uint32_t tickms;
volatile uint32_t tickms = 0;

void SysTick_Handler(void);
void delay(uint32_t ms);

void GPIOA_Init(void);

int main(void)
{
	SystemClock_Config();
	SysTick_Config(180000);
	GPIOA_Init();
	
	while(1)
	{
		delay(500);
		GPIOA->ODR ^= (1<<5);
	}
}

// System Tick Timer for delay
void SysTick_Handler(void)
{
	tickms++;
}

void delay(uint32_t ms)
{
	uint32_t start = tickms;
	
	while(tickms - start < ms){}
}

// GPIO Settings
void GPIOA_Init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	
	GPIOA->MODER &= ~(0x3U << 10);
	GPIOA->MODER |= (0x1U << 10);
	
}
