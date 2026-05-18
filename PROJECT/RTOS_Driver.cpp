/*
 * RTOS_Task.cpp
 *
 *  Created on: 24-Apr-2026
 *      Author: mirafra
 */
#include "header.h"
#include <cstring>

 //object for adc class
ADC_data data;

QueueHandle_t adcQueue = NULL;
QueueHandle_t sensorQueue = NULL;
QueueHandle_t taskQueue = NULL;
QueueHandle_t buttonQueue = NULL;

TaskHandle_t alert_handle = NULL;
SemaphoreHandle_t lcdMutex = NULL;

void ADC_Task::Task(void* parameters)
{
	ADC_class adc;
	adc.ADC_init();

	while(1)
	{
		//HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
		adc.ADC_read(data);

		xQueueSend(adcQueue, &data, portMAX_DELAY);
		vTaskDelay(pdMS_TO_TICKS(500));

	}
}


void Sensor_Task::Task(void* parameters)
{
	TemperatureSensor Tsensor;
	Vital_Sensor *ptr1 = &Tsensor;

	OxygenSensor Osensor;
	Vital_Sensor *ptr2 = &Osensor;

	HeartrateSensor HRsensor;
	Vital_Sensor *ptr3 = &HRsensor;

	ADC_data value;
	Sensor_data sensor;

	while(1)
	{
		if(xQueueReceive(adcQueue, &value, portMAX_DELAY))
		{
			//HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
			sensor.temp = ptr1->read(value);

			sensor.oxygen = ptr2->read(value);

			sensor.heartrate = ptr3->read(value);

			xQueueSend(sensorQueue, &sensor, portMAX_DELAY);
		}

	}
}


void Control_Task::Task(void* parameters)
{
	Sensor_data sensor;
	Message_data str;
	while(1)
	{
		if(xQueueReceive(sensorQueue, &sensor, portMAX_DELAY))
		{
//			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
			/* TEMPERATURE OF BODY */
			if(sensor.temp > 99)
			{
			    strcpy(str.Tempmsg,"Fever");
			}
			else if(sensor.temp >= 97 && sensor.temp <= 99)
			{
			    strcpy(str.Tempmsg,"Normal");
			}
			else if(sensor.temp >= 86 && sensor.temp < 97)
			{
			    strcpy(str.Tempmsg,"Mild hypothermia");
			}
			else
			{
			    strcpy(str.Tempmsg,"Severe hypothermia");
			}

            /* OXYGEN IN BLOOD */
			if(sensor.oxygen >= 95 && sensor.oxygen<=100)
			{
				strcpy(str.Oxymsg,"Normal spo2");
			}
			else if(sensor.oxygen >=92 && sensor.oxygen <95)
			{
				strcpy(str.Oxymsg,"Low oxygen");
			}
			else
			{
				strcpy(str.Oxymsg,"serious hypoxemia");
			}

			/* HEART RATE  */
			if(sensor.heartrate >= 60 && sensor.heartrate<=100)
			{
				strcpy(str.HRmsg,"Normal Heart rate");
			}
			else if(sensor.heartrate > 100)
			{
				strcpy(str.HRmsg," Tachycardia");
			}
			else if(sensor.heartrate < 50)
			{
				strcpy(str.HRmsg,"Bradycardia");
			}

			xQueueSend(taskQueue,&str, portMAX_DELAY);
		}
	}
}


void Display_Task::Task(void* parameters)
{
	Message_data str;
	LCD_class lcd1;
	GPIO_class green(12);
	while(1)
	{
		if(xQueueReceive(taskQueue, &str, portMAX_DELAY)==pdTRUE)
		{

		//	xSemaphoreTake(lcdMutex, portMAX_DELAY);

			 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
			 lcd1.LCD_clear();
			 vTaskDelay(pdMS_TO_TICKS(2));

			 lcd1.LCD_cmd(0x80);
			 vTaskDelay(pdMS_TO_TICKS(2));
			 lcd1.write_string(str.Tempmsg);
			 vTaskDelay(pdMS_TO_TICKS(1000));

			 lcd1.LCD_cmd(0xC0);
			 vTaskDelay(pdMS_TO_TICKS(2));
			 lcd1.write_string(str.Oxymsg);
			 vTaskDelay(pdMS_TO_TICKS(1000));

			 lcd1.LCD_clear();
			 vTaskDelay(pdMS_TO_TICKS(2));
			 lcd1.LCD_cmd(0x80);
			 vTaskDelay(pdMS_TO_TICKS(2));
			 lcd1.write_string(str.HRmsg);
			 vTaskDelay(pdMS_TO_TICKS(1000));


			// xSemaphoreGive(lcdMutex);
		}
	}
}


void Button_Task::Task(void* parameters)
{
	uint32_t button_id;
	LCD_class lcd1;

	while(1)
	{
		 if (xQueueReceive(buttonQueue, &button_id, portMAX_DELAY))
		 {
			 //xSemaphoreTake(lcdMutex, portMAX_DELAY);

			 lcd1.LCD_clear();
			 lcd1.LCD_cmd(0x80);

			 if(button_id==1)
			 {
				 lcd1.write_string("Nurse Ack");
				 xTaskNotify(alert_handle, 1, eSetValueWithOverwrite);

				 vTaskDelay(pdMS_TO_TICKS(1000));
			 }

			 else if(button_id==2)
			 {
				 lcd1.write_string("Emergency");
				 xTaskNotify(alert_handle, 2, eSetValueWithOverwrite);

				 vTaskDelay(pdMS_TO_TICKS(1000));
			 }

			// xSemaphoreGive(lcdMutex);
		 }
	}
}


void Alert_Task::Task(void* parameters)
{
	uint32_t value;
	Buzzer_class buz;
	buz.buzzer_init();
	while(1)
	{
		xTaskNotifyWait(0, 0xFFFFFFFF, &value, portMAX_DELAY);

		if(value==1)
		{
			buz.beep_low();
		}
		else if(value==2)
		{
			buz.beep_high();
		}
	}
}







































