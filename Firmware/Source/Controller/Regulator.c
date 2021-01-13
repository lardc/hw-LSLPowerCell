// Header
//
#include "Regulator.h"
#include "DataTable.h"
#include "LowLevel.h"
#include "ConvertUtils.h"

// Functions prototypes
//
void REGULATOR_LoggingData(volatile RegulatorParamsStruct* Regulator);

// Functions
//
bool REGULATOR_Process(volatile RegulatorParamsStruct* Regulator)
{
	float RegulatorError, Qp;
	static float Qi = 0;

	RegulatorError = (Regulator->RegulatorPulseCounter == 0) ? 0 : (Regulator->CurrentTable[Regulator->RegulatorPulseCounter] - Regulator->MeasuredCurrent);

	Qi += RegulatorError * Regulator->Ki[Regulator->CurrentRange];

	if(Qi > DataTable[REG_REGULATOR_QI_MAX])
		Qi = DataTable[REG_REGULATOR_QI_MAX];

	if(Qi < (-1) * DataTable[REG_REGULATOR_QI_MAX])
		Qi = (-1) * DataTable[REG_REGULATOR_QI_MAX];

	Qp = RegulatorError * Regulator->Kp[Regulator->CurrentRange];

	Regulator->RegulatorOutput = Regulator->CurrentTable[Regulator->RegulatorPulseCounter] + Qp +Qi;

	if(Regulator->DebugMode)
		LL_WriteDAC(Regulator->CurrentTable[Regulator->RegulatorPulseCounter] + Regulator->DACOffset);
	else
		LL_WriteDAC(CU_ItoDAC(Regulator->RegulatorOutput, Regulator->CurrentRange) + Regulator->DACOffset);

	REGULATOR_LoggingData(Regulator);

	Regulator->RegulatorPulseCounter++;

	if(Regulator->RegulatorPulseCounter >= PULSE_BUFFER_SIZE)
	{
		Regulator->DebugMode = false;
		Regulator->RegulatorPulseCounter = 0;
		return true;
	}
	else
		return false;
}
//-----------------------------------------------

void REGULATOR_LoggingData(volatile RegulatorParamsStruct* Regulator)
{
	static Int16U ScopeLogStep = 0, LocalCounter = 0;

	// Сброс локального счетчика в начале логгирования
	if (CONTROL_Values_Counter == 0)
		LocalCounter = 0;

	if (ScopeLogStep++ >= DataTable[REG_SCOPE_STEP])
	{
		ScopeLogStep = 0;

		CONTROL_ValuesCurrent[LocalCounter] = (Int16U)(Regulator->MeasuredCurrent * 10);
		CONTROL_RegulatorErr[LocalCounter] = (Int16U)(Regulator->RegulatorError * 10);
		CONTROL_RegulatorOutput[LocalCounter] = (Int16U)(Regulator->RegulatorOutput * 10);
		CONTROL_ValuesBatteryVoltage[LocalCounter] = (Int16U)(Regulator->MeasuredBatteryVoltage * 10);

		CONTROL_Values_Counter = LocalCounter;

		++LocalCounter;
	}

	// Условие обновления глобального счетчика данных
	if (CONTROL_Values_Counter < VALUES_x_SIZE)
		CONTROL_Values_Counter = LocalCounter;

	// Сброс локального счетчика
	if (LocalCounter >= VALUES_x_SIZE)
		LocalCounter = 0;
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

	Regulator->DebugMode = false;
	Regulator->DACOffset = DataTable[REG_DAC_OFFSET];
}
//-----------------------------------------------


