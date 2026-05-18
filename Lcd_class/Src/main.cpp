#include<stm32f405xx.h>

class LCD
{
	void GPIO_Init()
	{
		RCC->AHB1ENR|=(1<<0);
		GPIOA->MODER&=~(3<<(1*2));
		GPIOA->MODER&=~(3<<(2*2));
		GPIOA->MODER&=~(3<<(3*2));
		GPIOA->MODER&=~(3<<(4*2));
		GPIOA->MODER&=~(3<<(5*2));//rs//pa5
		GPIOA->MODER&=~(3<<(6*2));//r/w//pa6
		GPIOA->MODER&=~(3<<(7*2));//en//pa7

		GPIOA->MODER|=(1<<(1*2));
	    GPIOA->MODER|=(1<<(2*2));
	    GPIOA->MODER|=(1<<(3*2));
		GPIOA->MODER|=(1<<(4*2));
		GPIOA->MODER|=(1<<(5*2));
		GPIOA->MODER|=(1<<(6*2));
		GPIOA->MODER|=(1<<(7*2));

	}
	static void delay(void)
	{
		for(volatile int i=0;i<3000;i++)
		{

		}
	}
	void LCD_write(uint8_t val)
	{
		GPIOA->ODR &= ~(0xF << 1);
		GPIOA->ODR = ((val) << 1);
        GPIOA->ODR|=1<<7;
        delay();
        GPIOA->ODR&=~(1<<7);
        delay();

	}
	void LCD_cmd(uint8_t val)
	{
		GPIOA->ODR&=~(1<<5);
		LCD_write(val>>4);
		LCD_write(val&0x0f);
	}
	void LCD_data(uint8_t val)
	{
		GPIOA->ODR|=(1<<5);
		LCD_write(val>>4);
		LCD_write(val&0x0f);
	}
	void LCD_Init()
	{
		delay();
		LCD_cmd(0x33);
		LCD_cmd(0x30);
		LCD_cmd(0x28);
		LCD_cmd(0x0C);
		LCD_cmd(0x01);
		delay();
	}
public:
	  LCD()
	  {
		  GPIO_Init();
		  LCD_Init();
	  };
	  void write_string(const char* str)
	  {
		  while(*str)
		  {
			  LCD_data(*str);
			  str++;
		  }
	  }
};
int main()
{
	LCD lcd1;
	///while(1)
	lcd1.write_string("MANOHAR");
}
