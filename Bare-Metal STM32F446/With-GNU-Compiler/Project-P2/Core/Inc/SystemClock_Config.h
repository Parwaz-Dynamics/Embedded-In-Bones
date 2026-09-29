#ifndef SYSTEMCLOCK_CONFIG_H
#define SYSTEMCLOCK_CONFIG_H

#include <stdint.h>

/* ---------- Register access ---------- */
#define REG32(addr)             (*(volatile uint32_t *)(addr))

/* ---------- Base addresses ---------- */
#define RCC_BASE                0x40023800UL
#define PWR_BASE                0x40007000UL
#define FLASH_IF_BASE           0x40023C00UL

/* ---------- RCC registers ---------- */
#define RCC_CR                  REG32(RCC_BASE + 0x00)
#define RCC_PLLCFGR             REG32(RCC_BASE + 0x04)
#define RCC_CFGR                REG32(RCC_BASE + 0x08)
#define RCC_APB1ENR             REG32(RCC_BASE + 0x40)

/* ---------- PWR registers ---------- */
#define PWR_CR                  REG32(PWR_BASE + 0x00)
#define PWR_CSR                 REG32(PWR_BASE + 0x04)

/* ---------- FLASH interface registers ---------- */
#define FLASH_ACR               REG32(FLASH_IF_BASE + 0x00)

void SystemClock_Config(void)
{
	/*************>>>>>>> STEPS FOLLOWED <<<<<<<<************

	1. ENABLE HSE and wait for the HSE to become Ready
	2. Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR
	3. Configure the FLASH PREFETCH and the LATENCY Related Settings
	4. Configure the PRESCALARS HCLK, PCLK1, PCLK2
	5. Configure the MAIN PLL
	6. Enable the PLL and wait for it to become ready
	7. Enable OVER-DRIVE (needed for 180 MHz)
	8. Select the Clock Source and wait for it to be set

	********************************************************/

	#define PLL_M		4		// assumes 8 MHz HSE -> 2 MHz VCO input
	#define PLL_N		180		// VCO = 360 MHz
	#define PLL_P		0		// 0 means /2 -> 180 MHz
	#define PLL_Q		4		// must be 2..15, 0 is not allowed
	#define PLL_R		2		// must be 2..7,  0 is not allowed

//	1. ENABLE HSE and wait for the HSE to become Ready
	RCC_CR |= (1UL << 16);
	while(!(RCC_CR & (1UL << 17)));

//	2. Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR
	RCC_APB1ENR |= (1UL << 28);
	PWR_CR |= (3UL << 14);

//	3. Configure the FLASH PREFETCH and the LATENCY Related Settings
	FLASH_ACR = (1UL << 9) | (1UL << 10) | (1UL << 8) | (5UL << 0);

//	4. Configure the PRESCALARS HCLK, PCLK1, PCLK2
	RCC_CFGR |= (5UL << 10);             // APB1 = 45 MHz
	RCC_CFGR |= (4UL << 13);             // APB2 = 90 MHz

//	5. Configure the MAIN PLL
	RCC_PLLCFGR = (PLL_M << 0) | (PLL_N << 6) | (PLL_P << 16) | (PLL_Q << 24) | (PLL_R << 28);
	RCC_PLLCFGR |= (1UL << 22);

//	6. Enable the PLL and wait for it to become ready
	RCC_CR |= (1UL << 24);
	while(!(RCC_CR & (1UL << 25)));

//	7. Enable over-drive and wait for it to switch
	PWR_CR |= (1UL << 16);
	while(!(PWR_CSR & (1UL << 16)));
	PWR_CR |= (1UL << 17);
	while(!(PWR_CSR & (1UL << 17)));

//	8. Select the Clock Source and wait for it to be set
	RCC_CFGR |= (2UL << 0);
	while((RCC_CFGR & (3UL << 2)) != (2UL << 2));
}

#endif