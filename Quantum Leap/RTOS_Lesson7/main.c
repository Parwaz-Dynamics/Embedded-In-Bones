
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
		SendMorseCode(0xA8EEE2A0);
		
//		uint32_t volatile i;
//		
//		for(i = 6840 + 1; i > 0; --i)
//		{
////			Green_LED_On();
////			Green_LED_Off();
//			Green_LED_Toggle();
//		}
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
		
		for(i = 6840 * 3; i > 0; --i)
		{
//			Yellow_LED_On();
//			Yellow_LED_Off();
			Yellow_LED_Toggle();
		}
	}
}

uint32_t stack_blinky3[40];
QXThread blinky3;

void main_blinky3(QXThread * const me)
{		
	while(1)
	{		
		SendMorseCode(0xE22A3800U); /* "TEST" */
    SendMorseCode(0xE22A3800U); /* "TEST" */
		
		QXThread_delay(5U);
	}
}

int main(void)
{	
//	GPIO_Init();
//	QF_init();
	
	QF_init();
	GPIO_Init();
	
	/* initialize the SW1_sema semaphore as binary, signaling semaphore */
    QXSemaphore_init(&SW1_sema, /* pointer to semaphore to initialize */
                     0U,  /* initial semaphore count (singaling semaphore) */
                     1U); /* maximum semaphore count (binary semaphore) */
	
	// start blinky thread 1
	
	QXThread_ctor(&blinky1, &main_blinky1, 0);
	QXTHREAD_START(&blinky1,
									5U,
								 (void *)0, 0,
								stack_blinky1, sizeof(stack_blinky1),
									0);
	
	QXThread_ctor(&blinky2, &main_blinky2, 0);
	// start blinky thread 2
	QXTHREAD_START(&blinky2,
									2U,
								 (void *)0, 0,
								stack_blinky2, sizeof(stack_blinky2),
									0);
	
	QXThread_ctor(&blinky3, &main_blinky3, 0);
	// start blinky thread 3
	QXTHREAD_START(&blinky3,
									1U,
								 (void *)0, 0,
								stack_blinky3, sizeof(stack_blinky3),
									0);
	
	// transfer control to RTOS to run threads
	QF_run();
	
	return 0;
}

