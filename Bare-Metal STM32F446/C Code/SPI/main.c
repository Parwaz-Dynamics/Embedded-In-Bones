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
	
	// SPI nCS
	
	GPIOA->MODER &= ~(0x3U << (2 * 9));
	GPIOA->MODER |=  (0x1U << (2 * 8));		// PA5 LED
	
	// SPI MOSI, MISO, SCK
	
	GPIOA->MODER &= ~(0x3FU << (2 * 5));
	GPIOA->MODER |= (0x2AU << (2 * 5));
	
	GPIOA->OSPEEDR |= (0x3FU << (2 * 5));
	GPIOA->OSPEEDR |= (0x3U << (2 * 9));
	
	GPIOA->AFR[0] |= (0x5U << 20) | (0x5U << 24) | (0x5U << 28);
	
	
}

// ====================== SPI ======================

void SPI1_Config(void)
{
	/************** STEPS TO FOLLOW *****************
	1. Enable SPI clock
	2. Configure the Control Register 1
	3. Configure the CR2
	************************************************/
	
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	
	SPI1->CR1 |= (0x3U << 0); // CPOL & CPHA - mode 3
	
	SPI1->CR1 |= SPI_CR1_MSTR;	// Select STM32 as Master
	
	SPI1->CR1 |= (0x3U << 3);		// Select the baudrate 5M = fclk / 16 (over clk is 160Mhz for now)
	
	SPI1->CR1 &= ~SPI_CR1_LSBFIRST;		// Send MSB First Mode
	
	SPI1->CR1 |= (0x3U << 8);					// Slave Select Settings
	
	SPI1->CR1 &= ~SPI_CR1_RXONLY;			// Full Duplex 
	
	SPI1->CR1 &= ~SPI_CR1_DFF;				// 8 bit data format
	
	SPI1->CR2 = 0;	
}

void SPI1_Enable(void)
{
	SPI1->CR1 |= SPI_CR1_SPE;
}

void SPI1_Disable(void)
{
	SPI1->CR1 &= ~SPI_CR1_SPE;
}

void SPI1_CS_Enable(void)
{
	GPIOA->BSRR |= ((0x1U << 9) << 16);
}

void SPI1_CS_Disable(void)
{
	GPIOA->BSRR |= (0x1U << 9);
}

void SPI1_Transmit (uint8_t *data, int size)
{
	
	/************** STEPS TO FOLLOW *****************
	1. Wait for the TXE bit to set in the Status Register
	2. Write the data to the Data Register
	3. After the data has been transmitted, wait for the BSY bit to reset in Status Register
	4. Clear the Overrun flag by reading DR and SR
	************************************************/		
	
	int i=0;
	while (i<size)
	{
	   while (!(SPI1->SR & SPI_SR_TXE));  // wait for TXE bit to set -> This will indicate that the buffer is empty
	   SPI1->DR = data[i];  // load the data into the Data Register
	   i++;
	}	
	
	
	/*During discontinuous communications, there is a 2 APB clock period delay between the
	write operation to the SPI_DR register and BSY bit setting. As a consequence it is
	mandatory to wait first until TXE is set and then until BSY is cleared after writing the last
	data.
	*/
	while (!(SPI1->SR & SPI_SR_TXE));  // wait for TXE bit to set -> This will indicate that the buffer is empty
	while (SPI1->SR & SPI_SR_BSY);  // wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication	
	
	//  Clear the Overrun flag by reading DR and SR
	(void)SPI1->DR;
	(void)SPI1->SR;
	
}

void SPI_Receive (uint8_t *data, int size)
{
	/************** STEPS TO FOLLOW *****************
	1. Wait for the BSY bit to reset in Status Register
	2. Send some Dummy data before reading the DATA
	3. Wait for the RXNE bit to Set in the status Register
	4. Read data from Data Register
	************************************************/		

	while (size)
	{
		while (SPI1->SR & SPI_SR_BSY);  // wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication
		SPI1->DR = 0;  // send dummy data
		while (!(SPI1->SR & SPI_SR_RXNE));  // Wait for RXNE to set -> This will indicate that the Rx buffer is not empty
	  *data++ = (uint8_t) SPI1->DR;
		size--;
	}	
}

int main(void)
{
	SystemClock_Config();
	SysTick_Config(180000);
	GPIOA_Init();
	SPI1_Config();
	
	uint8_t data[3] = {0x02, 0x32, 0xf6};
	
	SPI1_Enable();
	SPI1_CS_Enable();
	SPI1_Transmit(data, 3);
	SPI1_CS_Disable();

	while(1)
	{
//		GPIOA->ODR ^= 1<<5;
//		delay(500);
	}
}

