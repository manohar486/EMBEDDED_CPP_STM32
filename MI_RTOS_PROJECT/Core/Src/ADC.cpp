
#include<cstdint>
#include "ADC.h"
void ADC1_Init(void)
{
    // 1. Enable clocks
    RCC->AHB1ENR |= (1U << 2);   // GPIOC clock
    RCC->APB2ENR |= (1U << 8);   // ADC1 clock

    // 2. Configure PC0, PC1, PC2 as Analog (CH10,11,12)
    GPIOC->MODER &= ~(3U << (0 * 2));
    GPIOC->MODER |=  (3U << (0 * 2));  // PC0 → CH10

    GPIOC->MODER &= ~(3U << (1 * 2));
    GPIOC->MODER |=  (3U << (1 * 2));  // PC1 → CH11

    GPIOC->MODER &= ~(3U << (2 * 2));
    GPIOC->MODER |=  (3U << (2 * 2));  // PC2 → CH12

    // 3. ADC common config (PCLK2 / 4)
    ADC->CCR &= ~(3U << 16);
    ADC->CCR |=  (1U << 16);

    // 4. Disable scan & continuous mode
    ADC1->CR1 &= ~(1U << 8);   // SCAN = 0
    ADC1->CR2 &= ~(1U << 1);   // CONT = 0

    // 5. Right alignment (default)
    ADC1->CR2 &= ~(1U << 11);

    // 6. Sampling time (long = stable)
    ADC1->SMPR1 &= ~(7U << 0);   // CH10
    ADC1->SMPR1 |=  (7U << 0);

    ADC1->SMPR1 &= ~(7U << 3);   // CH11
    ADC1->SMPR1 |=  (7U << 3);

    ADC1->SMPR1 &= ~(7U << 6);   // CH12
    ADC1->SMPR1 |=  (7U << 6);

    // 7. Turn ON ADC
    ADC1->CR2 |= (1U << 0);
}

uint32_t ADC1_Read(uint32_t channel)
{
    // 1. Select channel (rank 1)
    ADC1->SQR3 = channel;

    // 2. Start conversion
    ADC1->CR2 |= (1U << 30);

    // 3. Wait until conversion complete (EOC)
    `while (!(ADC1->SR & (1U << 1)));

    for(int i=0;i<50000;i++);
    // 4. Read result
    return (uint16_t)ADC1->DR;
}










