#ifndef MEASUREMENT_H_
#define MEASUREMENT_H_

// Includes
#include "SysConfig.h"
#include "ZwBase.h"

typedef struct __MeasureSample
{
	float Current;
	float Voltage;
}MeasureSample;

// Definitions
#define PULSE_ARR_MAX_LENGTH					300				// Количество точек оцифровки
//
#define MEASURE_CURRENT_RANGE_QUANTITY			3
#define MEASURE_CURRENT_RANGE_0					0
#define MEASURE_CURRENT_RANGE_1					1
#define MEASURE_CURRENT_RANGE_2					2

// Variables
extern Int16U MEASURE_ADC_CurrentRaw[ADC_DMA_BUFF_SIZE];

// Functions
float MEASURE_SampleVoltage();
void MEASURE_SampleCurrent(volatile MeasureSample* Sample);
void MEASURE_SampleParams(volatile MeasureSample* Sample);
void MEASURE_DMABuffersClear();
void MEASURE_SetCurrentRange(float Current);

#endif /* MEASUREMENT_H_ */
