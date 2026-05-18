/*
 * header.h
 *
 *  Created on: 23-Apr-2026
 *      Author: mirafra
 */

#ifndef INC_HEADER_H_
#define INC_HEADER_H_

#include "FreeRTOS.h"
#include "queue.h"
#include "main.h"
#include "stdio.h"
#include "semphr.h"


extern SemaphoreHandle_t lcdMutex;

typedef struct
{
	uint32_t ch12_HRsensor;
	uint32_t ch11_oxysensor;
	uint32_t ch10_tempsensor;
} ADC_data;

typedef struct
{
	uint32_t temp;
	uint32_t oxygen;
	uint32_t heartrate;
}Sensor_data;

typedef struct
{
	char Tempmsg[32];
	char Oxymsg[32];
	char HRmsg[32];
}Message_data;

extern QueueHandle_t adcQueue;
extern QueueHandle_t sensorQueue;
extern QueueHandle_t taskQueue;
extern QueueHandle_t buttonQueue;

extern TaskHandle_t alert_handle;


class ADC_class
{

public:
	void ADC_init();
	void ADC_read(ADC_data &data);
};

class USART_class
{
public:
	void USART2_init();
	void USART2_write(uint8_t ch);
	uint8_t USART2_read();
	void USART2_string(const char* str);
};

class GPIO_class
{
private:
	uint32_t pin;
public:
	GPIO_class(uint32_t gpiopin);
	void GPIO_init();
	void LED_On();
	void LED_Off();
	void LED_Toggle();
};

class LCD_class
{
private:
	void GPIO_Init();
	static void delay(void);
	void LCD_write(uint8_t val);
	void LCD_data(uint8_t val);
	//void LCD_cmd(uint8_t val);
	void LCD_Init();
public:
	 LCD_class();
	 void write_string(const char* str);
	 void LCD_cmd(uint8_t val);
	 void LCD_clear();

};

class Buzzer_class
{
public:
	void buzzer_init();
	void delay(uint32_t d);
	void beep_low();
	void beep_medium();
	void beep_high();
};

class Vital_Sensor {
public:
virtual float read(const ADC_data &data) =0;
};

class TemperatureSensor: public Vital_Sensor
{
public:
	float read(const ADC_data &data) override;
};

class OxygenSensor: public Vital_Sensor
{
public:
	float read(const ADC_data &data) override;
};

class HeartrateSensor: public Vital_Sensor
{
public:
	float read(const ADC_data &data) override;
};



class DataAcquisitionTask
{
public:
	static void Task(void* parameters);
};

class SensorProcessingTask
{
public:
	static void Task(void* parameters);
};

class HealthMonitorTask
{
public:
	static void Task(void* parameters);
};

class DisplayTask
{
public:
	static void Task(void* parameters);
};

class ButtonHandlerTask
{
public:
	static void Task(void* parameters);
};

class AlertTask
{
public:
	static void Task(void* parameters);
};



#endif /* INC_HEADER_H_ */























