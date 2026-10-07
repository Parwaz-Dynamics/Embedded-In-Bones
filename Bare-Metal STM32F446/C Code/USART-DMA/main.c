#include "stm32f446xx.h"
#include "SystemClock_Config.h"

/* ========================================================================
 *  USART1 + DMA2 (circular RX) + IDLE-line framing + ORE recovery
 *  STM32F446RET6  @ 180 MHz  (PCLK2 = 90 MHz)
 *
 *  Pins:   PA9  = USART1_TX (AF7)
 *          PA10 = USART1_RX (AF7)
 *          PA5  = onboard LED (Nucleo)
 *
 *  DMA mapping (fixed in hardware):
 *          USART1_TX -> DMA2 Stream7, Channel4
 *          USART1_RX -> DMA2 Stream2, Channel4  (circular)
 * ======================================================================== */

/* ---------------- Tick / delay ---------------- */
volatile uint32_t tickms = 0;

void SysTick_Handler(void);
void delay(uint32_t ms);

/* ---------------- Init prototypes ---------------- */
void GPIOA_Init(void);
void USART1_Init(void);
void DMA2_Init(void);

/* ---------------- USART / DMA transfer prototypes ---------------- */
void USART1_DMA_Write(uint8_t *buffer, uint16_t size);
void USART1_DMA_Read_Start(uint8_t *buffer, uint16_t size);

/* ---------------- Buffers ---------------- */
#define RX_BUF_SIZE  16              /* must equal the NDTR armed for RX */

static uint8_t  TxData[] = "Hello\r\n";
static uint8_t  RxData[RX_BUF_SIZE];  /* DMA circular ring buffer         */

/* Received-packet state (filled by the IDLE handler, read in main) */
static volatile uint16_t rx_tail       = 0;   /* last ring index consumed  */
static volatile uint8_t  rx_packet[RX_BUF_SIZE];
static volatile uint16_t rx_packet_len = 0;
static volatile uint8_t  rx_ready      = 0;   /* 1 = a burst is waiting    */

/* ======================================================================== */
/*  main                                                                    */
/* ======================================================================== */
int main(void)
{
    SystemClock_Config();          /* 180 MHz, PCLK2 = 90 MHz */
    SysTick_Config(180000);        /* 1 ms tick @ 180 MHz     */

    GPIOA_Init();
    USART1_Init();                 /* also sets IDLEIE + DMAT/DMAR */
    DMA2_Init();

    USART1_DMA_Read_Start(RxData, RX_BUF_SIZE);   /* arm circular RX once */

    /* NVIC */
    NVIC_EnableIRQ(USART1_IRQn);        /* IDLE + ORE          */
    NVIC_EnableIRQ(DMA2_Stream7_IRQn);  /* TX transfer complete */
    NVIC_EnableIRQ(DMA2_Stream2_IRQn);  /* RX wrap (optional)   */

    /* Greeting so you can confirm TX works on power-up */
    USART1_DMA_Write(TxData, sizeof(TxData) - 1);   /* -1 skips the '\0' */

    while (1)
    {
        /* When a burst has arrived (line went idle), echo it back */
        if (rx_ready)
        {
            rx_ready = 0;
            USART1_DMA_Write((uint8_t *)rx_packet, rx_packet_len);
        }

        /* Heartbeat LED */
        delay(500);
        GPIOA->ODR ^= (1 << 5);
    }
}

/* ======================================================================== */
/*  SysTick / delay                                                         */
/* ======================================================================== */
void SysTick_Handler(void)
{
    tickms++;
}

void delay(uint32_t ms)
{
    uint32_t start = tickms;
    while (tickms - start < ms) {}
}

