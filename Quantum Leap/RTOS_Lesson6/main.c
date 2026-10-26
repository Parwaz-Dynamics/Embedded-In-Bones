
#include "stm32f446xx.h"
#include "stdint.h"

#include "bsp.h"

#include "blinky.h"

#include "qpc.h"

QXSemaphore SW1_sema;

uint32_t stack_blinky1[40];
QXThread blinky1;

void main_blinky1(QXThread * const me){		
	while(1)
	{
		uint32_t volatile i;
		
		for(i = 5400; i > 0; --i)
		{
			Green_LED_On();
			Green_LED_Off();
		}
		QXThread_delay(1U);
	}
}

uint32_t stack_blinky2[40];
QXThread blinky2;

void main_blinky2(QXThread * const me)
{		
	while(1)
	{
		uint32_t volatile i;
		
		QXSemaphore_wait(&SW1_sema,  /* pointer to semaphore to wait on */
                         QXTHREAD_NO_TIMEOUT); /* timeout for waiting */
		
		for(i = 5400 * 3; i > 0; --i)
		{
			Yellow_LED_On();
			Yellow_LED_Off();
		}
	}
}

uint32_t stack_blinky3[40];
QXThread blinky3;

void main_blinky3(void)
{		
	while(1)
	{		
		Red_LED_On();
		QXThread_delay(1000 / 3);
		Red_LED_Off();
		QXThread_delay(1000 * 3 / 5);
	}
}

int main(void)
{	
	GPIO_Init();
	QF_init();
	
	/* initialize the SW1_sema semaphore as binary, signaling semaphore */
    QXSemaphore_init(&SW1_sema, /* pointer to semaphore to initialize */
                     0U,  /* initial semaphore count (singaling semaphore) */
                     1U); /* maximum semaphore count (binary semaphore) */
	
	// start blinky thread 1
	
	QXThread_ctor(&blinky1, &main_blinky1, 0);
	QXThread_ctor(&blinky2, &main_blinky2, 0);
	
	QXTHREAD_START(&blinky1,
									5U,
								 (void *)0, 0,
								stack_blinky1, sizeof(stack_blinky1),
									0);
	
	// start blinky thread 2
	QXTHREAD_START(&blinky2,
									2U,
								 (void *)0, 0,
								stack_blinky2, sizeof(stack_blinky2),
									0);
	
//	// start blinky thread 3
//	QXThread_Start(&blinky3,
//								 &main_blinky3,
//								stack_blinky3, sizeof(stack_blinky3));
	
	// transfer control to RTOS to run threads
	QF_run();
	
	return 0;
}

