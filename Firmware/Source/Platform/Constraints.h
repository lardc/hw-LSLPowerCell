#ifndef __CONSTRAINTS_H
#define __CONSTRAINTS_H

// Include
#include "stdinc.h"
#include "DataTable.h"
#include "Global.h"

//Definitions
#define NO		0
#define YES		1
//
#define I_RANGE0_THRESHOLD_MIN		0			// A * 10
#define I_RANGE0_THRESHOLD_MAX		INT16U_MAX	// A * 10
#define I_RANGE0_THRESHOLD_DEF		10000		// A * 10
//
#define I_RANGE1_THRESHOLD_MIN		0			// A * 10
#define I_RANGE1_THRESHOLD_MAX		INT16U_MAX	// A * 10
#define I_RANGE1_THRESHOLD_DEF		65000		// A * 10
//
#define V_BAT_THRESHOLD_MIN			100			// ֲ * 10
#define V_BAT_THRESHOLD_MAX			500			// ֲ * 10
#define V_BAT_THRESHOLD_DEF			450			// ֲ * 10
//
#define DAC_OUTPUT_LIM_MIN			0
#define DAC_OUTPUT_LIM_MAX			4095
#define DAC_OUTPUT_LIM_DEF			3500
//
#define DAC_OFFSET_MIN				0
#define DAC_OFFSET_MAX				4095
#define DAC_OFFSET_DEF				1300
//
#define FULL_CHARGE_TIMEOUT_MIN		30000		// לס
#define FULL_CHARGE_TIMEOUT_MAX		60000		// לס
#define FULL_CHARGE_TIMEOUT_DEF		50000		// לס
//
#define RECHARGE_TIMEOUT_MIN		0			// לס
#define RECHARGE_TIMEOUT_MAX		40000		// לס
#define RECHARGE_TIMEOUT_DEF		20000		// לס
//
#define SHUNT_RESISTANCE_MIN		0			// לך־ל
#define SHUNT_RESISTANCE_MAX		5000		// לך־ל
#define SHUNT_RESISTANCE_DEF		250			// לך־ל
//
#define AFTER_PULSE_PAUSE_MIN		0			// לס
#define AFTER_PULSE_PAUSE_MAX		40000		// לס
#define AFTER_PULSE_PAUSE_DEF		15000		// לס
//
#define I_PER_CURBOARD_MIN			500			// A * 10
#define I_PER_CURBOARD_MAX			15000		// A * 10
#define I_PER_CURBOARD_DEF			10833		// A * 10
//
#define CURBOARD_QUANTITY_MIN		1
#define CURBOARD_QUANTITY_MAX		6
#define CURBOARD_QUANTITY_DEF		6
//
#define COEF_P2_MIN					0
#define COEF_P2_MAX					INT16U_MAX
#define COEF_P2_DEF					0
//
#define COEF_K_MIN					1
#define COEF_K_MAX					INT16U_MAX
#define COEF_K_DEF					1000
//
#define OFFSET_MIN					0
#define OFFSET_MAX					INT16U_MAX
#define OFFSET_DEF					0
//
#define REGULATOR_KP_MIN			0
#define REGULATOR_KP_MAX			INT16U_MAX
#define REGULATOR_KP_DEF			1000
//
#define REGULATOR_KI_MIN			0
#define REGULATOR_KI_MAX			INT16U_MAX
#define REGULATOR_KI_DEF			0
//
#define CURRENT_SETPOINT_MIN		50			// (ְ * 10)
#define CURRENT_SETPOINT_MAX		4500		// (ְ * 10)
//
#define FAN_PERIOD_MIN				0			// סוך
#define FAN_PERIOD_MAX				600			// סוך
#define FAN_PERIOD_DEF				300			// סוך
//
#define FAN_TIME_MIN				0			// סוך
#define FAN_TIME_MAX				600			// סוך
#define FAN_TIME_DEF				60			// סוך
//
#define V_BAT_K_MIN					0
#define V_BAT_K_MAX					INT16U_MAX
#define V_BAT_K_DEF					13400
//
#define CONF_STATE_TIMEOUT_MIN		1000		// לס
#define CONF_STATE_TIMEOUT_MAX		10000		// לס
#define CONF_STATE_TIMEOUT_DEF		5000		// לס

// Types
typedef struct __TableItemConstraint
{
	Int16U Min;
	Int16U Max;
	Int16U Default;
} TableItemConstraint;

// Variables
extern const TableItemConstraint NVConstraint[DATA_TABLE_NV_SIZE];
extern const TableItemConstraint VConstraint[DATA_TABLE_WP_START - DATA_TABLE_WR_START];

#endif // __CONSTRAINTS_H
