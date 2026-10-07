#include "stm32f446xx.h"

#define In_Out volatile
	
typedef struct 
{
	In_Out uint32_t MODER;
	In_Out uint16_t OTYPER;
	In_Out uint16_t rev0;
	In_Out uint32_t OSPEEDER;
	In_Out uint32_t PUPDR;
	In_Out uint16_t IDR;
	In_Out uint16_t ODR;
	In_Out uint32_t BSRR;
	In_Out uint16_t LCKR;
	In_Out uint16_t rev1;
	In_Out uint32_t AFR[2];
	
}	GPIO_Type;

#define GPIO_A ((GPIO_Type*) 0x40020000)

void blinky_init(void);
void blinky(void);

void blinky_init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	
	GPIO_A->MODER = (01<<10);
}

void blinky()
{
	GPIO_A->BSRR |= (1<<5);
	delay(1000);
	
	GPIO_A->BSRR |= ((1<<5)<<16);
	delay(1000);
}
