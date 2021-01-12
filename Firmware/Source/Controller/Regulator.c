// Header
//
#include "Regulator.h"
#include "DataTable.h"
#include "LowLevel.h"
#include "ConvertUtils.h"

// Variables
//
Int16U	RegulatorPulseCounter;

// Functions
//
bool REGULATOR_Process(volatile RegulatorParamsStruct* Regulator)
{
	float RegulatorError, Qp, RegulatorOutput;
	static float Qi = 0;

	RegulatorError = (RegulatorPulseCounter == 0) ? 0 : (Regulator->CurrentTable[RegulatorPulseCounter] - Regulator->MeasuredCurrent);

	Qi += RegulatorError * Regulator->Ki[Regulator->CurrentRange];

	if(Qi > DataTable[REG_REGULATOR_QI_MAX])
		Qi = DataTable[REG_REGULATOR_QI_MAX];

	if(Qi < (-1) * DataTable[REG_REGULATOR_QI_MAX])
		Qi = (-1) * DataTable[REG_REGULATOR_QI_MAX];

	Qp = RegulatorError * Regulator->Kp[Regulator->CurrentRange];

	RegulatorOutput = Regulator->CurrentTable[RegulatorPulseCounter] + Qp +Qi;

	LL_WriteDAC(CU_ItoDAC(RegulatorOutput, Regulator->CurrentRange));

	RegulatorPulseCounter++;

	if(RegulatorPulseCounter >= PULSE_BUFFER_SIZE)
	{
		RegulatorPulseCounter = 0;
		return true;
	}
	else
		return false;
}
//-----------------------------------------------

void REGULATOR_CashVariables(volatile RegulatorParamsStruct* Regulator)
{
	// Кеширование коэффициентов регулятора
	for(int i = 0; i < CURRENT_RANGE_QUANTITY; i++)
	{
		Regulator->Kp[i] = DataTable[REG_REGULATOR_RANGE0_Kp + i * 2];
		Regulator->Ki[i] = DataTable[REG_REGULATOR_RANGE0_Ki + i * 2];
	}
}
//-----------------------------------------------


