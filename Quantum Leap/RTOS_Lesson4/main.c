
#include "stm32f446xx.h"
#include "stdint.h"

#include "bsp.h"

#include "blinky.h"

#include "miros.h"

// GPIO

// Interrupts

uint32_t stack_blinky1[40];
OSThread blinky1;

void main_blinky1(void){		
	while(1)
	{
		Green_LED_On();
		OS_Delay(250);
		Green_LED_Off();
		OS_Delay(250 * 3);
	}
}

uint32_t stack_blinky2[40];
OSThread blinky2;

void main_blinky2(void)
{		
	while(1)
	{		
		Yellow_LED_On();
		OS_Delay(250 * 2);
		Yellow_LED_Off();
		OS_Delay(1000 / 3);
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
								 &main_blinky1,
								stack_blinky1, sizeof(stack_blinky1));
	
	// start blinky thread 2
	OSThread_Start(&blinky2,
								 &main_blinky2,
								stack_blinky2, sizeof(stack_blinky2));
	
	// start blinky thread 3
	OSThread_Start(&blinky3,
								 &main_blinky3,
								stack_blinky3, sizeof(stack_blinky3));
	
	// transfer control to RTOS to run threads
	OS_Run();
	
	return 0;
}

