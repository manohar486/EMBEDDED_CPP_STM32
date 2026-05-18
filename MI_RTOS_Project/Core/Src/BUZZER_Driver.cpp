/*
 * BUZZER_Driver.cpp
 *
 *  Created on: 27-Apr-2026
 *      Author: mirafra
 */
#include"header.h"


void Buzzer_class::buzzer_init()
{
	RCC->AHB1ENR|=(1<<4);
	GPIOE->MODER&=~(3<<4);
	GPIOE->MODER|=(1<<4);
}
void Buzzer_class::delay(uint32_t d)
{
uint32_t i;
	for(i=0;i<d;i++);
}
void Buzzer_class::beep_low()
{
    GPIOE->BSRR = (1<<2);          // ON
    delay(500000);
    GPIOE->BSRR = (1<<(2+16));     // OFF
    delay(2000000);
}
void Buzzer_class::beep_medium()
{
    GPIOE->BSRR = (1<<2);
    delay(300000);
    GPIOE->BSRR = (1<<(2+16));
    delay(300000);
}
void Buzzer_class::beep_high()
{
    for(int i=0;i<10;i++)
    {
        GPIOE->BSRR = (1<<2);
        delay(100000);
        GPIOE->BSRR = (1<<(2+16));
        delay(100000);
    }
}









