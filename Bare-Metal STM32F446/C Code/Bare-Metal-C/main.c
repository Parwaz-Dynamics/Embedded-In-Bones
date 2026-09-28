#include "SystemClock_Config.h"

// SysTick Variables
extern volatile uint32_t tick_ms;
volatile uint32_t tick_ms = 0;

void delay(uint32_t time);

//	GPIO
void GPIO_Config(void);

int main(void)
{
	SystemClock_Config();
	GPIO_Config();
	
	SysTick_Config(180000);
	
	; Theory Mostly, FPU uses different Registers s0, s1, ... s31 or D0, D1, ... D15
	; It has it's own instruction set like VADD, VSUB
	; If a MCU does not have FPU it requires software library.
	
	volatile float a = 0.7f, b = 0.0f, c, d, e, g;
	
	c = a + b;
	d = a * b;
	e = a / b;
	g = e + b;
	
	while(1);
}

// SysTick Variable
void delay(uint32_t time)
{
	uint32_t start = tick_ms;
	while((tick_ms - start) < time);
}

// GPIO	
void GPIO_Config(void)
{
//	Enable Clock for GPIOA 
	RCC->AHB1ENR |= (1<<0);
	
//	Set Pin as Output 
	GPIOA->MODER |= (1<<10) | (0<<11);
	
//	Pin Configurations -> Default for now
}
