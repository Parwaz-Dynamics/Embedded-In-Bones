#include "SystemClock_Config.h"

void delay_us(uint16_t us);
void delay_ms(uint32_t ms);
void GPIO_Config(void);

extern volatile uint32_t ms_ticks;   // declaration (no initializer!)
volatile uint32_t ms_ticks = 0;      // definition
void SysTick_Handler(void) 				// This function is also defined in startup asm file
{
	ms_ticks++;
}


int main(void)
{
	SystemClock_Config();
	SysTick_Config(180000);
	GPIO_Config();
	
	while(1)
	{
		GPIOA->BSRR |= (1<<5);	// Set output pin A5 high
		delay_ms(250);
		
		GPIOA->BSRR |= ((1<<5)<<16);	// Set output pin A5 low
		delay_ms(250);

	}
}

void delay_ms(uint32_t ms)
{
    uint32_t start = ms_ticks;
    while ((ms_ticks - start) < ms);   // wait until 'ms' ticks have elapsed
}

void GPIO_Config(void)
{
	
	/*************>>>>>>> STEPS FOLLOWED <<<<<<<<************
	
	1. Enable GPIO Clock
	2. Set the pin as output
	3. Configure the output mode
	
	********************************************************/
	
//	1. Enable GPIO Clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

//	2. Set the pin as output
	GPIOA->MODER |= (1<<10);
	
//	3. Configure the output mode
	GPIOA->OTYPER &= ~(1U<<5);  // bit 5=0 --> Output push pull
	GPIOA->OSPEEDR |= (1<<11);  // Pin PA5 (bits 11:10) as Fast Speed (1:0)
	GPIOA->PUPDR &= ~((1U<<10) | (1U<<11));  // Pin PA5 (bits 11:10) are 0:0 --> no pull up or pulldown
}
