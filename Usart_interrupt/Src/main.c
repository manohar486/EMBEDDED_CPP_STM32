#include <stdint.h>
#include "stm32f405xx.h"
void USART3_init(void);
void USART3_string(char *ch);
uint8_t USART3_read(void);
void USART3_write(uint8_t ch);

// NVIC
inline volatile uint32_t *NVIC_ISER = (uint32_t*)0xE000E100;
inline volatile uint32_t *NVIC_ICPR = (uint32_t*)0xE000E280;


volatile uint8_t rx_data = 0;
volatile uint8_t rx_flag = 0;


void TIM2_init(void);
void delaySec(uint32_t sec);

void USART3_init(void);
void USART3_write(uint8_t ch);
uint8_t USART3_read(void);
void USART3_string(char *ch);


 void USART3_IRQHandler(void)
{
    if (USART3->SR & (1 << 5))   // RXNE
    {
        rx_data = USART3->DR;   // Read data (clears RXNE)
        rx_flag = 1;            // Notify main loop
    }
}


int main(void)
{
    USART3_init();
    TIM2_init();

    while (1)
    {
        if (rx_flag)
        {
            rx_flag = 0;

            USART3_write(rx_data);              // Echo received data
            USART3_string(" RXNE Interrupt\r\n");

            delaySec(2);
    }
}

}



void USART3_init(void)
{
    RCC->AHB1ENR |= (1 << 2);   // GPIOC clock
    RCC->APB1ENR |= (1 << 18);  // USART3 clock

    // PC10 → TX
    GPIOC->MODER &= ~(3 << 20);
    GPIOC->MODER |=  (1 << 21);
    GPIOC->AFR[1] &= ~(0xF << 8);
    GPIOC->AFR[1] |=  (7 << 8);

    // PC11 → RX
    GPIOC->MODER &= ~(3 << 22);
    GPIOC->MODER |=  (1 << 23);
    GPIOC->AFR[1] &= ~(0xF << 12);
    GPIOC->AFR[1] |=  (7 << 12);

    // USART config
    USART3->BRR = 0x0683;
    USART3->CR1 |= (1 << 3);   // TE
    USART3->CR1 |= (1 << 2);   // RE
    USART3->CR1 |= (1 << 13);  // UE

    USART3->CR1 |= (1 << 5);   // RXNE interrupt enable

    // NVIC config (USART3 IRQ = 39 → ISER[1], bit 7)
    NVIC_ICPR[1] |= (1 << 7);
    NVIC_ISER[1] |= (1 << 7);
}


void USART3_write(uint8_t ch)
{
    while (!(USART3->SR & (1 << 7))); // TXE
    USART3->DR = ch;
    while (!(USART3->SR & (1 << 6))); // TC
}


uint8_t USART3_read(void)
{
    while (!(USART3->SR & (1 << 5))); // RXNE
    return USART3->DR;
}


void USART3_string(char *ch)
{
    for (int i = 0; ch[i] != '\0'; i++)
    {
        USART3_write(ch[i]);
    }
}

void TIM2_init(void)
{
    RCC->APB1ENR |= (1 << 0);   // Enable TIM2 clock

    TIM2->PSC = 16000 - 1;      // Prescaler (16MHz / 16000 = 1kHz)
    TIM2->ARR = 1000 - 1;       // 1 second

    TIM2->CNT = 0;
    TIM2->CR1 |= (1 << 0);      // Enable timer
}


void delaySec(uint32_t sec)
{
    for (uint32_t i = 0; i < sec; i++)
    {
        TIM2->CNT = 0;

        while (!(TIM2->SR & (1 << 0))); // Wait for update event
        TIM2->SR &= ~(1 << 0);          // Clear flag
    }
}
