/*
 * led.h
 *
 *  Created on: 27-Mar-2026
 *  Author: mirafra
 *
 *
 */

#ifndef LED_H
#define LED_H

#include <stdint.h>
#include "stm32f405xx.h"

class EnergyUnit
{
    uint32_t volt;
    uint32_t curr;
    uint32_t pow;

public:
    EnergyUnit(uint32_t v=0,uint32_t c=0,uint32_t p=0);

    uint32_t compute_power();
    void clear_power();
};

class StatusLED
{
    uint32_t led_g;
    uint32_t led_o;
    uint32_t led_r;
    uint32_t led_b;

public:
    StatusLED(uint32_t g,uint32_t o,uint32_t r,uint32_t b);

    void display(EnergyUnit& obj);

    void led_on(uint32_t pin);
    void led_off(uint32_t pin);
    void all_led_off();
};

#endif
