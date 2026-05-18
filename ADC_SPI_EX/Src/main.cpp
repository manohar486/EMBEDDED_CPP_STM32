#include "stm32f405xx.h"
#include <stdint.h>

uint32_t result;

void delay()
{
    for(int i=0;i<50000;i++);
}

void ADC_Init()
{
    RCC->AHB1ENR |= (1<<3); // GPIOD
    RCC->AHB1ENR |= (1<<2); // GPIOC
    RCC->APB2ENR |= (1<<8); // ADC1

    // PD12 → LED
    GPIOD->MODER &= ~(3U<<(12*2));
    GPIOD->MODER |=  (1U<<(12*2));

    // PC0, PC1 → Analog
    GPIOC->MODER &= ~(3U<<(2*0));
    GPIOC->MODER |= (3U<<(2*0));
    GPIOC->MODER &= ~(3U<<(2*1));
    GPIOC->MODER |= (3U<<(2*1));

    // ADC prescaler
    ADC->CCR &= ~(3<<16);

    //Clear CR1 and CR2
     ADC1->CR1 |=0;
     ADC1->CR2 |=0;

    // Scan mode
    ADC1->CR1 |= (1<<8);

    // Continuous mode
    ADC1->CR2 |= (1<<1);

    // Sampling time
    ADC1->SMPR1 |= (7<<0);  // ch10
    ADC1->SMPR1 |= (7<<3);  // ch11

    // Sequence length = 2
    ADC1->SQR1 |= (1<<20);

    // Sequence
    ADC1->SQR3 |= (10<<0);  // rank1
    ADC1->SQR3 |= (11<<5);  // rank2

    // Enable ADC
    ADC1->CR2 |= (1<<0);
}

int main()
{
    ADC_Init();

    // Start conversion
    ADC1->CR2 |= (1<<30);

    while(1)
    {
        while(!(ADC1->SR & (1<<1)));
        result = ADC1->DR;

        while(!(ADC1->SR & (1<<1)));
                result = ADC1->DR;

        if(result > 2000)
            GPIOD->ODR |= (1<<12);
        else
            GPIOD->ODR &= ~(1<<12);

        delay();
    }
}
