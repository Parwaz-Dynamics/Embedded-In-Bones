// miros.h
#ifndef MIROS_H
#define MIROS_H

#include "stdint.h"
#include "stm32f446xx.h"


// Thread Control Block
typedef struct{
	void *sp;		// Stack Pointer
	// other attributes associated with the thread
}OSThread;

void OS_Init(void);
void OS_Sched(void);

void OS_Run(void);
void OS_onStartup(void);

typedef void (*OSThreadHandler) (void);

void OSThread_Start(
	OSThread *me,
	OSThreadHandler threadHandler,
	void *stkSto, uint32_t stkSize);

#endif
