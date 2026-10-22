#include "miros.h"

OSThread * volatile OS_Curr; // Pointer to the current thread
OSThread * volatile OS_Next; // Pointer to the next thread

void OS_Init(void)
{
	// Set PendSV interrupt priority to lowest
	*(uint32_t volatile *)0xE000ED20 |= (0xFFU << 16);
}	

void OS_Sched(void)
{
	// OS Thread Scheduling
	
	if(OS_Next != OS_Curr)
	{
		// Generate PendSV interrupt
		*(uint32_t volatile *) 0xE000ED04 |= (0x1U << 28);
	}
}

void OSThread_Start(
	OSThread *me,
	OSThreadHandler threadHandler,
	void *stkSto, uint32_t stkSize)
{
	uint32_t *sp = (uint32_t *)((((uint32_t) stkSto + stkSize) / 8) * 8);
	uint32_t *stk_limit;
	
	*(--sp) = (1<<24);		// xPSR
	*(--sp) = (uint32_t) &threadHandler;		// PC
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
	
	stk_limit = (uint32_t *)((((uint32_t) stkSto - 1) / 8 + 1) * 8);
	
	for(sp = sp -1; sp >= stk_limit; --sp)
	{
		*sp = 0xDEADBEEFU;
	}
}

__asm
void PendSV_Handler(void)
{	
	IMPORT OS_Curr
	IMPORT OS_Next
	//  0x08000516
	
//    __disable_irq(); 
			CPSID      
	I
//		if(OS_Curr != (OSThread *)0) 
			LDR 					r0, =OS_Curr
			LDR           r0,[r0,#0x00]
			CBZ           r0,PendSV_Restore
			
// 				Push register r4-11 to current stack 
			PUSH			{r4-r11}
	
//		OS_Curr->sp = sp;
			LDR           r1,=OS_Curr
			LDR           r1,[r1,#0x00]
			STR           sp,[r1,#0x00]
			
PendSV_Restore			
// 		sp = OS_Next->sp; 
			LDR           r1,=OS_Next
			LDR           r1,[r1,#0x00]
			LDR           sp,[r1,#0x00]

//		OS_Curr = OS_Next;  
			LDR           r1,=OS_Next
			LDR           r1,[r1,#0x00]
			LDR           r2,=OS_Curr
			STR           r1,[r2,#0x00]
			
//		Pop register r4-11 next stack
			POP				{r4-r11}
			
//		__enable_irq();
			CPSIE         I
//		}
}