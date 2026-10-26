#include "blinky.h"

//SEMA
//QXSemaphore Morse_Sema;

//MUTEX
QXMutex Morse_mutex;

// Green
void Green_LED_Off(void)
{
    GPIOA->BSRR |= ((1<<5)<<16);
}

void Green_LED_On(void)
{
    GPIOA->BSRR |= (1<<5);
}

void Green_LED_Toggle(void)
{
	// This is to tell the system that here is a critical section of the code
	QF_CRIT_STAT
	QF_CRIT_ENTRY();
	
  GPIOA->ODR ^= (1<<5);
	
	QF_CRIT_EXIT();
}

// Yellow
void Yellow_LED_Off(void)
{
    GPIOA->BSRR |= ((1<<6)<<16);
}

void Yellow_LED_On(void)
{
    GPIOA->BSRR |= (1<<6);
}

void Yellow_LED_Toggle(void)
{
	// This is to tell the system that here is a critical section of the code
	QF_CRIT_STAT
	QF_CRIT_ENTRY();
	
  GPIOA->ODR ^= (1<<6);
	
	QF_CRIT_EXIT();
}

// Red
void Red_LED_Off(void)
{
    GPIOA->BSRR |= ((1<<7)<<16);
}

void Red_LED_On(void)
{
    GPIOA->BSRR |= (1<<7);
}

void Red_LED_Toggle(void)
{
    GPIOA->ODR ^= (1<<7);
}

void SendMorseCode(uint32_t bitmask) {
    uint32_t volatile delay_ctr;
    enum { DOT_DELAY = 550};

    //LOCK
    QSchedStatus sstat;

    //SEMA
//    QXSemaphore_wait(&Morse_Sema,  /* pointer to semaphore to wait on */
//                     QXTHREAD_NO_TIMEOUT); /* timeout for waiting */

    //LOCK
//    sstat = QXK_schedLock(5U); /* priority ceiling 5 */

    //MUTEX
    QXMutex_lock(&Morse_mutex,
                 QXTHREAD_NO_TIMEOUT); /* timeout for waiting */

    for (; bitmask != 0U; bitmask <<= 1) {
        if ((bitmask & (1U << 31)) != 0U) {
            Green_LED_On();
        }
        else {
            Green_LED_Off();
        }
        for (delay_ctr = DOT_DELAY;
             delay_ctr != 0U; --delay_ctr) {
        }
    }
    Green_LED_Off();
    for (delay_ctr = 7*DOT_DELAY;
         delay_ctr != 0U; --delay_ctr) {
    }

    //SEMA
//    QXSemaphore_signal(&Morse_Sema);  /* pointer to semaphore to signal */

    //LOCK
//    QXK_schedUnlock(sstat);

    //MUTEX
    QXMutex_unlock(&Morse_mutex);
}