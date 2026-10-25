#include "miros.h"
#include "blinky.h"

// We have not added Assertions right now

#define LOG2(x) (32 - __CLZ(x))

OSThread * volatile OS_Curr; // Pointer to the current thread
OSThread * volatile OS_Next; // Pointer to the next thread/

OSThread *OS_Thread[32 + 1]; // Array of threads started
uint32_t OS_readySet;	// to determine the ready or block state of a task
uint32_t OS_delayedSet;	// 


// Idle Stack
OSThread idleThread;

void main_idleThread(void){		
	while(1)
	{
		OS_onIdle();
	}
}

// Idle task that will run while no task is ready
void OS_onIdle(void)
{
	
	Red_LED_On();
	Red_LED_Off();
	
	// Wait fo interrupt
//	__WFI();
}

// OS delay puts the thread from running state to blocked state
void OS_Delay(uint32_t ticks)
{
	__disable_irq();
	
	
	// never call OS_Delay is current thread is thread 0 -> Idle Thread
	if(OS_Curr == OS_Thread[0])
		NVIC_SystemReset();
	
	// defining the required delay
	OS_Curr->timeout = ticks;
	
	// remove the thread from ready state
	OS_readySet &= ~(1U << (OS_Curr->prio - 1U));
	OS_delayedSet |= (1U << (OS_Curr->prio - 1U));
	
	// call the scheduler
	OS_Sched();
	
	__enable_irq();
}

// calculates tick and see if any thread needs to be pushed into preempted stage
void OS_Tick(void)
{
	uint32_t workingSet = OS_delayedSet;
	while(workingSet != 0U)
	{
		OSThread *t = OS_Thread[LOG2(workingSet)];
		// an assertion should be added here to proctect 
		// from entering thread 0 and timeout
		
		--t->timeout;
		if(t->timeout == 0U)
		{
			OS_readySet |= (1U << (t->prio - 1U));
			OS_delayedSet &= ~(1U << (t->prio - 1U));
		}
		
		workingSet &= ~(1U << (t->prio - 1U));
		
	}
}


// RTOS initialization
void OS_Init(void *stkSto, uint32_t stkSize)
{
	// Set PendSV interrupt priority to lowest
	*(uint32_t volatile *)0xE000ED20 |= (0xFFU << 16);
	
	// start blinky thread 1
	OSThread_Start(&idleThread,
									0U,
								 &main_idleThread,
								 stkSto, stkSize);
}	

// RTOS Scheduler
void OS_Sched(void)
{
	// OS Thread Scheduling
	
	// if neither task is ready, enter idle state
	if(OS_readySet == 0x0U)
	{
		OS_Next = OS_Thread[0U];	
	}
	else
	{
		OS_Next = OS_Thread[LOG2(OS_readySet)];
		if(OS_Next == (OSThread*) 0)
			NVIC_SystemReset();
	}
	
	if(OS_Next != OS_Curr)
	{
		// Generate PendSV interrupt
		*(uint32_t volatile *) 0xE000ED04 |= (0x1U << 28);
	}
}

// Enabling real time operating system
void OS_Run(void)
{
	// callback to configure and start interrupt 
	OS_onStartup();
	
	__disable_irq();
	OS_Sched();
	__enable_irq();
	
	// this section should never execute
	NVIC_SystemReset();
}

// Initialize thread
void OSThread_Start(
	OSThread *me,
	uint8_t prio,
	OSThreadHandler threadHandler,
	void *stkSto, uint32_t stkSize)
{
	uint32_t *sp = (uint32_t *)((((uint32_t) stkSto + stkSize) / 8) * 8);
	uint32_t *stk_limit;
	
	*(--sp) = (1<<24);		// xPSR
	*(--sp) = (uint32_t) threadHandler;		// PC
	*(--sp) = 0x0000000EU;		// LR
	*(--sp) = 0x0000000CU;		// R12
	*(--sp) = 0x00000003U;		// R3
	*(--sp) = 0x00000002U;		// R2
	*(--sp) = 0x00000001U;		// R1
	*(--sp) = 0x00000000U;		// R0
	*(--sp) = 0x0000000BU;		// R11
	*(--sp) = 0x0000000AU;		// R10
	*(--sp) = 0x00000009U;		// R9
	*(--sp) = 0x00000008U;		// R8
	*(--sp) = 0x00000007U;		// R7
	*(--sp) = 0x00000006U;		// R6
	*(--sp) = 0x00000005U;		// R5
	*(--sp) = 0x00000004U;		// R4

	
	// Save the top of the stack in the thread attribute
	me->sp = sp;
	
	// Set the stack limit
	stk_limit = (uint32_t *)((((uint32_t) stkSto - 1) / 8 + 1) * 8);
	
	// fill the remaining entries of stacks with 0xDEADBEEF
	for(sp = sp -1; sp >= stk_limit; --sp)
	{
		*sp = 0xDEADBEEFU;
	}
	
	// tell the thread array that new task is loaded 
	OS_Thread[prio] = me;
	me->prio = prio;
	
	// make all threads except idle thread ready to run
	if(prio > 0U)
	{
		OS_readySet |= (1U << (prio - 1U));
	}
}

// Context switching from interrupt handler for us
__attribute__((naked))
void PendSV_Handler(void)
{
    __asm volatile (
        "  CPSID    I                  \n"
        "  LDR      r1,=OS_Curr        \n"
        "  LDR      r1,[r1,#0]         \n"
        "  CBZ      r1,PendSV_restore  \n"
        "  PUSH     {r4-r11}           \n"
        "  LDR      r1,=OS_Curr        \n"
        "  LDR      r1,[r1,#0]         \n"
        "  STR      sp,[r1,#0]         \n"
        "PendSV_restore:               \n"
        "  LDR      r1,=OS_Next        \n"
        "  LDR      r1,[r1,#0]         \n"
        "  LDR      r2,=OS_Curr        \n"
        "  STR      r1,[r2,#0]         \n"
        "  LDR      sp,[r1,#0]         \n"
        "  POP      {r4-r11}           \n"
        "  CPSIE    I                  \n"
        "  BX       lr                 \n"
    );
}