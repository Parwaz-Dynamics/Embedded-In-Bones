#include "bsp.h"
#include "miros.h"

static volatile uint32_t tickms = 0;

void SysTick_Handler(void)
{
	OS_Tick();
	
	__disable_irq();
	OS_Sched();
	__enable_irq();
}