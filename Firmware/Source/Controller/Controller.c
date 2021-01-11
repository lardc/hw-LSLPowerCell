// Header
#include "Controller.h"
//
// Includes
#include "Board.h"
#include "Delay.h"
#include "DeviceProfile.h"
#include "Interrupts.h"
#include "LowLevel.h"
#include "SysConfig.h"
#include "DebugActions.h"
#include "Diagnostic.h"
#include "BCCIxParams.h"
#include "Measurement.h"

// Types
//
typedef void (*FUNC_AsyncDelegate)();

// Variables
//
volatile DeviceState CONTROL_State = DS_None;
volatile DeviceSubState CONTROL_SubState = SS_None;
static Boolean CycleActive = false;
//
volatile Int64U CONTROL_TimeCounter = 0;
volatile Int64U	CONTROL_AfterPulsePause = 0;
volatile Int16U CONTROL_Values_Counter = 0;
volatile Int16U CONTROL_RegulatorErr_Counter = 0;
volatile Int16U CONTROL_ValuesCurrent[VALUES_x_SIZE];
volatile Int16U CONTROL_RegulatorErr[VALUES_x_SIZE];
//
volatile MeasureSample SampleParams;

/// Forward functions
//
static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U pUserError);
void CONTROL_SetDeviceState(DeviceState NewState, DeviceSubState NewSubState);
void CONTROL_SwitchToFault(Int16U Reason);
void CONTROL_DelayMs(uint32_t Delay);
void CONTROL_UpdateWatchDog();
void CONTROL_ResetToDefaultState();
void CONTROL_LogicProcess();
void CONTROL_StopProcess();
void CONTROL_StartProcess();
void CONTROL_ResetOutputRegisters();
void CONTROL_SaveTestResult(bool ExcessCurrent, Int16U Problem);
bool CONTROL_RegulatorCycle(float SampleCurrent);

// Functions
//
void CONTROL_Init()
{
	// Переменные для конфигурации EndPoint
	Int16U EPIndexes[EP_COUNT] = {EP_CURRENT, EP_REGULATOR_ERR};
	Int16U EPSized[EP_COUNT] = {VALUES_x_SIZE, VALUES_x_SIZE};
	pInt16U EPCounters[EP_COUNT] = {(pInt16U)&CONTROL_Values_Counter, (pInt16U)&CONTROL_RegulatorErr_Counter};
	pInt16U EPDatas[EP_COUNT] = {(pInt16U)CONTROL_ValuesCurrent, (pInt16U)CONTROL_RegulatorErr};

	// Конфигурация сервиса работы Data-table и EPROM
	EPROMServiceConfig EPROMService = {(FUNC_EPROM_WriteValues)&NFLASH_WriteDT, (FUNC_EPROM_ReadValues)&NFLASH_ReadDT};
	// Инициализация data table
	DT_Init(EPROMService, false);
	DT_SaveFirmwareInfo(CAN_SALVE_NID, 0);
	// Инициализация device profile
	DEVPROFILE_Init(&CONTROL_DispatchAction, &CycleActive);
	DEVPROFILE_InitEPService(EPIndexes, EPSized, EPCounters, EPDatas);
	// Сброс значений
	DEVPROFILE_ResetControlSection();
	CONTROL_ResetToDefaultState();
}
//------------------------------------------

void CONTROL_ResetOutputRegisters()
{
	DataTable[REG_FAULT_REASON] = DF_NONE;
	DataTable[REG_DISABLE_REASON] = DF_NONE;
	DataTable[REG_WARNING] = WARNING_NONE;
	DataTable[REG_PROBLEM] = PROBLEM_NONE;
	DataTable[REG_OP_RESULT] = OPRESULT_NONE;
	
	DataTable[REG_RESULT_CURRENT] = 0;

	DEVPROFILE_ResetScopes(0);
	DEVPROFILE_ResetEPReadState();
}
//------------------------------------------

void CONTROL_ResetToDefaultState()
{
	CONTROL_ResetOutputRegisters();
	
	LL_SetStateLineSync(true);
	LL_PowerSupplyEnable(false);

	CONTROL_SetDeviceState(DS_None, SS_None);
}
//------------------------------------------

void CONTROL_Idle()
{
	CONTROL_LogicProcess();

	DEVPROFILE_ProcessRequests();
	CONTROL_UpdateWatchDog();
}
//------------------------------------------

