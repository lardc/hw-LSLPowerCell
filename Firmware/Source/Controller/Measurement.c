// Includes
#include "Measurement.h"
#include "Board.h"
#include "LowLevel.h"
#include "DataTable.h"
#include "Global.h"

// Variables
//
Int16U MEASURE_ADC_CurrentRaw[ADC_DMA_BUFF_SIZE];
Int16U MEASURE_ADC_BatteryVoltageRaw[ADC_DMA_BUFF_SIZE];

// Functions prototypes
Int16U MEASURE_DMAExtractX(Int16U* InputArray, Int16U ArraySize);
Int16U MEASURE_DMAExtractCurrent();
void MEASURE_StartNewSampling();

// Functions
//
float MEASURE_SampleVoltage()
{
	return CU_ADCtoV(ADC_Measure(ADC1, ADC1_V_BAT_CHANNEL));
}
//-----------------------------------------------

void MEASURE_SampleCurrent(volatile RegulatorParamsStruct* Regulator)
{
	Regulator->MeasuredCurrent =  CU_ADCtoI(MEASURE_DMAExtractCurrent(), Regulator->CurrentRange);
	MEASURE_StartNewSampling();
}
//-----------------------------------------------

Int16U MEASURE_DMAExtractX(Int16U* InputArray, Int16U ArraySize)
{
	Int32U AverageData = 0;

	for(int i = 0; i < ArraySize; i++)
		AverageData += *(InputArray + i);

	return (Int16U)((float)AverageData / ArraySize);
}
//-----------------------------------------------

Int16U MEASURE_DMAExtractCurrent()
{
	return MEASURE_DMAExtractX(&MEASURE_ADC_CurrentRaw[1], ADC_DMA_BUFF_SIZE - 1);
}
//-----------------------------------------------

void MEASURE_DMABufferClear()
{
	for(int i = 0; i < ADC_DMA_BUFF_SIZE; i++)
		MEASURE_ADC_CurrentRaw[i] = 0;
}
//-----------------------------------------------

void MEASURE_StartNewSampling()
{
	DMA_TransferCompleteReset(DMA2, DMA_TRANSFER_COMPLETE);
	ADC_SamplingStart(ADC3);
}
//-----------------------------------------------

void MEASURE_SetCurrentRange(volatile RegulatorParamsStruct* Regulator)
{
	if(Regulator->MeasuredCurrent <= DataTable[REG_CURRENT_LEVEL_RANGE0])
	{
		Regulator->CurrentRange = CURRENT_RANGE_0;
		LL_SetCurrentRange0();
	}
	else if(Regulator->MeasuredCurrent <= DataTable[REG_CURRENT_LEVEL_RANGE1])
	{
		Regulator->CurrentRange = CURRENT_RANGE_0;
		LL_SetCurrentRange1();
	}
	else
	{
		Regulator->CurrentRange = CURRENT_RANGE_0;
		LL_SetCurrentRange2();
	}

}
//-----------------------------------------------


