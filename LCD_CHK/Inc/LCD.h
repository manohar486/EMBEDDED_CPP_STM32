/*
 * LCD.h
 *
 *  Created on: 28-Apr-2026
 *      Author: mirafra
 */
#ifndef LCD_H_
#define LCD_H_

#include <cstdint>

class LCD
{
private:
    void delay_ms(volatile int t);
    void LCD_RS(int state);
    void LCD_EN();
    void lcd_send(uint8_t data);

public:
    void lcd_contrl();
    void LCD_SendByte(int data,int isdata);
    void LCD_Init();
    void set_cursor(int row,int col);
    void write_string(const char* str);
    void write_num(uint32_t num);
};

#endif
