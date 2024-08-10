#ifndef CONVERTUTILS_H_
#define CONVERTUTILS_H_

// Includes
#include "ZwBase.h"

// Functions
float CU_ItoDAC(float Current);
float CU_ADCtoI(Int16U Data);
float CU_ADCtoV(Int16U Data);
void CU_LoadConvertParams(Int16U CurrentRange, float TargetCurrent);
void CU_LoadConvertVoltageParams();

#endif // CONVERTUTILS_H_
