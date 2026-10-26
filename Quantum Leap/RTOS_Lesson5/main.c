
#include "stm32f446xx.h"
#include "stdint.h"

#include "bsp.h"

#include "blinky.h"

#include "miros.h"

uint32_t stack_blinky1[40];
OSThread blinky1;

void main_blinky1(void){		
	while(1)
	{
		uint32_t volatile i;
		
		for(i = 5400; i > 0; --i)
		{
			Green_LED_On();
			Green_LED_Off();
		}
		OS_Delay(1U);
	}
}

uint32_t stack_blinky2[40];
OSThread blinky2;

void main_blinky2(void)
{		
	while(1)
	{
		uint32_t volatile i;
		
		for(i = 3 * 5400; i > 0; --i)
		{
			Yellow_LED_On();
			Yellow_LED_Off();
		}
		OS_Delay(50U);
	}
}

uint32_t stack_blinky3[40];
OSThread blinky3;

void main_blinky3(void)
{		
	while(1)
	{		
		Red_LED_On();
		OS_Delay(1000 / 3);
		Red_LED_Off();
		OS_Delay(1000 * 3 / 5);
	}
}

uint32_t stack_idleThread[40];

int main(void)
{	
	GPIO_Init();
	OS_Init(stack_idleThread, sizeof(stack_idleThread));
	
	// start blinky thread 1
	OSThread_Start(&blinky1,
									5U,
								 &main_blinky1,
								stack_blinky1, sizeof(stack_blinky1));
	
	// start blinky thread 2
	OSThread_Start(&blinky2,
									2U,
								 &main_blinky2,
								stack_blinky2, sizeof(stack_blinky2));
	
//	// start blinky thread 3
//	OSThread_Start(&blinky3,
//								 &main_blinky3,
//								stack_blinky3, sizeof(stack_blinky3));
	
	// transfer control to RTOS to run threads
	OS_Run();
	
	return 0;
}

