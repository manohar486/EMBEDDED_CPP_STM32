#include "all_sensor.h"
#include "ADC.h"   //Only here

#define TEMP_CH   10
#define HR_CH     11
#define SPO2_CH   12

// -------- TEMP --------
float TemperatureSensor::read() {
    uint16_t adc = ADC1_Read((uint32_t)TEMP_CH);
    return (adc * 3.3f / 4095.0f) * 100;
}


// -------- HR --------
float HeartRateSensor::read() {
    uint16_t adc = ADC1_Read((uint32_t)HR_CH);
    return (adc * 200.0f) / 4095.0f;
}


// -------- SPO2 --------
float PulseOxiMeter::read() {
    uint16_t adc = ADC1_Read((uint32_t)SPO2_CH);
    return (adc * 100.0f) / 4095.0f;
}

