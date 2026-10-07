
#include "stm32f446xx.h"
#include "SystemClock_Config.h"

extern volatile uint32_t tickms;
volatile uint32_t tickms = 0;

void delay(uint32_t time);
void GPIOA_Init(void);

void USART1_Init(void);

void USART1_Write(uint8_t *buffer, uint16_t size);
void USART1_Read(uint8_t *buffer, uint16_t size);

uint8_t TxData[15] = "Hello\r\n";
uint8_t RxData[15];

int main(void)
{
	SystemClock_Config();
	SysTick_Config(180000);
	GPIOA_Init();
	USART1_Init();
	
	while(1)
	{
		delay(1000);
		GPIOA->ODR ^= 1<<5;
//		USART1_Write(TxData, 15);
		USART1_Read(RxData, 5);
	}
	
}

void delay(uint32_t time)
{
	uint32_t current = tickms;
	while(tickms - current < time);
}

void GPIOA_Init(void)
{
	// Enable Clock for PortA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	
	// Select the pin mode od Pin A5 as output
	GPIOA->MODER &= ~(2 << 10);
	GPIOA->MODER |= (01 << 10);
	
	// Enable USART1 on APB2
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

  // PA9 / PA10 -> alternate function mode (MODER = 0b10)
  GPIOA->MODER &= ~(0xF << (2*9));
  GPIOA->MODER |=  0xA << (2*9);

	// Output type push-pull and high speed for the TX pin */
  GPIOA->OSPEEDR |= (3 << (2*9));   // very high speed

  /* Alternate function = AF7 (USART1) for PA9 and PA10.
   *    Pins 8..15 use AFR[1] (AFRH); each pin is a 4-bit field.
   *    PA9  -> AFRH field (9-8)=1  -> bits [7:4]
   *    PA10 -> AFRH field (10-8)=2 -> bits [11:8]
   */
  GPIOA->AFR[1] &= ~((0xFU << ((9 - 8) * 4)) | (0xFU << ((10 - 8) * 4)));
  GPIOA->AFR[1] |=  ((7U   << ((9 - 8) * 4)) | (7U   << ((10 - 8) * 4)));	
}


void USART1_Init(void)
{
	// Disable the USART
	USART1->CR1 &= ~USART_CR1_UE;		// Cleared Usart Enable bit
	
	// Set data length to 8 bits
	USART1->CR1 &= ~USART_CR1_M;
	
	// Select stop bit 1
	USART1->CR2 &= ~USART_CR2_STOP;
	
	// Set no parity
	USART1->CR1 &= ~USART_CR1_PCE;
	
	// Sampling rate 16 bit
	USART1->CR1 &= ~USART_CR1_OVER8;
	
	// Set the Baudrate
//	BRR = fCK / baud (OVER8 = 0) = 90,000,000 / 9600 = 9375 = 0x249F
	USART1->BRR = 0x249F;
	
	// Enable Transmission and Reception
	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;
	
	// Enable USART
	USART1->CR1 |= USART_CR1_UE;
}

void USART1_Write(uint8_t *buffer, uint16_t size)
{
	// For multiple bytes
	for(int i = 0; i < size; i++)
	{
		// In Status Register check if previous data is already sent
		while (!(USART1->SR & USART_SR_TXE)){}
		// Send the new byte
    USART1->DR = buffer[i] & 0xFF;
	}
	
	// Check if completion is complete
	while(!(USART1->SR & USART_SR_TC)){}
}

void USART1_Read(uint8_t *buffer, uint16_t size)
{
	// For multiple bytes
	for(int i = 0; i < size; i++)
	{
		// Wait till data is received
		while(!(USART1->SR & USART_SR_RXNE)){}
		
		// Store the data	
		buffer[i] = (uint8_t) USART1->DR;
	}
}
