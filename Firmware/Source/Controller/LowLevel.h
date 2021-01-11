#ifndef __LOWLEVEL_H
#define __LOWLEVEL_H

// Include
#include "Board.h"
#include "stdinc.h"

// Functions
//
void LL_ToggleBoardLED();
void LL_SetStateLineSync(bool State);
void LL_PowerSupplyEnable(bool State);
void LL_SetCurrentRange0();
void LL_SetCurrentRange1();
void LL_SetCurrentRange2();
void LL_FanEnable(bool State);
void INITCFG_DAC();

#endif //__LOWLEVEL_H
