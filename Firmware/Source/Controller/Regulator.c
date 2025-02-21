// Header
//
#include "Regulator.h"
#include "DataTable.h"
#include "LowLevel.h"
#include "ConvertUtils.h"
#include "Math.h"
#include "Controller.h"

// Functions prototypes
//
void REGULATOR_LoggingData(volatile RegulatorParamsStruct* Regulator);
Int16U REGULATOR_DACApplyLimits(float Value, Int16U Offset, Int16U LimitValue);

// Functions
//
bool REGULATOR_Process(volatile RegulatorParamsStruct* Regulator)
{
	static float Qi = 0, Qp;
	static Int16U  FollowingErrorCounter = 0;
	Regulator->RegulatorError = (Regulator->RegulatorPulseCounter == 0) ? 0 : (Regulator->CurrentTable[Regulator->RegulatorPulseCounter] - Regulator->MeasuredCurrent);

	if(fabsf(Regulator->RegulatorError / Regulator->CurrentTarget * 100) < Regulator->RegulatorAlowedError)
		FollowingErrorCounter = 0;
	else
		FollowingErrorCounter++;

	if(FollowingErrorCounter >= Regulator->FollowingErrorCounterMax)
		{
			FollowingErrorCounter = 0;
			CONTROL_StopProcess();
			CONTROL_SetDeviceState(DS_Ready, SS_None);
			DataTable[REG_PROBLEM] = PROBLEM_FOLLOWING_ERROR;
		}

	Qp = Regulator->RegulatorError * Regulator->Kp;
	Qi += Regulator->RegulatorError * (Regulator->Ki + Regulator->KiTune);

	float Qi_max = (float)DataTable[REG_REGULATOR_QI_MAX];
	if(Qi > Qi_max)
		Qi = Qi_max;
	else if (Qi < -Qi_max)
		Qi = -Qi_max;

	Regulator->RegulatorOutput = Regulator->CurrentTable[Regulator->RegulatorPulseCounter] + Qp + Qi;

	// Выбор источника данных для записи в ЦАП
	float ValueToDAC;
	if(Regulator->DebugMode)
		ValueToDAC = Regulator->CurrentTable[Regulator->RegulatorPulseCounter];
	else
		ValueToDAC = CU_ItoDAC(Regulator->RegulatorOutput);

	// Проверка границ диапазона ЦАП
	Regulator->DACSetpoint = REGULATOR_DACApplyLimits(ValueToDAC, Regulator->DACOffset, Regulator->DACLimitValue);
	LL_WriteDAC(Regulator->DACSetpoint);

	REGULATOR_LoggingData(Regulator);
	Regulator->RegulatorPulseCounter++;
	if(Regulator->RegulatorPulseCounter >= PULSE_BUFFER_SIZE || DataTable[REG_PROBLEM] == PROBLEM_FOLLOWING_ERROR)
	{
		Regulator->DebugMode = false;
		Regulator->RegulatorPulseCounter = 0;
		Qi = 0;
		FollowingErrorCounter = 0;
		return true;
	}
	else
		return false;
}
//-----------------------------------------------

Int16U REGULATOR_DACApplyLimits(float Value, Int16U Offset, Int16U LimitValue)
{
	Int16S Result = (Int16S)(Value + Offset);
	if (Result < 0)
		return 0;
	else if (Result > LimitValue)
		return LimitValue;
	else
		return Result;
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
		CONTROL_RegulatorOutput[LocalCounter] = (Int16S)(Regulator->RegulatorOutput);
		CONTROL_ValuesBatteryVoltage[LocalCounter] = (Int16U)(Regulator->MeasuredBatteryVoltage * 10);
		CONTROL_DACRawData[LocalCounter] = (Int16U)(Regulator->DACSetpoint);

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
