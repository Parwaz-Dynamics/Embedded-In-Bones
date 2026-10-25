// miros.h
#ifndef MIROS_H
#define MIROS_H

#include "stdint.h"
#include "stm32f446xx.h"


// Thread Control Block
typedef struct{
	void *sp;		// Stack Pointer
	uint32_t timeout; // Timeout delay count-down
	uint8_t prio;
	// other attributes associated with the thread
}OSThread;

void OS_Init(void *stkSto, uint32_t stkSize);
void OS_Sched(void);

void OS_Run(void);
void OS_onStartup(void);

void OS_onIdle(void);

void OS_Delay(uint32_t ticks);
void OS_Tick(void);

typedef void (*OSThreadHandler) (void);

void OSThread_Start(
	OSThread *me,
	uint8_t prio,
	OSThreadHandler threadHandler,
	void *stkSto, uint32_t stkSize);

#endif
