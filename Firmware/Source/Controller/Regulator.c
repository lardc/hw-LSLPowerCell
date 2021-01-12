// Header
//
#include "Regulator.h"
#include "SysConfig.h"
#include "DataTable.h"
#include "LowLevel.h"

// Structs
//
typedef struct __RegulatorParams
{
	float Kp;
	float Ki;
}RegulatorParamsStruct;

volatile RegulatorParamsStruct RegulatorParams[CURRENT_RANGE_QUANTITY];

// Variables
//
Int16U	RegulatorPulseCounter;

// Functions
//
bool REGULATOR_Process(MeasureSample* Sample)
{
	float RegulatorError, Qp;
	static float Qi = 0;

	RegulatorError = (RegulatorPulseCounter == 0) ? 0 : (Sample->CurrentTarget - Sample->Current);

	Qi += RegulatorError * RegulatorParams[Sample->CurrentRange].Ki;

	if(Qi > DataTable[REG_REGULATOR_QI_MAX])
		Qi = DataTable[REG_REGULATOR_QI_MAX];

	if(Qi < (-1) * DataTable[REG_REGULATOR_QI_MAX])
		Qi = (-1) * DataTable[REG_REGULATOR_QI_MAX];

	Qp = RegulatorError * RegulatorParams[Sample->CurrentRange].Kp;

	LL_WriteDAC((Int16U)(Sample->CurrentTarget + Qp +Qi));

	RegulatorPulseCounter++;

	if(RegulatorPulseCounter >= PULSE_BUFFER_SIZE)
		return true;
	else
		return false;
}
//-----------------------------------------------

void REGULATOR_CashVariables()
{
	// Кеширование коэффициентов регулятора
	for(int i = 0; i < CURRENT_RANGE_QUANTITY; i++)
	{
		RegulatorParams[i].Kp = DataTable[REG_REGULATOR_RANGE0_Kp + i * 2];
		RegulatorParams[i].Ki = DataTable[REG_REGULATOR_RANGE0_Ki + i * 2];
	}
}
//-----------------------------------------------