static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U pUserError)
{
	*pUserError = ERR_NONE;
	
	switch (ActionID)
	{
		case ACT_ENABLE_POWER:
			if(CONTROL_State == DS_None)
				CONTROL_SetDeviceState(DS_InProcess, SS_PowerPrepare);
			else if(CONTROL_State != DS_Ready)
				*pUserError = ERR_OPERATION_BLOCKED;
			break;

		case ACT_DISABLE_POWER:
			if(CONTROL_State == DS_Ready)
			{
				CONTROL_ResetToDefaultState();
			}
			else if(CONTROL_State != DS_None)
					*pUserError = ERR_OPERATION_BLOCKED;
			break;

		case ACT_CONFIG_UNIT:
			if (CONTROL_State == DS_Ready)
			{
				CONTROL_ResetOutputRegisters();
				CONTROL_SetDeviceState(DS_InProcess, SS_PulsePrepare);
			}
			else
				if (CONTROL_State == DS_InProcess)
					*pUserError = ERR_OPERATION_BLOCKED;
				else
					*pUserError = ERR_DEVICE_NOT_READY;
			break;

		case ACT_SOFTWARE_START:
			if (CONTROL_State == DS_ConfigReady)
			{
				CONTROL_SoftwareStartProcess();
			}
			else
				if (CONTROL_State == DS_InProcess)
					*pUserError = ERR_OPERATION_BLOCKED;
				else
					*pUserError = ERR_DEVICE_NOT_READY;
			break;

		case ACT_STOP_PROCESS:
			if (CONTROL_State == DS_InProcess)
			{
				CONTROL_StopProcess();
			}
			break;

		case ACT_CLR_FAULT:
			if (CONTROL_State == DS_Fault)
			{
				CONTROL_SetDeviceState(DS_None, SS_None);
				DataTable[REG_FAULT_REASON] = DF_NONE;
			}
			break;

		case ACT_CLR_WARNING:
			DataTable[REG_WARNING] = WARNING_NONE;
			break;

		default:
			return DIAG_HandleDiagnosticAction(ActionID, pUserError);
			
	}
	return true;
}
//-----------------------------------------------

void CONTROL_LogicProcess()
{
	static Int64U DelayCounter = 0;

	if(CONTROL_State == DS_InProcess)
	{
		switch(CONTROL_SubState)
		{
			case SS_PowerPrepare:
				CONTROL_BatteryVoltageMeasurement(CONTROL_SubState);
				break;

			case SS_PulsePrepare:
				CONTROL_StartPrepare();
				CONTROL_SetDeviceState(DS_ConfigReady, SS_None);
				break;

			case SS_WaitAfterPulse:
				CONTROL_SetDeviceState(DS_Ready, SS_None);
				break;

			case SS_Pulse:
				break;

			default:
				break;
		}
	}
}
//-----------------------------------------------

void CONTROL_HighPriorityProcess()
{
	if(CONTROL_SubState == SS_Pulse)
	{
		MEASURE_SampleCurrent(&SampleParams.Current);

		if(CONTROL_RegulatorCycle(SampleParams.Current))
			CONTROL_StopProcess();
	}
}
//-----------------------------------------------

bool CONTROL_RegulatorCycle(float SampleCurrent)
{

}
//-----------------------------------------------

void CONTROL_StartProcess()
{
	MEASURE_DMABuffersClear();
	TIM_Start(TIM6);
}
//-----------------------------------------------

void CONTROL_StopProcess()
{
	float AfterPulseCoefficient, CurrentMaxValue;

	LL_WriteDAC(0);
	LL_SetStateLineSync(true);

	CurrentMaxValue = DataTable[REG_CURRENT_PER_CURBOARD] * DataTable[REG_CURBOARD_QUANTITY];
	AfterPulseCoefficient = (float)DataTable[REG_CURRENT_PULSE_VALUE] / CurrentMaxValue;
	CONTROL_AfterPulsePause = CONTROL_TimeCounter + DataTable[REG_AFTER_PULSE_PAUSE] * AfterPulseCoefficient;

	CONTROL_SetDeviceState(DS_Ready, SS_None);
}
//------------------------------------------

void CONTROL_SwitchToFault(Int16U Reason)
{
	CONTROL_SetDeviceState(DS_Fault, SS_None);
	DataTable[REG_FAULT_REASON] = Reason;
}
//------------------------------------------

void CONTROL_SetDeviceState(DeviceState NewState, DeviceSubState NewSubState)
{
	CONTROL_State = NewState;
	CONTROL_SubState = NewSubState;
	DataTable[REG_DEV_STATE] = NewState;
	DataTable[REG_SUB_STATE] = NewSubState;
}
//------------------------------------------

void CONTROL_DelayMs(uint32_t Delay)
{
	uint64_t Counter = (uint64_t)CONTROL_TimeCounter + Delay;
	while(Counter > CONTROL_TimeCounter)
		CONTROL_UpdateWatchDog();
}
//------------------------------------------

void CONTROL_UpdateWatchDog()
{
	if(BOOT_LOADER_VARIABLE != BOOT_LOADER_REQUEST)
		IWDG_Refresh();
}
//------------------------------------------

