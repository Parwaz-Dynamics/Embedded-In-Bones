#include "miros.h"
#include "bsp.h"

void OS_onStartup(void)
{
	SystemClock_Config();
	SysTick_Config(180000);
	
	// Set SysTick interrupt priority to highest
	NVIC_SetPriority(SysTick_IRQn, 0U);
}