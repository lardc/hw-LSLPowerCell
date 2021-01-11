// Includes
//
#include "ConvertUtils.h"
#include "Global.h"
#include "LowLevel.h"
#include "DataTable.h"
#include "Measurement.h"

// Definitions
#define DAC_MAX_VAL		0x0FFF

// Structs
typedef struct __ConvertParams
{
	float P2;
	float P1;
	float P0;
	float K;
	float B;
}ConvertParams;

// Variables
ConvertParams AdcToVoltageParams;
ConvertParams AdcToCurrentParams[CURRENT_RANGE_QUANTITY];
ConvertParams CurrentToDacParams[CURRENT_RANGE_QUANTITY];
//

// Functions prototypes
float CU_ADCtoX(Int16U Data, ConvertParams* Coefficients);


// Functions
//
Int16U CU_ItoDAC(float Current, Int16U CurrentRange)
{
	float DACValue = (Current + CurrentToDacParams[CurrentRange].B) * CurrentToDacParams[CurrentRange].K;

	if(DACValue < 0)
		DACValue = 0;

	if (DACValue > DAC_MAX_VAL)
		DACValue = DAC_MAX_VAL;

	return (Int16U)DACValue;
}
//-----------------------------

float CU_ADCtoX(Int16U Data, ConvertParams* Coefficients)
{
	float Temp;

	Temp = Data * Coefficients->K + Coefficients->B;
	Temp = Temp * Temp * Coefficients->P2 + Temp * Coefficients->P1 + Coefficients->P0;

	return Temp;
}
//-----------------------------

float CU_ADCtoI(Int16U Data, Int16U CurrentRange)
{
	return CU_ADCtoX(Data, &AdcToCurrentParams[CurrentRange]);
}
//-----------------------------

float CU_ADCtoV(Int16U Data)
{
	return CU_ADCtoX(Data, &AdcToVoltageParams);
}
//-----------------------------

void CU_LoadConvertParams()
{
	// Параметры преобразования значения АЦП в напряжение
	AdcToVoltageParams.P2 = 0;
	AdcToVoltageParams.P1 = 1;
	AdcToVoltageParams.P0 = 0;
	AdcToVoltageParams.K = (float)DataTable[REG_ADC_VOLTAGE_K] / 1000;
	AdcToVoltageParams.B = (Int16S)DataTable[REG_ADC_VOLTAGE_K];

	// Параметры преобразования значения АЦП в ток и тока в ЦАП
	for(int i = 0; i < MEASURE_CURRENT_RANGE_QUANTITY; i++)
	{
		AdcToCurrentParams[i].P2 = (float)DataTable[REG_ADC_I_RANGE0_P2 + i * 6] / 1e-6;
		AdcToCurrentParams[i].P1 = (float)DataTable[REG_ADC_I_RANGE0_P1 + i * 6] / 1000;
		AdcToCurrentParams[i].P0 = (Int16S)DataTable[REG_ADC_I_RANGE0_P0 + i * 6];
		AdcToCurrentParams[i].K = (float)DataTable[REG_ADC_I_RANGE0_N + i * 6] / DataTable[REG_ADC_I_RANGE0_D + i * 6];
		AdcToCurrentParams[i].B = (Int16S)DataTable[REG_ADC_I_RANGE0_B + i * 6];

		CurrentToDacParams[i].K = (float)DataTable[REG_I_TO_DAC_RANGE0_K + i * 2] / 1000;
		CurrentToDacParams[i].B = (Int16S)DataTable[REG_I_TO_DAC_RANGE0_B + i * 2];
	}
}
//-----------------------------

