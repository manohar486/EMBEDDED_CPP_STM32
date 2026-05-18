#ifndef INC_ADC_H_
#define INC_ADC_H_
#include "stm32f4xx.h"
#include<cstdint>

void ADC1_Init();
uint32_t ADC1_Read(uint32_t channel);

#endif /* INC_ADC_H_ */
