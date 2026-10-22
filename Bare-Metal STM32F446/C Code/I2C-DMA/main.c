#include "stm32f446xx.h"
#include "SystemClock_Config.h"

extern volatile uint32_t tickms;
volatile uint32_t tickms = 0;

void SysTick_Handler(void) { tickms++; }

void delay(uint32_t ms)
{
	uint32_t start = tickms;
	while(tickms - start < ms);
}

// ====================== GPIO ======================
static void GPIOA_Init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER &= ~(0x3U << (2 * 5));
	GPIOA->MODER |=  (0x1U << (2 * 5));		// PA5 LED
}

// ====================== I2C ======================
void I2C1_Config(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	GPIOB->MODER &= ~(0xFU << 16);
	GPIOB->MODER |=  (0xAU << 16);		// AF mode PB8, PB9
	GPIOB->OTYPER |= (0x3U << 8);		// open-drain
	GPIOB->OSPEEDR |= (0xFU << 16);		// high speed
	GPIOB->PUPDR &= ~(0xFU << 16);
	GPIOB->PUPDR |=  (0x5U << 16);		// pull-up (keep external 4.7k)
	GPIOB->AFR[1] &= ~(0xFFU << 0);
	GPIOB->AFR[1] |=  (0x44U << 0);		// AF4 = I2C1

	I2C1->CR1 |= I2C_CR1_SWRST;
	I2C1->CR1 &= ~I2C_CR1_SWRST;

	I2C1->CR2 |= (45 << 0);				// PCLK1 = 45 MHz

	I2C1->CCR = 0;
	I2C1->CCR |= I2C_CCR_FS;			// fast mode
	I2C1->CCR |= (37 << 0);				// 400 kHz
	I2C1->TRISE = 14;

	I2C1->CR1 |= I2C_CR1_PE;

	NVIC_EnableIRQ(I2C1_EV_IRQn);
	NVIC_EnableIRQ(I2C1_ER_IRQn);
}

// --- polling helpers (used only by ICM_Init / WHO_AM_I) ---
void I2C1_Start(void)
{
	I2C1->CR1 |= I2C_CR1_ACK;
	I2C1->CR1 |= I2C_CR1_START;
	while(!(I2C1->SR1 & I2C_SR1_SB));
}
void I2C1_Write(uint8_t data)
{
	while(!(I2C1->SR1 & I2C_SR1_TXE));
	I2C1->DR = 0xFFU & data;
	while(!(I2C1->SR1 & I2C_SR1_BTF));
}
void I2C1_SlaveAddress(uint8_t address)
{
	I2C1->DR = address;
	while(!(I2C1->SR1 & I2C_SR1_ADDR));
	uint32_t t __attribute__((unused)) = I2C1->SR1 | I2C1->SR2;
}
void I2C1_Stop(void) { I2C1->CR1 |= I2C_CR1_STOP; }

uint8_t I2C1_Read_Single(uint8_t devAddr, uint8_t regAddr)
{
	uint8_t data;
	I2C1_Start();
	I2C1_SlaveAddress(devAddr << 1);
	I2C1_Write(regAddr);
	I2C1->CR1 |= I2C_CR1_START;
	while(!(I2C1->SR1 & I2C_SR1_SB));
	I2C1->CR1 &= ~I2C_CR1_ACK;
	I2C1->DR = (devAddr << 1) | 0x01;
	while(!(I2C1->SR1 & I2C_SR1_ADDR));
	(void)I2C1->SR1; (void)I2C1->SR2;
	I2C1->CR1 |= I2C_CR1_STOP;
	while(!(I2C1->SR1 & I2C_SR1_RXNE));
	data = (uint8_t)I2C1->DR;
	return data;
}

