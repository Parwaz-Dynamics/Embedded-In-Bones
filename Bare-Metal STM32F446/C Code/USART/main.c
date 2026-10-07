
#include "stm32f446xx.h"
#include "SystemClock_Config.h"

extern volatile uint32_t tickms;
volatile uint32_t tickms = 0;

void delay(uint32_t time);
void GPIOA_Init(void);

void USART1_Init(void);

// For Polling USART
void USART1_Write_Polling(uint8_t *buffer, uint16_t size);
void USART1_Read_Polling(uint8_t *buffer, uint16_t size);

// For Interrupt USART
#define USART1_Buffer_Size 15							// Receive Buffer Size
static volatile uint16_t counterRx = 0;		// Rx Index

static volatile uint8_t *TxBuf;
static volatile uint16_t TxIdx = 0;
static volatile uint16_t TxLen = 0;

void USART1_IRQHandler(void);

void USART_Write_IT(uint8_t *buffer, uint16_t len);		// To send data for Tx
void USART_Write_Interrupt(USART_TypeDef *USARTx ,uint8_t *buffer, uint16_t *Size);	// Tx Interrupt Management

void USART_Read_Interrupt(USART_TypeDef *USARTx ,uint8_t *buffer, uint16_t *Size);	// Rx Interrupt Management

// Data Buffers
static uint8_t TxData[10] = "Hello\r\n";
static uint8_t RxData[USART1_Buffer_Size];

int main(void)
{
	SystemClock_Config();
	SysTick_Config(180000);
	GPIOA_Init();
	USART1_Init();
	
	/* For Interrupt based USART */
	
	// Enable USART Receiver Not Empty Interrupt Event
	USART1->CR1 |= USART_CR1_RXNEIE;
	
	// Enable USART Transmitter Empty Interrupt Event
	USART1->CR1 &= ~USART_CR1_TXEIE;
	
	// Set Interrupt Priority
	NVIC_SetPriority(USART1_IRQn, 0); 		// Highiest Urgency
	NVIC_EnableIRQ(USART1_IRQn);					// Enable NVIC
	
	while(1)
	{
		delay(1000);
		GPIOA->ODR ^= 1<<5;
//		USART_Write_IT(TxData, 10);			// To send data through Interrupt
//		USART1_Write_Polling(TxData, 15);
//		USART1_Read(RxData, 5);
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
	GPIOA->MODER &= ~(2U << 10);
	GPIOA->MODER |= (01 << 10);
	
	// Enable USART1 on APB2
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

  // PA9 / PA10 -> alternate function mode (MODER = 0b10)
  GPIOA->MODER &= ~(0xFU << (2*9));
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


// Polling Based USART Write and Read
void USART1_Write_Polling(uint8_t *buffer, uint16_t size)
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

void USART1_Read_Polling(uint8_t *buffer, uint16_t size)
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

void USART1_IRQHandler(void)
{
	if(USART1->SR & USART_SR_RXNE)		// If Rx is not empty
		USART_Read_Interrupt(USART1, RxData, &counterRx);
	if(USART1->SR & USART_SR_TXE)			// If Tx is empty -> means ready to send data
		USART_Write_Interrupt(USART1, TxBuf, &TxIdx);
}

void USART_Write_IT(uint8_t *buffer, uint16_t len)
{
	TxBuf = buffer;
	TxLen = len;
	TxIdx = 0;
	
	// Enable USART Transmitter Empty Interrupt Event
	USART1->CR1 |= USART_CR1_TXEIE;
}

void USART_Write_Interrupt(USART_TypeDef *USARTx ,uint8_t *buffer, uint16_t *Idx)
{
		if (TxIdx < TxLen) {
        USARTx->DR = buffer[*Idx] & 0xFF;   // send current byte
        (*Idx)++;                            // advance AFTER sending
    } else {
        USARTx->CR1 &= ~USART_CR1_TXEIE;    // nothing left -> stop TXE interrupts
    }
}

void USART_Read_Interrupt(USART_TypeDef *USARTx ,uint8_t *buffer, uint16_t *Size)
{
//	Store the buffer index value to the Data Register of USART1
	buffer[*Size] = (uint8_t) USARTx->DR;
	(*Size)++;			// Increase the index
	if((*Size) >= USART1_Buffer_Size)		// If idex exceed the buffer size
	{
		*Size = 0;	// Jump back to index 0 -> Ring Buffer
	}
}
