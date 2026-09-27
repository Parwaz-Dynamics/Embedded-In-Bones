#include "SystemClock_Config.h"

void TIM6_Config(void);
void delay_us(uint16_t us);
void delay_ms(uint16_t ms);
void GPIO_Config(void);


int main(void)
{
	SystemClock_Config();
	TIM6_Config();
	GPIO_Config();
	
	while(1)
	{
		GPIOA->BSRR |= (1<<5);	// Set output pin A5 high
		delay_ms(250);
		
		GPIOA->BSRR |= ((1<<5)<<16);	// Set output pin A5 low
		delay_ms(250);

	}
}

void TIM6_Config(void)
{
	/*************>>>>>>> STEPS FOLLOWED <<<<<<<<************
	
	1. Enable Timer Clock
	2. Set the prescalar and the ARR
	3. Enable the timer and wait for update flag to set
	
	********************************************************/
	
	#define PRESCALAR 90 - 1
	#define AUTORELOAD 0xFFFF
	
//	1. Enable Timer Clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;		// Enable Timer 6 Clock
	
//	2. Set the prescalar and the ARR
	TIM6->PSC = PRESCALAR;						// Prescalar = 90 - Tick = 90MHz / 90 = 1MHz 
	TIM6->ARR = AUTORELOAD;						// ARR to maximum
	
//	3. Enable the timer and wait for update flag to set
	TIM6->CR1 |= (1<<0);
	while(!(TIM6->SR & (1<<0)));
}

void delay_us(uint16_t us)
{
	/************** STEPS TO FOLLOW *****************
	1. RESET the Counter
	2. Wait for the Counter to reach the entered value. As each count will take 1 us, 
		 the total waiting time will be the required us delay
	************************************************/
	
	TIM6->CNT = 0;
	while(TIM6->CNT < us);
}

void delay_ms(uint16_t ms)
{
	for(int i = 0; i < ms; i++)
	{
		delay_us(1000);
	}
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
