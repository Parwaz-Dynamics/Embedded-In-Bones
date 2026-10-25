#include "bsp.h"

void QF_onStartup(void)
{
	SystemClock_Config();
	SysTick_Config(180000);
	
	// Set SysTick interrupt priority to highest
	NVIC_SetPriority(SysTick_IRQn, 0U);
}

void QF_onCleanup(void)
{
	
}

void QXK_onIdle(void) {
#ifdef NDBEBUG
    __WFI(); /* stop the CPU and Wait for Interrupt */
#endif
}