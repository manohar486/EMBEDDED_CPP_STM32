#include "lcd.h"


void LCD::SetOutput(GPIO_TypeDef *gpio,uint32_t pin)
{
    gpio->MODER &= ~(3 << (pin * 2));
    gpio->MODER |=  (1 << (pin * 2));   // Output mode
}

void LCD::SetInput(GPIO_TypeDef *gpio,uint32_t pin)
{
    gpio->MODER &= ~(3 << (pin * 2));   // Input mode
}

void LCD::SetBit(GPIO_TypeDef *gpio,uint32_t pin)
{
    gpio->BSRR = (1 << pin);
}

void LCD::ClrBit(GPIO_TypeDef *gpio,uint32_t pin)
{
    gpio->BSRR = (1 << (pin + 16));
}

void LCD::CplBit(GPIO_TypeDef *gpio,uint32_t pin)
{
    gpio->ODR ^= (1 << pin);
}

uint8_t LCD::GetStatus(GPIO_TypeDef *gpio,uint32_t pin)
{
    return (gpio->IDR & (1 << pin)) ? 1 : 0;
}

void LCD::SetPullup(GPIO_TypeDef *gpio,uint32_t pin)
{
    gpio->PUPDR &= ~(3 << (pin * 2));
    gpio->PUPDR |=  (1 << (pin * 2));   // Pull-up
}

void LCD::SetAltFunc(GPIO_TypeDef *gpio, uint32_t pin, uint8_t num)
{
    if(pin < 8)
    {
        gpio->AFR[0] |= (num << (pin * 4));
    }
    else
    {
        gpio->AFR[1] |= (num << ((pin - 8) * 4));
    }
}



void LCD::delayms(uint32_t dly)
{
    for(uint32_t i = 0; i < dly * 16000; i++);
}


void pulseEnable()
{
    GPIOA->BSRR = (1 << PIN_EN);
    for(volatile int i=0;i<1000;i++);
    GPIOA->BSRR = (1 << (PIN_EN + 16));
}

void send4bit(uint8_t data)
{
    // D4-D7 mapping
    (data & 0x01) ? (PORT_D4->BSRR = (1<<PIN_D4)) : (PORT_D4->BSRR = (1<<(PIN_D4+16)));
    (data & 0x02) ? (PORT_D5->BSRR = (1<<PIN_D5)) : (PORT_D5->BSRR = (1<<(PIN_D5+16)));
    (data & 0x04) ? (PORT_D6->BSRR = (1<<PIN_D6)) : (PORT_D6->BSRR = (1<<(PIN_D6+16)));
    (data & 0x08) ? (PORT_D7->BSRR = (1<<PIN_D7)) : (PORT_D7->BSRR = (1<<(PIN_D7+16)));

    pulseEnable();
}



LCD::LCD()
{
    // Enable GPIO clocks
    RCC->AHB1ENR |= (1 << 0); // GPIOA
    RCC->AHB1ENR |= (1 << 1); // GPIOB
}

void LCD::LCD_Init()
{
    // Configure pins as output
    SetOutput(PORT_RS, PIN_RS);
    SetOutput(PORT_EN, PIN_EN);
    SetOutput(PORT_D4, PIN_D4);
    SetOutput(PORT_D5, PIN_D5);
    SetOutput(PORT_D6, PIN_D6);
    SetOutput(PORT_D7, PIN_D7);

    delayms(20);

    // Initialization sequence (4-bit mode)
    send4bit(0x03);
    delayms(5);

    send4bit(0x03);
    delayms(1);

    send4bit(0x03);
    send4bit(0x02); // 4-bit mode

    // Function set: 2 lines, 5x8 font
    LCD_write("\x28");

    // Display ON, Cursor OFF
    LCD_write("\x0C");

    // Clear display
    LCD_write("\x01");
    delayms(2);

    // Entry mode
    LCD_write("\x06");
}

void LCD::LCD_write(const char *ch)
{
    uint8_t data = *ch;

    // RS = 1 (data)
    SetBit(PORT_RS, PIN_RS);

    send4bit(data >> 4);
    send4bit(data & 0x0F);

    delayms(2);
}

void LCD::LCD_write_string(const char *str)
{
    while(*str)
    {
        LCD_write(str++);
    }
}

void LCD::LCD_write_number(int num)
{
    char buffer[10];
    int i = 0;

    if(num == 0)
    {
        LCD_write("0");
        return;
    }

    if(num < 0)
    {
        LCD_write("-");
        num = -num;
    }

    while(num > 0)
    {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }

    while(i--)
    {
        char ch = buffer[i];
        LCD_write(&ch);
    }
}
