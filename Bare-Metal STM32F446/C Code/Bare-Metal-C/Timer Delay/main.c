#include "stm32f446xx.h"

void SystemClock_Config(void);
void GPIO_Config(void);

static void delay(int delay_time_us)
{
	while(delay_time_us--);
}

int main(void)
{
	SystemClock_Config();
	GPIO_Config();
	
	while(1)
	{
		GPIOA->BSRR |= (1<<5);	// Set output pin A5 high
		delay(10000000);
		
		GPIOA->BSRR |= ((1<<5)<<16);	// Set output pin A5 low
		delay(10000000);
	}
}

void SystemClock_Config(void)
{
		/*************>>>>>>> STEPS FOLLOWED <<<<<<<<************
	
	1. ENABLE HSE and wait for the HSE to become Ready
	2. Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR
	3. Configure the FLASH PREFETCH and the LATENCY Related Settings
	4. Configure the PRESCALARS HCLK, PCLK1, PCLK2
	5. Configure the MAIN PLL
	6. Enable the PLL and wait for it to become ready
	7. Select the Clock Source and wait for it to be set
	
	********************************************************/
	
	#define PLL_M		4
	#define PLL_N		180
	#define PLL_P		0 		// 0 means 2
	
	
//	1. ENABLE HSE and wait for the HSE to become Ready
	
	RCC->CR |= RCC_CR_HSEON;						// In RCC_CR enable HSEON bit which is 16th bit
	while(!(RCC->CR &	RCC_CR_HSERDY));	// Wait for HSERDY (clock ready flag) to become "1"
	
//	2. Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;	// Power Control Register
	PWR->CR |= PWR_CR_VOS;
	
//	3. Configure the FLASH PREFETCH and the LATENCY Related Settings
	FLASH->ACR =	FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_5WS;	//Embedded Flash Memory Interface
	
//	4. Configure the PRESCALARS HCLK, PCLK1, PCLK2

	//AHB Prescalar
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;    
	
	// APB1 Prescalar
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;
	
	// APB2 Prescalar
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
	
//	5. Configure the MAIN PLL

	// PLL M
	RCC->PLLCFGR = (PLL_M << 0) | (PLL_N << 6) | (PLL_P << 16);


	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;
	
//	6. Enable the PLL and wait for it to become ready
	RCC->CR |= RCC_CR_PLLON;
	while(!(RCC->CR & RCC_CR_PLLRDY));
	
//	7. Select the Clock Source and wait for it to be set
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
	
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
	GPIOA->OTYPER = 0;
	GPIOA->OSPEEDR = 0;
}
