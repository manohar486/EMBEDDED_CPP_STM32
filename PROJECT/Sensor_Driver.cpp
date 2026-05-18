/*
 * Sensor_Driver.cpp
 *
 *  Created on: 23-Apr-2026
 *      Author: mirafra
 */
#include "header.h"
#define Baseline 110
#define Sensitivity 25

 float fahrenheit=0, spo2=0 ,beats=0;

float TemperatureSensor::read(const ADC_data &data)
{
	float celsius;

	celsius = (data.ch10 * 100.0f) / 4095.0f;
    fahrenheit = (celsius * 9.0f / 5.0f) + 32.0f; // convert °C to °F

    return fahrenheit;
}

float OxygenSensor::read(const ADC_data &data)
{
	float vir,vred,R;

	vir = (data.ch11 * 3.3f) / 4095.0f;
	vred = (data.ch11 * 3.3f) / 4095.0f;

	R= (vir / vred);

	spo2 = (Baseline- Sensitivity) *R ;

	return spo2;
}

float HeartrateSensor::read(const ADC_data &data)
{
	return beats;
}


















