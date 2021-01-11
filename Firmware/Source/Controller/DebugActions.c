// Header
#include "DebugActions.h"

// Include
//
#include "LowLevel.h"
#include "Board.h"
#include "Delay.h"
#include "Controller.h"
#include "DataTable.h"

// Definitions
//
#define DBG_CURRENT_RANGE_0		0
#define DBG_CURRENT_RANGE_1		1
#define DBG_CURRENT_RANGE_2		2
#define DBG_CURRENT_RANGE_3		3

// Functions
//
void DBGACT_GenerateImpulseToLineSync()
{
	LL_SetStateLineSync(true);
	CONTROL_DelayMs(10);
	LL_SetStateLineSync(false);
}
//-----------------------------

void DBGACT_SetCurrentRange(uint16_t Range)
{
	switch(Range)
	{
		case DBG_CURRENT_RANGE_0:
			LL_SetCurrentRange0();
			break;

		case DBG_CURRENT_RANGE_1:
			LL_SetCurrentRange1();
			break;

		case DBG_CURRENT_RANGE_2:
			LL_SetCurrentRange2();
			break;
	}
}
//-----------------------------

void DBGACT_PowerSupplyEnable(bool State)
{
	LL_PowerSupplyEnable(State);
}
//-----------------------------
