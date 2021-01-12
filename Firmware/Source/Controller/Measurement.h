#ifndef MEASUREMENT_H_
#define MEASUREMENT_H_

// Includes
#include "SysConfig.h"
#include "ZwBase.h"
#include "ConvertUtils.h"

// Structs
//
typedef struct __MeasureSample
{
	Int16U CurrentRange;
	float CurrentTarget;
	float Current;
	float Voltage;
}MeasureSample;

extern MeasureSample SampleParams;

// Definitions
#define PULSE_ARR_MAX_LENGTH					300				// Количество точек оцифровки
//

// Variables
extern Int16U MEASURE_ADC_CurrentRaw[ADC_DMA_BUFF_SIZE];

// Functions
float MEASURE_SampleVoltage();
void MEASURE_SampleCurrent();
void MEASURE_SampleParams(volatile MeasureSample* Sample);
void MEASURE_DMABufferClear();
void MEASURE_SetCurrentRange(float Current);

#endif /* MEASUREMENT_H_ */
