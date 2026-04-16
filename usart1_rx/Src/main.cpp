#include "stm32f405xx.h"
#include <stdint.h>



uint8_t read_Ch;

void USART1_init();
void LED_init();
void USART1_write(uint8_t ch);
uint8_t USART1_read();
void delayMs(int);

int main()
{
    USART1_init();   // initialize UART

   while (1)
    {
	   read_Ch=USART1_read();

         if(read_Ch=='1')
         {
        	  GPIOB->BSRR = (1 << 4);
        	  delayMs(50000);
        	  GPIOB->BSRR = (1 << (4+16));


         }
         else if(read_Ch=='2')
         {
        	 GPIOB->BSRR = (1 << 8);
        	 delayMs(50000);
        	  GPIOB->BSRR = (1 << (8+16));

         }
         else if(read_Ch=='3')
         {
        	 GPIOE->BSRR = (1 << 2);
        	 delayMs(50000);
        	    GPIOE->BSRR = (1 << (2+16));
         }
         else if(read_Ch=='4')
         {
        	 GPIOE->BSRR = (1 << 5);
        	 delayMs(50000);
        	  GPIOE->BSRR = (1 << (5+16));
         }
         else
         {
        	 GPIOB->BSRR = (1 << 4);
        	 GPIOB->BSRR = (1 << 8);
        	 GPIOE->BSRR = (1 << 2);
        	 GPIOE->BSRR = (1 << 5);

        	 delayMs(50000);

        	 GPIOB->BSRR = (1 << (4+16));
        	  GPIOB->BSRR = (1 << (8+16));
        	  GPIOE->BSRR = (1 << (2+16));
        	  GPIOE->BSRR = (1 << (5+16));


         }

   }
}

void USART1_init()
{
    RCC->AHB1ENR |= (1 << 1);   // enable GPIOB clock
    RCC->APB2ENR |= (1 << 4);  // enable USART1 clock

    // Pb6 → TX
    GPIOB->MODER &= ~(3 << 12); // pin6 =pin*2, claered the bits
    GPIOB->MODER |=  (1 << 13);   // Alternate function -10
    // created Array :AFR[2]  , AFR[0]=AFRL ,AFR[1]= AFRH
    GPIOB->AFR[0] &= ~(0xF << 24); // AFRH =AFR[1]- Clearing 4 bits
    GPIOB->AFR[0] |=  (7 << 24);   // AF7 (USART3) // 0111 for AF7

    // Pb7 → RX
    GPIOB->MODER &= ~(3 << 14);
    GPIOB->MODER |=  (1 << 15);// 10 AFR mode

    GPIOB->AFR[0] &= ~(0xF << 28);// AFRH Register
    GPIOB->AFR[0] |=  (7 << 28); // AF7 -0111

    // USART settings
    USART1->BRR = 0x0683;      // 9600 baud @ 16 MHz(HSI)
   // USART1->CR1 |= (1 << 3);   // TE
    USART1->CR1 |= (1 << 2);   // RE
    USART1->CR1 |= (1 << 13);  // UE - USART enable
}


uint8_t USART1_read()
{
    while (!(USART1->SR & (1 << 5))); // RXNE
    return USART1->DR;
}

void LED_init()
{
    RCC->AHB1ENR |= (1 << 1);
    RCC->AHB1ENR |= (1 << 4);

    GPIOB->MODER &= ~(3 << (4 * 2));
    GPIOB->MODER &= ~(3 << (7 * 2));

    GPIOE->MODER &= ~(3 << (2 * 2));
    GPIOE->MODER &= ~(3 << (5 * 2));

}



void delayMs(int n)
{
    for (; n > 0; n--)
        for (volatile int i = 0; i < 2000; i++);
}
