// Header
#include "Diagnostic.h"

// Includes
#include "DataTable.h"
#include "LowLevel.h"
#include "Controller.h"
#include "DebugActions.h"

// Functions
bool DIAG_HandleDiagnosticAction(uint16_t ActionID, uint16_t *pUserError)
{
	switch (ActionID)
	{
		case ACT_DBG_PULSE_SYNC:
			{
				DBGACT_GenerateImpulseToLineSync();
			}
			break;

		case ACT_DBG_SET_CURRENT_RANGE:
			{
				DBGACT_SetCurrentRange(DataTable[REG_DBG]);
			}
			break;

		case ACT_DBG_POWER_SUPPLY_EN:
			{
				DBGACT_PowerSupplyEnable(DataTable[REG_DBG]);
			}
			break;

		default:
			return false;
	}

	return true;
}
