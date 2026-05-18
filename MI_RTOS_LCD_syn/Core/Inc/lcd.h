
#ifndef LCD_H
#define LCD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

#ifdef __cplusplus
}
#endif

#include "stm32f4xx_hal_conf.h"   // Required for GPIO_TypeDef
#include <stdint.h>      // Required for uint32_t, uint8_t
#define PORT_RS		GPIOA
#define PIN_RS		0

#define PORT_EN		GPIOA
#define PIN_EN		1

#define PORT_D4		GPIOB
#define PIN_D4		12

#define PORT_D5		GPIOB
#define PIN_D5		13

#define PORT_D6		GPIOB
#define PIN_D6		14

#define PORT_D7		GPIOB
#define PIN_D7		15


class LCD
{
	void SetOutput(GPIO_TypeDef *gpio,uint32_t pin);
	void SetInput(GPIO_TypeDef *gpio,uint32_t pin);
	void SetBit(GPIO_TypeDef *gpio,uint32_t pin);
	void SetPullup(GPIO_TypeDef *gpio,uint32_t pin);
	void ClrBit(GPIO_TypeDef *gpio,uint32_t pin);
	void CplBit(GPIO_TypeDef *gpio,uint32_t pin);
	uint8_t GetStatus(GPIO_TypeDef *gpio,uint32_t pin);
	void SetAltFunc(GPIO_TypeDef *gpio, uint32_t pin, uint8_t num);
	void delayms(uint32_t dly);

public:
	LCD();
    void LCD_Init();
    void LCD_write(const char *ch);
    void LCD_write_string(const char *str);
    void LCD_write_number(int num);
};

#endif
