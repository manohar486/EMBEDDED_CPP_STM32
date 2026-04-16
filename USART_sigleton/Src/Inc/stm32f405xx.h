#include <cstdint>

// ================= REGISTER DEFINITIONS =================

struct RCC_RegDef_t
{
    volatile uint32_t CR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t AHB3RSTR;
    uint32_t RESERVED0;
    volatile uint32_t APB1RSTR;
    volatile uint32_t APB2RSTR;
    uint32_t RESERVED1[2];
    volatile uint32_t AHB1ENR;
};

struct GPIO_RegDef_t
{
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
};

struct USART_RegDef_t
{
    volatile uint32_t SR;
    volatile uint32_t DR;
};

// ================= BASE ADDRESSES =================

#define RCC_BASE     0x40023800
#define GPIOD_BASE   0x40020C00
#define USART3_BASE  0x40004800

#define RCC     ((RCC_RegDef_t*)RCC_BASE)
#define GPIOD   ((GPIO_RegDef_t*)GPIOD_BASE)
#define USART3  ((USART_RegDef_t*)USART3_BASE)


// ================= USART SINGLETON =================

class USART
{
private:
    USART() {}   // private

public:
    static USART& instance()
    {
        static USART inst;
        return inst;
    }

    USART(const USART&) = delete;
    USART& operator=(const USART&) = delete;

    void write_char(char c)
    {
        while (!(USART3->SR & (1 << 7))); // TXE
        USART3->DR = c;
    }

    void write_string(const char* str)
    {
        while (*str)
        {
            write_char(*str++);
        }
    }
};


// ================= GPIO INIT =================

void gpio_init()
{
    RCC->AHB1ENR |= (1 << 3);     // Enable GPIOD clock

    GPIOD->MODER &= ~(3 << (12 * 2));
    GPIOD->MODER |=  (1 << (12 * 2));  // PD12 as output
}


// ================= DELAY =================

void delay()
{
    for (volatile int i = 0; i < 500000; i++);
}


// ================= MAIN =================

int main()
{
    auto& uart = USART::instance();

    gpio_init();

    uart.write_string("System Started\n");

    while (1)
    {
        GPIOD->ODR ^= (1 << 12);   // Toggle LED

        uart.write_string("LED TOGGLED\n");

        delay();
    }
}
