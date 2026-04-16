#include "stm32f405xx.h"
#include <stdint.h>

// USART3_TX → PC10
// USART3_RX → PC11

volatile uint8_t read_Ch;

void USART3_1_init(void);
void USART3_write(uint8_t ch);
uint8_t USART1_read(void);
void delayMs(int);


/*----------------------------------------------------------
  MAIN
----------------------------------------------------------*/
int main(void)
{
    USART3_1_init();   // Initialize UART

   while (1)
    {
        USART3_write('A');// 1 byte
         delayMs(10);
        read_Ch=USART1_read();
        if (read_Ch == 'A')
                {
                    GPIOD->ODR ^= (1 << 12); // LED toggles = loopback success
                }
         delayMs(100);
   }
}

/*----------------------------------------------------------
  USART3 Initialization
----------------------------------------------------------*/
void USART3_1_init(void)
{
     RCC->APB1ENR|=(1<<18);//enabling clock for usart 3
     RCC->APB2ENR|=(1<<4);//enabling clock for usart 1
     RCC->AHB1ENR|=(1<<2)|(1<<0)|(1<<3);//enabling clock for GPIOA and GPIOC

     GPIOD->MODER&=~(3<<24);
     GPIOD->MODER|=(1<<24);

     GPIOC->MODER&=~(3<<20);
     GPIOC->MODER|=(2<<20);//setting alternate function for GPIOC pin 10;

     GPIOC->AFR[1]&=~(0xF<<8);
     GPIOC->AFR[1]|=(0x7<<8);//setting pc10 for TX

     GPIOA->MODER&=~(3<<20);
     GPIOA->MODER|=(2<<20);//setting alternate function for GPIOA pin 10;

     GPIOA->AFR[1]&=~(0xF<<8);
     GPIOA->AFR[1]|=(0x7<<8);//setting PA10 for  RX;

     USART3->BRR=0x0683;
     USART1->BRR=0x0683;
     USART3->CR1|=1<<2;
     USART1->CR1|=1<<2;
     USART3->CR1|=1<<3;
     USART1->CR1|=1<<3;
     USART3->CR1|=1<<13;
     USART1->CR1|=1<<13;


}

/*----------------------------------------------------------
  Write one byte
----------------------------------------------------------*/
void USART3_write(uint8_t ch) //ch ='A'
{
    // Wait until DR empty
    while (!(USART3->SR & (1 << 7))); // TXE =0 while(!0) - while loop(true)
    //TXE=1 Data in DR register will be given to shift register

    USART3->DR = ch; // new data will be loaded to DR

    // Wait until transmission complete
    while (!(USART3->SR & (1 << 6))); // TC=0 while(!0) while(T) waiting
}

///*----------------------------------------------------------
//  Read one byte (blocking)
//----------------------------------------------------------*/
uint8_t USART1_read(void)
{
   while (!(USART1->SR & (1 << 5))); // RXNE
   return USART1->DR;
}



/*----------------------------------------------------------
  Delay
----------------------------------------------------------*/
void delayMs(int n)
{
    for (; n > 0; n--)
        for (volatile int i = 0; i < 2000; i++);
        }
