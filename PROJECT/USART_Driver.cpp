/*
 * USART_Driver.cpp
 *
 *  Created on: 23-Apr-2026
 *      Author: mirafra
 */
#include"header.h"
void USART_class::USART2_init()
{
	RCC->APB1ENR |= 1<<17;
	RCC->AHB1ENR |= 1<<3;   //gpioD

	//TX,PD5
	GPIOD->MODER &=~ (3<<10);
	GPIOD->MODER |= 2<<10;
	GPIOD->AFR[0] &=~ (0XF<<20);
	GPIOD->AFR[0] |= 7<<20;

	//RX ,PD6
	GPIOD->MODER &=~ (3<<12);
	GPIOD->MODER |= 2<<12;
	GPIOD->AFR[0] &=~ (0XF<<24);
	GPIOD->AFR[0] |= 7<<24;

	USART2->BRR = 16000000 / 9600;
	USART2->CR1 |=1<<13;
	USART2->CR1 |=1<<2;
	USART2->CR1 |=1<<3;
}


void USART_class::USART2_write(uint8_t ch)
{
	while(!(USART2->SR & (1<<7)));
	USART2->DR =ch;
	while(!(USART2->SR & (1<<6)));
}


uint8_t USART_class::USART2_read()
{
	while(!(USART2->SR & (1<<5)));
	return USART2->DR;
}


void USART_class::USART2_string(const char* str)
{
    while(*str)
    {
        while(!(USART2->SR & (1<<7)));  // Wait until TXE = 1
        USART2->DR = (uint8_t)(*str);   // Write data
        str++;
    }
    while(!(USART2->SR & (1<<6)));      // Wait until TC = 1 for last byte
}



















