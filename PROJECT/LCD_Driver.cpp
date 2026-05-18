/*
 * LCD_Driver.cpp
 *
 *  Created on: 24-Apr-2026
 *      Author: mirafra
 */

#include "header.h"

    void LCD_class::GPIO_Init()
	{
		RCC->AHB1ENR|=(1<<1);

		GPIOB->MODER&=~(3<<(2*2));//b2
		GPIOB->MODER&=~(3<<(3*2));//b3
		GPIOB->MODER&=~(3<<(4*2));//b4
		GPIOB->MODER&=~(3<<(5*2));//b5
		GPIOB->MODER&=~(3<<(6*2));///rs//pb6
		GPIOB->MODER&=~(3<<(7*2));//rw//pb7
		GPIOB->MODER&=~(3<<(8*2));//en//pb8

	    GPIOB->MODER|=(1<<(2*2));
	    GPIOB->MODER|=(1<<(3*2));
		GPIOB->MODER|=(1<<(4*2));
		GPIOB->MODER|=(1<<(5*2));
		GPIOB->MODER|=(1<<(6*2));
		GPIOB->MODER|=(1<<(7*2));
		GPIOB->MODER|=(1<<(8*2));

	}
    void LCD_class::delay(void)
    {
        HAL_Delay(2);
    }
	void LCD_class::LCD_write(uint8_t val)
	{
		GPIOB->ODR = (GPIOB->ODR & ~(0xF << 1)) | ((val) << 2);
        GPIOB->ODR|=1<<8;
        delay();
        GPIOB->ODR&=~(1<<8);
        delay();

	}
	void LCD_class::LCD_cmd(uint8_t val)
	{
		GPIOB->ODR&=~(1<<6);
		LCD_write(val>>4);
		LCD_write(val&0x0f);
	}
	void LCD_class::LCD_data(uint8_t val)
	{
		GPIOB->ODR|=(1<<6);
		LCD_write(val>>4);
		LCD_write(val&0x0f);
	}
	void LCD_class::LCD_Init()
	{
		delay();
		LCD_cmd(0x33);
		LCD_cmd(0x32);
		LCD_cmd(0x28);
		LCD_cmd(0x0C);
		LCD_cmd(0x01);
		delay();
	}
	LCD_class::LCD_class()
	  {
		  GPIO_Init();
		  LCD_Init();
	  };
	  void LCD_class::write_string(const char* str)
	  {
		  while(*str)
		  {
			  LCD_data(*str);
			  str++;
		  }
	  }

	  void LCD_class::LCD_clear()
	  {
		  LCD_cmd(0x01);
		  delay();
	  }










