
#include "stdint.h"
#include "stm32f446xx.h"

static volatile uint32_t tick_ms = 0;

void delay_init(void);
void delay(uint32_t t);
void SysTick_Handler(void);

void delay_init(void)
{
	SysTick_Config(180000);
}

void delay(uint32_t t)
{
	uint32_t start = tick_ms;
	while(tick_ms - start < t);
}

void SysTick_Handler(void)
{
	tick_ms++;
}

