#ifndef REGULATOR_H_
#define REGULATOR_H_

// Includes
//
#include "stdinc.h"
#include "Global.h"
#include "SysConfig.h"

// Definitions
//
#define PULSE_BUFFER_SIZE	CURRENT_PULSE_WIDTH / TIMER15_uS

// Structs
//
typedef struct __RegulatorParams
{
	Int16U CurrentRange;
	float CurrentTarget;
	float MeasuredCurrent;
	float CurrentTable[PULSE_BUFFER_SIZE];
	float Kp[CURRENT_RANGE_QUANTITY];
	float Ki[CURRENT_RANGE_QUANTITY];
	bool DebugMode;
}RegulatorParamsStruct;

// Functions
//
bool REGULATOR_Process(volatile RegulatorParamsStruct* Regulator);
void REGULATOR_CashVariables(volatile RegulatorParamsStruct* Regulator);
//

#endif /* REGULATOR_H_ */
