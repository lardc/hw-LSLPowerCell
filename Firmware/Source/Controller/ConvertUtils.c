// Includes
//
#include "ConvertUtils.h"
#include "LowLevel.h"
#include "DataTable.h"
#include "Global.h"

// Structs
typedef struct __ConvertParams
{
	float P2;
	float P1;
	float P0;
	float K;
	float B;
	float Kamp;
}ConvertParams;

// Variables
ConvertParams AdcToVoltageParams;
ConvertParams AdcToCurrentParams;
ConvertParams CurrentToDacParams;

// Functions prototypes
float CU_ADCtoX(float Data, ConvertParams* Coefficients);

// Functions
//
float CU_ItoDAC(float Current)
{
	if(DataTable[REG_I2DAC_CUST_RANGE0] || DataTable[REG_I2DAC_CUST_RANGE1])
		return CU_ADCtoX(Current, &CurrentToDacParams);
	else
	{
		// Менее удачный алгоритм пересчёта оставлен для обратной совместимости
		// Пересчет амплитуды тока в расчете на одну CurrentBoard
		Current = Current / DataTable[REG_CURBOARD_QUANTITY];
		return (Current + CurrentToDacParams.B) * CurrentToDacParams.K;
	}
}
//-----------------------------

float CU_ADCtoX(float Data, ConvertParams* Coefficients)
{
	return (Data * Coefficients->K + Coefficients->B);
}
//-----------------------------

float CU_ADCtoI(Int16U Data)
{
	float Uadc, Current;

	Uadc = CU_ADCtoX(Data, &AdcToCurrentParams);
	Current = Uadc / AdcToCurrentParams.Kamp / DataTable[REG_SHUNT_RESISTANCE] * 1000;

	return (Current * Current * AdcToCurrentParams.P2 + Current * AdcToCurrentParams.P1 + AdcToCurrentParams.P0);
}
//-----------------------------

float CU_ADCtoV(Int16U Data)
{
	return CU_ADCtoX(Data, &AdcToVoltageParams);
}
//-----------------------------

void CU_LoadConvertParams(Int16U CurrentRange, float TargetCurrent)
{
	// Параметры преобразования значения АЦП в ток и тока в ЦАП
	AdcToCurrentParams.P2 = (float)((Int16S)DataTable[REG_ADC_I_RANGE0_P2 + CurrentRange * 6]) / 1e6;
	AdcToCurrentParams.P1 = (float)DataTable[REG_ADC_I_RANGE0_P1 + CurrentRange * 6] / 1000;
	AdcToCurrentParams.P0 = (float)((Int16S)DataTable[REG_ADC_I_RANGE0_P0 + CurrentRange * 6]) / 10;
	AdcToCurrentParams.K = (float)DataTable[REG_ADC_I_RANGE0_N + CurrentRange * 6] / DataTable[REG_ADC_I_RANGE0_D + CurrentRange * 6];
	AdcToCurrentParams.B = (Int16S)DataTable[REG_ADC_I_RANGE0_B + CurrentRange * 6];
	AdcToCurrentParams.Kamp = (float)DataTable[REG_K_AMP_RANGE0 + CurrentRange] / 100;

	// Кастомные диапазоны пересчёта тока в ЦАП
	if(DataTable[REG_I2DAC_CUST_RANGE0] || DataTable[REG_I2DAC_CUST_RANGE1])
	{
		TargetCurrent *= 10;
		if(TargetCurrent < DataTable[REG_I2DAC_CUST_RANGE0])
			CurrentRange = 0;
		else if(TargetCurrent < DataTable[REG_I2DAC_CUST_RANGE1])
			CurrentRange = 1;
		else
			CurrentRange = 2;
	}
	CurrentToDacParams.K = (float)DataTable[REG_I_TO_DAC_RANGE0_K + CurrentRange * 2] / 1000;
	CurrentToDacParams.B = (Int16S)DataTable[REG_I_TO_DAC_RANGE0_B + CurrentRange * 2];
}

void CU_LoadConvertVoltageParams()
{
	// Параметры преобразования значения АЦП в напряжение
	AdcToVoltageParams.K = (float)DataTable[REG_ADC_VOLTAGE_K] / 1e6;
	AdcToVoltageParams.B = (Int16S)DataTable[REG_ADC_VOLTAGE_B];
}
//-----------------------------