// ====================== Interrupt-driven read ======================
volatile uint8_t  *i2c_buf;
volatile uint32_t  i2c_len, i2c_idx;
volatile uint8_t   i2c_devaddr, i2c_reg;
volatile uint8_t   i2c_busy;			// read in progress
volatile uint8_t   i2c_ready;			// new data in buffer
volatile uint8_t   i2c_error;
volatile uint32_t  i2c_overruns;		// timer fired while still busy

typedef enum { ST_SEND_REG, ST_REG_SENT, ST_READ_ADDR, ST_READ_DATA } i2c_state_t;
volatile i2c_state_t i2c_state;

void I2C1_Read_Multi_IT(uint8_t devAddr, uint8_t reg, volatile uint8_t *buf, uint32_t len)
{
	i2c_buf = buf;
	i2c_len = len;
	i2c_idx = 0;
	i2c_devaddr = devAddr;
	i2c_reg = reg;
	i2c_busy = 1;
	i2c_error = 0;
	i2c_state = ST_SEND_REG;

	I2C1->CR2 |= I2C_CR2_ITEVTEN | I2C_CR2_ITBUFEN | I2C_CR2_ITERREN;
	I2C1->CR1 |= I2C_CR1_ACK;
	I2C1->CR1 |= I2C_CR1_START;
}

void I2C1_EV_IRQHandler(void)
{
	uint32_t sr1 = I2C1->SR1;

	if (sr1 & I2C_SR1_SB)
	{
		if (i2c_state == ST_SEND_REG) I2C1->DR = (i2c_devaddr << 1);
		else                          I2C1->DR = (i2c_devaddr << 1) | 0x01;
	}

	if (sr1 & I2C_SR1_ADDR)
	{
		if (i2c_state == ST_READ_ADDR && i2c_len == 1)
			I2C1->CR1 &= ~I2C_CR1_ACK;
		(void)I2C1->SR1; (void)I2C1->SR2;		// clear ADDR
		if (i2c_state == ST_READ_ADDR) i2c_state = ST_READ_DATA;
	}

	if ((sr1 & I2C_SR1_TXE) && i2c_state == ST_SEND_REG)
	{
		I2C1->DR = i2c_reg;
		i2c_state = ST_REG_SENT;
	}

	if ((sr1 & I2C_SR1_BTF) && i2c_state == ST_REG_SENT)
	{
		i2c_state = ST_READ_ADDR;
		I2C1->CR1 |= I2C_CR1_START;				// repeated START
	}

	if ((sr1 & I2C_SR1_RXNE) && i2c_state == ST_READ_DATA)
	{
		i2c_buf[i2c_idx++] = (uint8_t)I2C1->DR;
		if (i2c_idx == i2c_len - 1)
			I2C1->CR1 &= ~I2C_CR1_ACK;			// NACK last byte
		if (i2c_idx == i2c_len)
		{
			I2C1->CR1 |= I2C_CR1_STOP;
			I2C1->CR2 &= ~(I2C_CR2_ITEVTEN | I2C_CR2_ITBUFEN | I2C_CR2_ITERREN);
			i2c_busy = 0;
			i2c_ready = 1;
		}
	}
}

void I2C1_ER_IRQHandler(void)
{
	i2c_error = (uint8_t)I2C1->SR1;
	I2C1->SR1 &= ~(I2C_SR1_AF | I2C_SR1_BERR | I2C_SR1_ARLO | I2C_SR1_OVR);
	I2C1->CR1 |= I2C_CR1_STOP;
	I2C1->CR2 &= ~(I2C_CR2_ITEVTEN | I2C_CR2_ITBUFEN | I2C_CR2_ITERREN);
	i2c_busy = 0;
}

// ====================== TIM5: free-running microsecond clock ======================
void TIM5_FreeRun_us(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
	TIM5->PSC = 90 - 1;				// 90 MHz / 90 = 1 MHz -> 1 us tick
	TIM5->ARR = 0xFFFFFFFF;			// 32-bit free run (~71 min wrap)
	TIM5->CR1 |= TIM_CR1_CEN;
}
static inline uint32_t micros(void) { return TIM5->CNT; }

