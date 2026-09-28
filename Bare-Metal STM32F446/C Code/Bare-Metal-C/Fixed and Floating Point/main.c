

#include "SystemClock_Config.h"
#include "stdio.h"

// SysTick Variables
extern volatile uint32_t tick_ms;
volatile uint32_t tick_ms = 0;

void delay(uint32_t time);

//	GPIO
void GPIO_Config(void);

// Fixed Point
typedef uint32_t q16_16;
#define FRAC_BIT 16
#define SCALE (1<<FRAC_BIT)

q16_16 to_fixed(float x);
float   to_float(q16_16 x);
q16_16 mul(q16_16 a, q16_16 b);



int main(void)
{
	SystemClock_Config();
	GPIO_Config();
	
	SysTick_Config(180000);
	
	
	float fn1 = 1.5;
	float fn2 = 2.0;
	q16_16 in1 = to_fixed(fn1);
	q16_16 in2 = to_fixed(fn2);
	
	// Addition
	// Addition Float
	float fn_add1 __attribute__((unused)) = fn1 + fn2;
	
	// Addition Fixed
	q16_16 in_add = in1 + in2;
	float fn_add2 __attribute__((unused)) = to_float(in_add);
	
	// Addition
	// Subtraction Float
	float fn_sub1 __attribute__((unused)) = fn1 - fn2;
	
	// Subtraction Fixed
	q16_16 in_sub = in1 - in2;
	float fn_sub2 __attribute__((unused)) = to_float(in_sub);
	
	// Multiplication
	// Multiplication Float
	float fn_mul1 __attribute__((unused)) = fn1 * fn2;
	
	// Subtraction Fixed
	q16_16 in_mul = mul(in1, in2);
	float fn_mul2 __attribute__((unused)) = to_float(in_mul);
	
	
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

// Fixed Point
q16_16 to_fixed(float x)   
{ 
	return (q16_16)(x * SCALE); 
}

float   to_float(q16_16 x) 
{ 
	return (float)x / SCALE; 
}

q16_16 mul(q16_16 a, q16_16 b) {
    return (q16_16)(((int64_t)a * b) >> FRAC_BIT);
}
