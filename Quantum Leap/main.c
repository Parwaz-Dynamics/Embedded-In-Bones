
#include "stm32f446xx.h"
#include "stdint.h"

#include "rcc.h"
#include "systick.h"

// Pointers
//static uint32_t a = 5;
//static uint32_t b = 6;

//void swap(uint32_t *a, uint32_t *b);
//void swap(uint32_t *a, uint32_t *b)
//{
//	*a = *a ^ *b;
//	*b = *a ^ *b;
//	*a = *a ^ *b;
//}

uint8_t u8a, u8b;
uint16_t u16c, u16d;
uint32_t u32e, u32f;

int main(void)
{	
	SystemClock_Config();
	SysTick_Config(180000);
	
//	swap(&a, &b);

	u8a = 0xffU;
	u16c = 0xc1c2U;
	u32e = 0xe1e2e3e4U;
	
	u8b = ~u8a;
	u16d = u16c;
	u32f = u32e;
	
	if((uint8_t)~u8a == 0x00U)
	{
		u8a = 0x00U;
	}
	
	while(1)
	{
//		delay(1000);
	}
	
	return 0;
}
