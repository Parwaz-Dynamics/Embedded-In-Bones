#include "stm32f446xx.h"
#include "SystemClock_Config.h"

extern volatile uint32_t tickms;
volatile uint32_t tickms = 0;

// System Tick
void SysTick_Handler(void)
{
	tickms++;
}

void delay(uint32_t ms)
{
	uint32_t start = tickms;
	while(tickms - start < ms);
}

// GPIO
static void GPIOA_Init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	/* PA5 -> general purpose output (LED) */
	GPIOA->MODER &= ~(0x3U << (2 * 5));
	GPIOA->MODER |=  (0x1U << (2 * 5));
}

// ====================== I2C ======================

void I2C1_Config(void)
{
//	1. Enable the I2C CLOCK and GPIO CLOCK
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

//	2. Configure the I2C PINs for Alternate Functions
	GPIOB->MODER &= ~(0xFU << 16);
	GPIOB->MODER |=  (0xAU << 16);		// AF mode for PB8, PB9

	GPIOB->OTYPER |= (0x3U << 8);		// open-drain
	GPIOB->OSPEEDR |= (0xFU << 16);		// high speed

	GPIOB->PUPDR &= ~(0xFU << 16);
	GPIOB->PUPDR |=  (0x5U << 16);		// pull-up (keep external 4.7k too)

	GPIOB->AFR[1] &= ~(0xFFU << 0);
	GPIOB->AFR[1] |=  (0x44U << 0);		// AF4 = I2C1

//	3. Reset the I2C
	I2C1->CR1 |= I2C_CR1_SWRST;
	I2C1->CR1 &= ~I2C_CR1_SWRST;

//	4. Peripheral input clock (PCLK1 = 45 MHz)
	I2C1->CR2 |= (45 << 0);

//	5. Clock control (100 kHz standard mode)
	I2C1->CCR |= (225 << 0);

//	6. Rise time
	I2C1->TRISE |= (46 << 0);

//	7. Enable peripheral
	I2C1->CR1 |= I2C_CR1_PE;
}

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
	uint32_t temp __attribute__((unused)) = I2C1->SR1 | I2C1->SR2;
}

void I2C1_Stop(void)
{
	I2C1->CR1 |= I2C_CR1_STOP;
}

uint8_t I2C1_Read_Single(uint8_t devAddr, uint8_t regAddr)
{
	uint8_t data;

	// Phase 1: write the register pointer
	I2C1_Start();
	I2C1_SlaveAddress(devAddr << 1);
	I2C1_Write(regAddr);

	// Phase 2: repeated start, switch to read
	I2C1->CR1 |= I2C_CR1_START;
	while(!(I2C1->SR1 & I2C_SR1_SB));

	I2C1->CR1 &= ~I2C_CR1_ACK;			// NACK the single byte
	I2C1->DR = (devAddr << 1) | 0x01;	// address + read bit
	while(!(I2C1->SR1 & I2C_SR1_ADDR));

	(void)I2C1->SR1;
	(void)I2C1->SR2;
	I2C1->CR1 |= I2C_CR1_STOP;

	while(!(I2C1->SR1 & I2C_SR1_RXNE));
	data = (uint8_t)I2C1->DR;

	return data;
}

void I2C1_Read_Multi(uint8_t devAddr, uint8_t regAddr, uint8_t *buf, uint32_t len)
{
	// Phase 1: send register pointer (write mode)
	I2C1_Start();
	I2C1_SlaveAddress(devAddr << 1);
	I2C1_Write(regAddr);

	// Phase 2: repeated start, read mode
	I2C1->CR1 |= I2C_CR1_ACK;
	I2C1->CR1 |= I2C_CR1_START;
	while(!(I2C1->SR1 & I2C_SR1_SB));

	I2C1->DR = (devAddr << 1) | 0x01;
	while(!(I2C1->SR1 & I2C_SR1_ADDR));
	(void)I2C1->SR1; (void)I2C1->SR2;		// clear ADDR

	while(len > 3)							// read with ACK until 3 remain
	{
		while(!(I2C1->SR1 & I2C_SR1_RXNE));
		*buf++ = (uint8_t)I2C1->DR;
		len--;
	}

	// closing sequence for the final 3 bytes
	while(!(I2C1->SR1 & I2C_SR1_BTF));
	I2C1->CR1 &= ~I2C_CR1_ACK;				// NACK last byte
	*buf++ = (uint8_t)I2C1->DR;				// DataN-2

	while(!(I2C1->SR1 & I2C_SR1_BTF));
	I2C1->CR1 |= I2C_CR1_STOP;				// STOP
	*buf++ = (uint8_t)I2C1->DR;				// DataN-1

	while(!(I2C1->SR1 & I2C_SR1_RXNE));
	*buf++ = (uint8_t)I2C1->DR;				// DataN
}

// ====================== ICM-20948 ======================

#define ICM_ADDR		0x68		// try 0x69 if AD0 high
#define WHO_AM_I		0x00
#define PWR_MGMT_1		0x06
#define PWR_MGMT_2		0x07
#define ACCEL_XOUT_H	0x2D		// accel+gyro block starts here (Bank 0)

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

uint8_t  who;
uint8_t  raw[12];
int16_t  ax, ay, az, gx, gy, gz;
float    accel_g[3], gyro_dps[3];

int main(void)
{
	SystemClock_Config();
	SysTick_Config(180000);
	GPIOA_Init();
	I2C1_Config();

	who = I2C1_Read_Single(ICM_ADDR, WHO_AM_I);	// expect 0xEA
	ICM_Init();

	while(1)
	{
		I2C1_Read_Multi(ICM_ADDR, ACCEL_XOUT_H, raw, 12);

		ax = (int16_t)((raw[0]  << 8) | raw[1]);
		ay = (int16_t)((raw[2]  << 8) | raw[3]);
		az = (int16_t)((raw[4]  << 8) | raw[5]);
		gx = (int16_t)((raw[6]  << 8) | raw[7]);
		gy = (int16_t)((raw[8]  << 8) | raw[9]);
		gz = (int16_t)((raw[10] << 8) | raw[11]);

		
		// default ranges: ±2 g (16384 LSB/g), ±250 dps (131 LSB/dps)
		accel_g[0] = ax / 16384.0f;
		accel_g[1] = ay / 16384.0f;
		accel_g[2] = az / 16384.0f;
		gyro_dps[0] = gx / 131.0f;
		gyro_dps[1] = gy / 131.0f;
		gyro_dps[2] = gz / 131.0f;
		

		GPIOA->ODR ^= (0x1U << 5);		// heartbeat
		delay(100);						// ~10 Hz sampling
	}
}