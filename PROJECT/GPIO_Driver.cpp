/*
 * GPIO_Driver.cpp
 *
 *  Created on: 23-Apr-2026
 *      Author: mirafra
 */
#include "header.h"

	GPIO_class::GPIO_class(uint32_t gpiopin)
	{
		pin=gpiopin;
	}
	void GPIO_class::GPIO_init()
	{
		RCC->AHB1ENR |= (1<<3);  // GPIOD

		GPIOD->MODER &=~ (3 <<(pin*2));  //CLEAR BIT
		GPIOD->MODER |= (1<<(pin*2));   //OUTPUT MODE
	}

	void GPIO_class::LED_On()
	{
		GPIOD->BSRR = (1<<pin);
	}
	void GPIO_class::LED_Off()
	{
		GPIOD->BSRR = (1<<(pin+16));
	}
	void GPIO_class::LED_Toggle()
	{
		GPIOD->ODR ^= (1<<pin);   //TOGGLE LED
	}













