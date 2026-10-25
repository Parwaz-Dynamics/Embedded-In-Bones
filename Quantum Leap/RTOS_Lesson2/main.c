#include "stm32f446xx.h"
#include "stdint.h"

#include "bsp.h"

#include "blinky.h"

#include "miros.h"


// Fake Start
uint32_t stack_blinky1[40];		

// Stack Pointer for blinky1
OSThread sp_blinky1;	// Since OSThread have only *sp which is a pointer
											// so sp_blinky1 is a pointer

void main_blinky1(void){		
	while(1)
	{
		Green_LED_On();
		delay(250);
		Green_LED_Off();
		delay(250 * 3);
	}
}

uint32_t stack_blinky2[40];
OSThread sp_blinky2;

void main_blinky2(void)
{		
	while(1)
	{		
		Yellow_LED_On();
		delay(250 * 2);
		Yellow_LED_Off();
		delay(1000 / 3);
	}
}

int main(void)
{	
	SystemClock_Config();
	SysTick_Config(180000);
	GPIO_Init();
	
	OS_Init();
	
	// Set SysTick interrupt priority to highest
	NVIC_SetPriority(SysTick_IRQn, 0U);
	
	__enable_irq();
	
	/* fabricate Cortex-M ISR stack frame for blinky1*/
	
	OSThread_Start(&sp_blinky1,
								 &main_blinky1,
								stack_blinky1, sizeof(stack_blinky1));
	
	OSThread_Start(&sp_blinky2,
								 &main_blinky2,
								stack_blinky2, sizeof(stack_blinky2));
	
	while(1){}
	
	return 0;
}

