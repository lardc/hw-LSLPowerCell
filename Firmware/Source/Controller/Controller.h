#ifndef __CONTROLLER_H
#define __CONTROLLER_H

// Include
//
#include "stdinc.h"
#include "Global.h"

// Types
//
typedef enum __DeviceState
{
	DS_None = 0,
	DS_Fault = 1,
	DS_Disabled = 2,
	DS_Ready = 3,
	DS_ConfigReady = 4,
	DS_InProcess = 5
} DeviceState;

typedef enum __DeviceSubState
{
	SS_None = 0,
	SS_PowerPrepare = 1,
	SS_PulsePrepare = 2,
	SS_WaitAfterPulse = 3,
	SS_Pulse = 4
} DeviceSubState;

// Variables
//
extern volatile Int64U CONTROL_TimeCounter;
extern Int64U CONTROL_LEDTimeout;
extern volatile Int16U CONTROL_Values_Counter;
extern volatile Int16U CONTROL_RegulatorErr_Counter;
extern volatile Int16U CONTROL_ValuesCurrent[VALUES_x_SIZE];
extern volatile Int16U CONTROL_RegulatorErr[VALUES_x_SIZE];


// Functions
//
void CONTROL_Init();
void CONTROL_Idle();
void CONTROL_DelayMs(uint32_t Delay);
void CONTROL_HighPriorityProcess();

#endif // __CONTROLLER_H
