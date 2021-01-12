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
#include "math.h"

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
volatile Int64U	CONTROL_BatteryChargeTimeCounter = 0;
volatile Int16U CONTROL_Values_Counter = 0;
volatile Int16U CONTROL_ValuesCurrent[VALUES_x_SIZE];
volatile Int16U CONTROL_RegulatorErr[VALUES_x_SIZE];
volatile Int16U CONTROL_ValuesBatteryVoltage[VALUES_x_SIZE];
volatile Int16U CONTROL_RegulatorOutput[VALUES_x_SIZE];
//
float 	CONTROL_CurrentMaxValue = 0;
//
volatile RegulatorParamsStruct RegulatorParams;

/// Forward functions
//
static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U pUserError);
void CONTROL_SwitchToFault(Int16U Reason);
void CONTROL_DelayMs(uint32_t Delay);
void CONTROL_UpdateWatchDog();
void CONTROL_ResetToDefaultState();
void CONTROL_LogicProcess();
void CONTROL_StopProcess();
void CONTROL_ResetOutputRegisters();
bool CONTROL_RegulatorCycle(volatile RegulatorParamsStruct* Regulator);
void CONTROL_StartPrepare();
void CONTROL_CashVariables();
bool CONTROL_BatteryVoltageCheck();

// Functions
//
void CONTROL_Init()
{
	// Переменные для конфигурации EndPoint
	Int16U EPIndexes[EP_COUNT] = {EP_CURRENT, EP_BATTERY_VOLTAGE, EP_REGULATOR_OUTPUT, EP_REGULATOR_ERR};
	Int16U EPSized[EP_COUNT] = {VALUES_x_SIZE, VALUES_x_SIZE, VALUES_x_SIZE, VALUES_x_SIZE};
	pInt16U EPCounters[EP_COUNT] = {(pInt16U)&CONTROL_Values_Counter, (pInt16U)&CONTROL_Values_Counter, (pInt16U)&CONTROL_Values_Counter, (pInt16U)&CONTROL_Values_Counter};
	pInt16U EPDatas[EP_COUNT] = {(pInt16U)CONTROL_ValuesCurrent, (pInt16U)CONTROL_ValuesBatteryVoltage, (pInt16U)CONTROL_RegulatorOutput, (pInt16U)CONTROL_RegulatorErr};

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
			{
				CONTROL_BatteryChargeTimeCounter = CONTROL_TimeCounter + DataTable[REG_BATTERY_FULL_CHRAGE_TIMEOUT];
				CONTROL_SetDeviceState(DS_InProcess, SS_PowerPrepare);
			}
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
				CONTROL_SetDeviceState(DS_InProcess, SS_Pulse);
				CONTROL_StartProcess();
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
				CONTROL_SetDeviceState(DS_Ready, SS_None);
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
	switch(CONTROL_SubState)
	{
		case SS_PowerPrepare:
			if(CONTROL_BatteryVoltageCheck())
				CONTROL_SetDeviceState(DS_Ready, SS_None);
			else
			{
				if(CONTROL_TimeCounter >= CONTROL_BatteryChargeTimeCounter)
				{
					CONTROL_ResetToDefaultState();
					CONTROL_SwitchToFault(PROBLEM_BATTERY);
				}
			}
			break;

		case SS_PulsePrepare:
			CONTROL_StartPrepare();
			break;

		case SS_WaitAfterPulse:
			if(CONTROL_TimeCounter > CONTROL_AfterPulsePause)
			{
				if(CONTROL_BatteryVoltageCheck())
					CONTROL_SetDeviceState(DS_Ready, SS_None);
				else
				{
					if(CONTROL_TimeCounter >= CONTROL_BatteryChargeTimeCounter)
					{
						CONTROL_ResetToDefaultState();
						CONTROL_SwitchToFault(PROBLEM_BATTERY);
					}
				}
			}
			break;

		default:
			CONTROL_BatteryVoltageCheck();
			break;
	}
}
//-----------------------------------------------

bool CONTROL_BatteryVoltageCheck()
{
	DataTable[REG_BATTERY_VOLTAGE] = (Int16U)(MEASURE_SingleSampleBatteryVoltage() * 10);

	if(DataTable[REG_BATTERY_VOLTAGE] < DataTable[REG_BATTERY_VOLTAGE_THRESHOLD])
		return false;
	else
		return true;
}
//-----------------------------------------------

void CONTROL_HighPriorityProcess()
{
	if(CONTROL_SubState == SS_Pulse)
	{
		MEASURE_SampleParams(&RegulatorParams);

		if(CONTROL_RegulatorCycle(&RegulatorParams))
		{
			CONTROL_StopProcess();
			CONTROL_SetDeviceState(DS_InProcess, SS_WaitAfterPulse);
		}
	}
}
//-----------------------------------------------

bool CONTROL_RegulatorCycle(volatile RegulatorParamsStruct* Regulator)
{
	return REGULATOR_Process(Regulator);
}
//-----------------------------------------------

void CONTROL_StartPrepare()
{
	MEASURE_DMABufferClear();
	CU_LoadConvertParams();
	REGULATOR_CashVariables(&RegulatorParams);
	CONTROL_CashVariables();
	CONTROL_SineConfig(&RegulatorParams);

	MEASURE_SetCurrentRange(&RegulatorParams);

	CONTROL_SetDeviceState(DS_ConfigReady, SS_None);
}
//-----------------------------------------------

void CONTROL_CashVariables()
{
	RegulatorParams.CurrentTarget = (float)DataTable[REG_CURRENT_PULSE_VALUE] / 10;

	CONTROL_CurrentMaxValue = (float)DataTable[REG_CURRENT_PER_CURBOARD] / 10 * DataTable[REG_CURBOARD_QUANTITY];
	if(RegulatorParams.CurrentTarget > CONTROL_CurrentMaxValue)
		RegulatorParams.CurrentTarget = CONTROL_CurrentMaxValue;
}
//-----------------------------------------------

void CONTROL_SineConfig(volatile RegulatorParamsStruct* Regulator)
{
	for(int i = 0; i < PULSE_BUFFER_SIZE; ++i)
		Regulator->CurrentTable[i] = Regulator->CurrentTarget * sin(PI * i / (PULSE_BUFFER_SIZE - 1));
}
//-----------------------------------------------

void CONTROL_StopProcess()
{
	float AfterPulseCoefficient;

	LL_WriteDAC(0);
	LL_SetStateLineSync(true);
	TIM_Stop(TIM15);

	AfterPulseCoefficient = (float)DataTable[REG_CURRENT_PULSE_VALUE] / CONTROL_CurrentMaxValue;
	CONTROL_AfterPulsePause = CONTROL_TimeCounter + DataTable[REG_AFTER_PULSE_PAUSE] * AfterPulseCoefficient;
	CONTROL_BatteryChargeTimeCounter = CONTROL_TimeCounter + DataTable[REG_BATTERY_RECHARGE_TIMEOUT];
}
//------------------------------------------

void CONTROL_ExternalInterruptProcess()
{
	if (CONTROL_State == DS_ConfigReady)
	{
		CONTROL_SetDeviceState(DS_InProcess, SS_Pulse);

		CONTROL_StartProcess();
	}
}
//------------------------------------------

void CONTROL_StartProcess()
{
	LL_SetStateLineSync(false);
	TIM_Reset(TIM15);
	TIM_Start(TIM15);
}
//-----------------------------------------------

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

