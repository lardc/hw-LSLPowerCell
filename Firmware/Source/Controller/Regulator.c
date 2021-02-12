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
	static float Qi = 0, Qp;

	Regulator->RegulatorError = (Regulator->RegulatorPulseCounter == 0) ? 0 : (Regulator->CurrentTable[Regulator->RegulatorPulseCounter] - Regulator->MeasuredCurrent);

	Qi += Regulator->RegulatorError * (Regulator->Ki[Regulator->CurrentRange] + Regulator->KiTune[Regulator->CurrentRange]);

	if(Qi > DataTable[REG_REGULATOR_QI_MAX])
		Qi = DataTable[REG_REGULATOR_QI_MAX];

	if(Qi < (-1) * DataTable[REG_REGULATOR_QI_MAX])
		Qi = (-1) * DataTable[REG_REGULATOR_QI_MAX];

	Qp = Regulator->RegulatorError * Regulator->Kp[Regulator->CurrentRange];

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
		Qi = 0;
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

		CONTROL_ValuesCurrent[LocalCounter] = (Int16U)(Regulator->MeasuredCurrent);
		CONTROL_RegulatorErr[LocalCounter] = (Int16S)(Regulator->RegulatorError);
		CONTROL_RegulatorOutput[LocalCounter] = (Int16U)(Regulator->RegulatorOutput);
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
	float CurrentMax = (float)DataTable[REG_CURRENT_PER_CURBOARD] / 10 * DataTable[REG_CURBOARD_QUANTITY];
	float CurrentTarget = (float)DataTable[REG_CURRENT_PULSE_VALUE] / 10;

	// Кеширование коэффициентов регулятора
	for(int i = 0; i < CURRENT_RANGE_QUANTITY; i++)
	{
		Regulator->Kp[i] = (float)DataTable[REG_REGULATOR_RANGE0_Kp + i * 2] / 1000;
		Regulator->Ki[i] = (float)DataTable[REG_REGULATOR_RANGE0_Ki + i * 2] / 1000;
		Regulator->KiTune[i] = (CurrentMax - CurrentTarget) * (float)DataTable[REG_REGULATOR_TF_Ki_RANG0 + i] / 1e6;
	}

	Regulator->DebugMode = false;
	Regulator->DACOffset = DataTable[REG_DAC_OFFSET];
}
//-----------------------------------------------


