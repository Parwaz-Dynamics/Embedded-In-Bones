#include "bsp.h"

void QF_onStartup(void)
{
	SystemClock_Config();
	SysTick_Config(180000);
	
	// Set SysTick interrupt priority to highest
	NVIC_SetPriority(SysTick_IRQn, QF_AWARE_ISR_CMSIS_PRI);
	NVIC_SetPriority(EXTI15_10_IRQn, QF_AWARE_ISR_CMSIS_PRI + 1);
	
	// Enable EXTI15_10 interrupt in NVIC
	NVIC_EnableIRQ(EXTI15_10_IRQn);		
}

void QF_onCleanup(void)
{
	
}

void QXK_onIdle(void) {

	
	GPIOA->BSRR |= (1 << 7);
	GPIOA->BSRR |= ((1 << 7) << 16);
//  __WFI(); /* stop the CPU and Wait for Interrupt */
}

Q_NORETURN Q_onError(char const * const module, int_t const id) {
    (void)module;
    (void)id;
    NVIC_SystemReset();
}