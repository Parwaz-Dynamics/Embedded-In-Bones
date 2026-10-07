
#include "SystemInitSettings.h"
#include "delay.h"
#include "blinky_driver.h"

int main(void)
{
	SystemClock_Init();
	delay_init();
	blinky_init();
	while(1)	
	{
		blinky();
	}
}