// ====================== TIM2: 1 kHz sample tick ======================
#define ICM_ADDR		0x68
#define WHO_AM_I		0x00
#define PWR_MGMT_1		0x06
#define PWR_MGMT_2		0x07
#define ACCEL_XOUT_H	0x2D

static volatile uint8_t  raw[12];
volatile uint32_t dt_us;				// measured sample period
static uint32_t   t_prev;

void TIM2_Config_1kHz(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->PSC = 90 - 1;				// 1 MHz tick
	TIM2->ARR = 1000 - 1;			// 1 kHz update
	
	TIM2->EGR = TIM_EGR_UG;			// force update: load PSC/ARR now
	TIM2->SR &= ~TIM_SR_UIF;		// clear the flag UG just raised
	
	TIM2->DIER |= TIM_DIER_UIE;
	TIM2->CR1  |= TIM_CR1_CEN;
	NVIC_EnableIRQ(TIM2_IRQn);
}

void TIM2_IRQHandler(void)
{
	if (TIM2->SR & TIM_SR_UIF)
	{
		TIM2->SR &= ~TIM_SR_UIF;

		uint32_t now = micros();
		static uint8_t first = 1;
		if (first) { first = 0; t_prev = now; }   // seed, skip bogus dt
		else       { dt_us = now - t_prev; t_prev = now; }

		if (!i2c_busy)
			I2C1_Read_Multi_IT(ICM_ADDR, ACCEL_XOUT_H, raw, 12);
		else
			i2c_overruns++;
	}
}

// ====================== ICM-20948 init (polling) ======================
void ICM_WriteReg(uint8_t reg, uint8_t val)
{
	I2C1_Start();
	I2C1_SlaveAddress(ICM_ADDR << 1);
	I2C1_Write(reg);
	I2C1_Write(val);
	I2C1_Stop();
}

void ICM_Init(void)
{
	ICM_WriteReg(PWR_MGMT_1, 0x01);		// wake, auto clock
	delay(10);
	ICM_WriteReg(PWR_MGMT_2, 0x00);		// enable accel + gyro
	delay(10);
}

// ====================== Main ======================
static uint8_t  who;
static int16_t  ax, ay, az, gx, gy, gz;
static float    accel_g[3], gyro_dps[3];
static uint32_t sample_count;

int main(void)
{
	SystemClock_Config();
	SysTick_Config(180000);
	GPIOA_Init();
	I2C1_Config();
	TIM5_FreeRun_us();						// start us clock first

	who = I2C1_Read_Single(ICM_ADDR, WHO_AM_I);	// expect 0xEA
	ICM_Init();
	
	delay(1000);

	t_prev = micros();
	TIM2_Config_1kHz();						// begin 1 kHz sampling

	while(1)
	{
		if (i2c_ready)
		{
			i2c_ready = 0;

			ax = (int16_t)((raw[0]  << 8) | raw[1]);
			ay = (int16_t)((raw[2]  << 8) | raw[3]);
			az = (int16_t)((raw[4]  << 8) | raw[5]);
			gx = (int16_t)((raw[6]  << 8) | raw[7]);
			gy = (int16_t)((raw[8]  << 8) | raw[9]);
			gz = (int16_t)((raw[10] << 8) | raw[11]);

			accel_g[0]  = ax / 16384.0f;
			accel_g[1]  = ay / 16384.0f;
			accel_g[2]  = az / 16384.0f;
			gyro_dps[0] = gx / 131.0f;
			gyro_dps[1] = gy / 131.0f;
			gyro_dps[2] = gz / 131.0f;

			sample_count++;
			if ((sample_count % 500) == 0)
				GPIOA->ODR ^= (0x1U << 5);	// LED ~every 0.5 s
		}
		// CPU free between samples (could __WFI())
	}
}