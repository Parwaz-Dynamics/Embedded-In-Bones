#include "bsp.h"
#include "miros.h"

static volatile uint32_t tickms = 0;

void SysTick_Handler(void)
{
	tickms++;
	
	__disable_irq();
	OS_Sched();
	__enable_irq();
}

uint32_t get_tick(void)
{
	uint32_t tick2out;
	
	__disable_irq();
	tick2out = tickms;
	__enable_irq();
	
	return tick2out;
	
}

void delay(uint32_t ms)
{
	
	uint32_t start = get_tick();
	
    while(get_tick() - start < ms);
}