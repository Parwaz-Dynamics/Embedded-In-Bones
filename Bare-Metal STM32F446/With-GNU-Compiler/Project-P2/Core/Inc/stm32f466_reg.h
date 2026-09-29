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