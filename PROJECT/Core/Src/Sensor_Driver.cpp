/*
 * Sensor_Driver.cpp
 *
 *  Created on: 23-Apr-2026
 *      Author: mirafra
 */
#include "header.h"

#define Baseline 110
#define Sensitivity 25
#define HRmax 180
#define HRmin 50

 float fahrenheit=0, spo2=0 ,beats=0;

float TemperatureSensor::read(const ADC_data &data)
{
	float celsius;

	celsius = (data.ch10_tempsensor * 100.0f) / 4095.0f;
    fahrenheit = (celsius * 9.0f / 5.0f) + 32.0f; // convert °C to °F

    return fahrenheit;// fahrenheit
}

float OxygenSensor::read(const ADC_data &data)
{
	 float vir;

	    vir = (data.ch13_oxysensor * 3.3f) / 4095.0f;

	    // Map voltage range to SpO2 range
	    spo2 = 90.0f + (vir * 10.0f);   // Example scaling

	    // Clamp values
	    if (spo2 > 100.0f) spo2 = 100.0f;
	    if (spo2 < 80.0f)  spo2 = 80.0f;

	    return spo2;
}

float HeartrateSensor::read(const ADC_data &data)
{
	beats= HRmin + (data.ch12_HRsensor*(HRmax-HRmin))/4095.0f;

	return beats;//Beat per minite(BPM)
}

















