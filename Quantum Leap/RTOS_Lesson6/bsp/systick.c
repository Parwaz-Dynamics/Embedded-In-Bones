#include "bsp.h"
#include "qpc.h"

static volatile uint32_t tickms = 0;

void SysTick_Handler(void)
{
	GPIOA->BSRR |= ((1 << 8));
	QXK_ISR_ENTRY();
	
	QF_TICK_X(0, (void *)0);
	
	QXK_ISR_EXIT();
	
	GPIOA->BSRR |= ((1 << 8) << 16);
}