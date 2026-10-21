
#include "stm32f446xx.h"
#include "stdint.h"

#include "bsp.h"

#include "blinky.h"

// GPIO

// Interrupts


int main(void)
{	
	SystemClock_Config();
	SysTick_Config(180000);
	GPIO_Init();
	
	__enable_irq();
	
	while(1)
	{
		LED_Toggle();
		delay(1000);
	}
	
	return 0;
}