/* ======================================================================== */
/*  GPIOA: PA5 LED, PA9/PA10 USART1 AF7                                     */
/* ======================================================================== */
void GPIOA_Init(void)
{
    /* Clock for Port A */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* PA5 -> general purpose output (LED) */
    GPIOA->MODER &= ~(0x3U << (2 * 5));
    GPIOA->MODER |=  (0x1U << (2 * 5));

    /* PA9 / PA10 -> alternate function mode (0b10 each) */
    GPIOA->MODER &= ~(0xFU << (2 * 9));
    GPIOA->MODER |=  (0xAU << (2 * 9));

    /* Very high speed on the TX pin region */
    GPIOA->OSPEEDR |= (0x3U << (2 * 9));

    /* AF7 (USART1) for PA9 and PA10. Pins 8..15 use AFR[1] (AFRH). */
    GPIOA->AFR[1] &= ~((0xFU << ((9 - 8) * 4)) | (0xFU << ((10 - 8) * 4)));
    GPIOA->AFR[1] |=  ((7U   << ((9 - 8) * 4)) | (7U   << ((10 - 8) * 4)));
}

/* ======================================================================== */
/*  USART1: 9600 8N1, oversample 16, IDLE interrupt, DMA requests           */
/* ======================================================================== */
void USART1_Init(void)
{
    /* Clock for USART1 (APB2) */
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    /* Configure while the USART is disabled */
    USART1->CR1 &= ~USART_CR1_UE;

    USART1->CR1 &= ~USART_CR1_M;        /* 8 data bits        */
    USART1->CR2 &= ~USART_CR2_STOP;     /* 1 stop bit         */
    USART1->CR1 &= ~USART_CR1_PCE;      /* no parity          */
    USART1->CR1 &= ~USART_CR1_OVER8;    /* oversampling by 16 */

    /* Baud: BRR = PCLK2 / baud = 90e6 / 9600 = 9375 = 0x249F */
    USART1->BRR = 0x249F;

    /* Enable transmitter + receiver */
    USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;

    /* IDLE-line interrupt: fires when RX goes quiet after a burst.
     * NOTE: do NOT enable RXNEIE -- DMA drains DR, the CPU must not. */
    USART1->CR1 |= USART_CR1_IDLEIE;

    /* USART issues DMA requests */
    USART1->CR3 |= USART_CR3_DMAT;      /* on TXE (transmit) */
    USART1->CR3 |= USART_CR3_DMAR;      /* on RXNE (receive) */

    /* Enable the USART */
    USART1->CR1 |= USART_CR1_UE;
}

/* ======================================================================== */
/*  DMA2 init: TX = Stream7 Ch4 (normal), RX = Stream2 Ch4 (circular)       */
/* ======================================================================== */
void DMA2_Init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

    /* ---------- TX: DMA2 Stream7, Channel4, memory -> peripheral ---------- */
    DMA2_Stream7->CR &= ~DMA_SxCR_EN;
    while (DMA2_Stream7->CR & DMA_SxCR_EN) {}

    DMA2_Stream7->PAR = (uint32_t)&USART1->DR;
    DMA2_Stream7->CR =
          (4U << DMA_SxCR_CHSEL_Pos)   /* channel 4                     */
        | DMA_SxCR_MINC                /* increment memory address      */
        | DMA_SxCR_DIR_0               /* direction = mem -> periph (01) */
        | DMA_SxCR_TCIE;               /* transfer-complete interrupt   */
    /* PSIZE/MSIZE = byte, PINC = 0 (DR fixed) -> defaults, fine */

    /* ---------- RX: DMA2 Stream2, Channel4, circular, periph -> memory ---- */
    DMA2_Stream2->CR &= ~DMA_SxCR_EN;
    while (DMA2_Stream2->CR & DMA_SxCR_EN) {}

    DMA2_Stream2->PAR = (uint32_t)&USART1->DR;
    DMA2_Stream2->CR =
          (4U << DMA_SxCR_CHSEL_Pos)   /* channel 4                     */
        | DMA_SxCR_MINC                /* increment memory address      */
        | DMA_SxCR_CIRC                /* circular: auto-reload + wrap   */
        | DMA_SxCR_TCIE;               /* DIR = 00 -> periph -> memory   */
}

