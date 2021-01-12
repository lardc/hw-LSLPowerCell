#ifndef REGULATOR_H_
#define REGULATOR_H_

// Includes
//
#include "stdinc.h"
#include "Measurement.h"

// Definitions
//
#define PULSE_BUFFER_SIZE	CURRENT_PULSE_WIDTH / TIMER15_uS

// Functions
//
bool REGULATOR_Process(MeasureSample* Sample);
void REGULATOR_CashVariables();
//

#endif /* REGULATOR_H_ */
