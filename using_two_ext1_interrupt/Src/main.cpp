#include <stdint.h>
#include "stm32f405xx.h"

// NVIC
inline volatile uint32_t *NVIC_ISER0 = (uint32_t*)0xE000E100;
inline volatile uint32_t *NVIC_ICPR0 = (uint32_t*)0xE000E280;


void LED_Init(void)
{
    RCC->AHB1ENR |= (1 << 3);   // GPIOD clock

    // PD12 → LED1
    GPIOD->MODER &= ~(3 << (12 * 2));
    GPIOD->MODER |=  (1 << (12 * 2));

    // PD13 → LED2
    GPIOD->MODER &= ~(3 << (13 * 2));
    GPIOD->MODER |=  (1 << (13 * 2));

    GPIOD->MODER &= ~(3 << (14 * 2));
     GPIOD->MODER |=  (1 << (14 * 2));

    GPIOD->ODR &= ~(1 << 12);
    GPIOD->ODR &= ~(1 << 13);
    GPIOD->ODR &= ~(1 << 14);
}


void Button_EXTI1_Init(void)
{
    RCC->AHB1ENR |= (1 << 3);   // GPIOD
    RCC->APB2ENR |= (1 << 14);  // SYSCFG

    // PD1 input
    GPIOD->MODER &= ~(3 << (1 * 2));

    // Pull-down
    GPIOD->PUPDR &= ~(3 << (1 * 2));
    GPIOD->PUPDR |=  (2 << (1 * 2));

    // Map EXTI1 → PD1
    SYSCFG->EXTICR[0] &= ~(0xF << 4);
    SYSCFG->EXTICR[0] |=  (3 << 4);   // Port D pd1

    EXTI->RTSR |= (1 << 1);   // Rising edge
    EXTI->IMR  |= (1 << 1);   // Unmask

    *NVIC_ISER0 |= (1 << 7);  // IRQ7 (EXTI1)
}


void Button_EXTI2_Init(void)
{
    RCC->AHB1ENR |= (1 << 3);   // GPIOD
    RCC->APB2ENR |= (1 << 14);  // SYSCFG

    // PD2 input
    GPIOD->MODER &= ~(3 << (2 * 2));

    // Pull-down
    GPIOD->PUPDR &= ~(3 << (2 * 2));
    GPIOD->PUPDR |=  (2 << (2 * 2));

    // Map EXTI2 → PD2
    SYSCFG->EXTICR[0] &= ~(0xF << 8);
    SYSCFG->EXTICR[0] |=  (3 << 8);   // Port D pd2

    EXTI->RTSR |= (1 << 2);
    EXTI->IMR  |= (1 << 2);

    *NVIC_ISER0 |= (1 << 8);  // IRQ8 (EXTI2)
}


void TIM2_Init(void)
{
    RCC->APB1ENR |= (1 << 0);

    TIM2->PSC = 1599;
    TIM2->ARR = 20000 - 1;

    TIM2->CNT = 0;

    TIM2->EGR |= (1 << 0);
    TIM2->SR  &= ~(1 << 0);

    TIM2->DIER |= (1 << 0);

    *NVIC_ICPR0 |= (1 << 28);
    *NVIC_ISER0 |= (1 << 28);

    TIM2->CR1 |= (1 << 0);
}



// EXTI1 and LED1
extern "C" void EXTI1_IRQHandler(void)
{
    if (EXTI->PR & (1 << 1))
    {
        GPIOD->ODR ^= (1 << 12);   // LED1 toggle
        EXTI->PR |= (1 << 1);      // clear
    }
}

// EXTI2 and LED2
extern "C" void EXTI2_IRQHandler(void)
{
    if (EXTI->PR & (1 << 2))
    {
        GPIOD->ODR ^= (1 << 13);   // LED2 toggle
        EXTI->PR |= (1 << 2);
    }
}

// TIMER  both LEDs
extern "C" void TIM2_IRQHandler(void)
{
    if (TIM2->SR & (1 << 0))
    {
        TIM2->SR &= ~(1 << 0);

        GPIOD->ODR ^= (1 << 14);

    }
}


int main(void)
{
    LED_Init();
    Button_EXTI1_Init();
    Button_EXTI2_Init();
    TIM2_Init();

    while (1)
    {
    }
}
