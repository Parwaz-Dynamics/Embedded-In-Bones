
#include "bsp.h"

//extern QXSemaphore Morse_Sema;

extern QXMutex Morse_mutex;

void GPIO_Init(void)
{
	
	/* Enable GPIO clock */
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

	// LED A5 -> Green
		GPIOA->MODER &= ~((1<<10) | (1<<11));
		GPIOA->MODER |= (1<<10) | (0<<11);
	
	// LED A6 -> Yellow
		GPIOA->MODER &= ~((1<<12) | (1<<13));
		GPIOA->MODER |= (1<<12) | (0<<13);
	
	// LED A7 -> Red
		GPIOA->MODER &= ~((1<<14) | (1<<15));
		GPIOA->MODER |= (1<<14) | (0<<15);
	
	// LED A7 -> Red
		GPIOA->MODER &= ~((1<<16) | (1<<17));
		GPIOA->MODER |= (1<<16) | (0<<17);
	
  /* Enable SYSCFG clock */
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	/* Connect EXTI13 to GPIOC */
	SYSCFG->EXTICR[3] &= ~(0xFU << 4);
	SYSCFG->EXTICR[3] |=  (0x2U << 4);

	/* Unmask EXTI line 13 */
	EXTI->IMR |= (1U << 13);

	/* Disable rising-edge trigger */
	EXTI->RTSR &= ~(1U << 13);

	/* Enable falling-edge trigger */
	EXTI->FTSR |= (1U << 13);

	/* Clear any pending interrupt */
	EXTI->PR = (1U << 13);
	
	
//	/* initialize the SW1_sema semaphore as binary, signaling semaphore */
//    QXSemaphore_init(&Morse_Sema, /* pointer to semaphore to initialize */
//                     1U,  /* initial semaphore count (singaling semaphore) */
//                     1U); /* maximum semaphore count (binary semaphore) */

			QXMutex_init(&Morse_mutex, 6U); /* priority ceiling 6 */
}

void EXTI15_10_IRQHandler(void)
{
	QXK_ISR_ENTRY();
	
	if(EXTI->PR & (1U << 13))
	{
		QXSemaphore_signal(&SW1_sema);
	}
	EXTI->PR = (1U << 13);  /* Clear pending flag */
	
	QXK_ISR_EXIT();
}