/* ======================================================================== */
/*  TX: start a one-shot DMA transmit                                       */
/* ======================================================================== */
void USART1_DMA_Write(uint8_t *buffer, uint16_t size)
{
    DMA2_Stream7->CR &= ~DMA_SxCR_EN;
    while (DMA2_Stream7->CR & DMA_SxCR_EN) {}

    /* Clear Stream7 flags (streams 4-7 -> HIGH flag register) */
    DMA2->HIFCR = DMA_HIFCR_CTCIF7 | DMA_HIFCR_CHTIF7
                | DMA_HIFCR_CTEIF7 | DMA_HIFCR_CDMEIF7 | DMA_HIFCR_CFEIF7;

    DMA2_Stream7->M0AR = (uint32_t)buffer;
    DMA2_Stream7->NDTR = size;
    DMA2_Stream7->CR  |= DMA_SxCR_EN;
}

/* ======================================================================== */
/*  RX: arm the circular receive ONCE (runs forever after this)            */
/* ======================================================================== */
void USART1_DMA_Read_Start(uint8_t *buffer, uint16_t size)
{
    DMA2_Stream2->CR &= ~DMA_SxCR_EN;
    while (DMA2_Stream2->CR & DMA_SxCR_EN) {}

    /* Clear Stream2 flags (streams 0-3 -> LOW flag register) */
    DMA2->LIFCR = DMA_LIFCR_CTCIF2 | DMA_LIFCR_CHTIF2
                | DMA_LIFCR_CTEIF2 | DMA_LIFCR_CDMEIF2 | DMA_LIFCR_CFEIF2;

    DMA2_Stream2->M0AR = (uint32_t)buffer;
    DMA2_Stream2->NDTR = size;
    DMA2_Stream2->CR  |= DMA_SxCR_EN;

    rx_tail = 0;
}

/* ======================================================================== */
/*  USART1 IRQ: IDLE-line framing + ORE recovery                            */
/* ======================================================================== */
void USART1_IRQHandler(void)
{
    uint32_t sr = USART1->SR;        /* read SR first (clear sequence step 1) */

    /* ---- Overrun recovery ---- */
    if (sr & USART_SR_ORE) {
        (void)USART1->DR;            /* SR-then-DR clears ORE; lost byte gone */
    }

    /* ---- IDLE: a burst just finished ---- */
    if (sr & USART_SR_IDLE) {
        (void)USART1->DR;            /* SR already read -> this clears IDLE   */

        /* How far has DMA written? head = size - NDTR */
        uint16_t head = RX_BUF_SIZE - (uint16_t)DMA2_Stream2->NDTR;

        /* Copy tail..head out of the ring (handles wrap) */
        uint16_t n = 0;
        while (rx_tail != head) {
            rx_packet[n++] = RxData[rx_tail];
            rx_tail = (rx_tail + 1) % RX_BUF_SIZE;
        }
        rx_packet_len = n;
        rx_ready = 1;
    }
}

/* ======================================================================== */
/*  DMA2 Stream7 IRQ: TX transfer complete                                  */
/* ======================================================================== */
void DMA2_Stream7_IRQHandler(void)
{
    if (DMA2->HISR & DMA_HISR_TCIF7) {
        DMA2->HIFCR = DMA_HIFCR_CTCIF7;   /* clear TX TC flag */
        /* DMA done; last byte may still be shifting out.
         * If you must guarantee the line is idle, wait here:
         *   while (!(USART1->SR & USART_SR_TC)) {}
         */
    }
}

/* ======================================================================== */
/*  DMA2 Stream2 IRQ: RX ring wrapped (fires once per full buffer)          */
/* ======================================================================== */
void DMA2_Stream2_IRQHandler(void)
{
    if (DMA2->LISR & DMA_LISR_TCIF2) {
        DMA2->LIFCR = DMA_LIFCR_CTCIF2;   /* clear RX TC flag; ring continues */
    }
}