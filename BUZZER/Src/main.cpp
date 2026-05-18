#include "stm32f405xx.h"
#include<stdint.h>

uint32_t c=0;

void buzzer_init()
{
	RCC->AHB1ENR|=(1<<4);
	GPIOE->MODER&=~(3<<4);
	GPIOE->MODER|=(1<<4);
}
void delay(uint32_t d)
{
uint32_t i;
	for(i=0;i<d;i++);
}
void beep_low()
{
    GPIOE->BSRR = (1<<2);          // ON
    delay(500000);
    GPIOE->BSRR = (1<<(2+16));     // OFF
    delay(2000000);
}
void beep_medium()
{
    GPIOE->BSRR = (1<<2);
    delay(300000);
    GPIOE->BSRR = (1<<(2+16));
    delay(300000);
}
void beep_high()
{
    for(int i=0;i<10;i++)
    {
        GPIOE->BSRR = (1<<2);
        delay(100000);
        GPIOE->BSRR = (1<<(2+16));
        delay(100000);
    }
}
int main()
{
	buzzer_init();
	while(1)
	{
		c++;
	   if(c<10)
	   {
		   beep_low();
	   }
	   else if(c>=10 && c<30)
	   {
		   beep_medium();
	   }
	   else if(c>=30)
	   {
		   beep_high();
	   }

	}
}
