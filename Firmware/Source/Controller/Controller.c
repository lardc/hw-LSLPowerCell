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
#include "InitConfig.h"
#include "math.h"
#include "SaveToFlash.h"
#include "Constraints.h"
#include "JSONDescription.h"

// Types
//
typedef void (*FUNC_AsyncDelegate)();

// Variables
//
volatile DeviceState CONTROL_State = DS_None;
volatile DeviceSubState CONTROL_SubState = SS_None;
static Boolean CycleActive = false;
static Boolean RequestSaveToFlash = false;
//
volatile Int64U CONTROL_TimeCounter = 0;
volatile Int64U	CONTROL_AfterPulsePause = 0;
volatile Int64U	CONTROL_BatteryChargeTimeCounter = 0;
volatile Int64U CONTROL_ConfigStateCounter = 0;
volatile Int16U CONTROL_Values_Counter = 0;
volatile Int16U CONTROL_ExtInfoCounter = 0;
volatile Int16U CONTROL_ValuesCurrent[VALUES_x_SIZE];
volatile Int16U CONTROL_RegulatorErr[VALUES_x_SIZE];
volatile Int16U CONTROL_ValuesBatteryVoltage[VALUES_x_SIZE];
volatile Int16U CONTROL_RegulatorOutput[VALUES_x_SIZE];
volatile Int16U CONTROL_CurentTable[VALUES_x_SIZE];
volatile Int16U CONTROL_DACRawData[VALUES_x_SIZE];
volatile Int16U CONTROL_ExtInfoData[VALUES_EXT_INFO_SIZE];
//
float CONTROL_CurrentMaxValue = 0;
//
volatile RegulatorParamsStruct RegulatorParams;
static FUNC_AsyncDelegate LowPriorityHandle = NULL;

/// Forward functions
//
static Boolean CONTROL_DispatchAction(Int16U ActionID, pInt16U pUserError);
void CONTROL_SwitchToFault(Int16U Reason);
void CONTROL_UpdateWatchDog();
void CONTROL_ResetToDefaultState();
void CONTROL_LogicProcess();
void CONTROL_StopProcess();
void CONTROL_PostPulseSlowSequence();
void CONTROL_ResetOutputRegisters();
void CONTROL_StartPrepare();
void CONTROL_CashVariables();
bool CONTROL_BatteryVoltageCheck();
void CONTROL_SwitchCurrentRangeGain();
void CONTROL_GetMaxCurrentAndDAC();
void CONTROL_InitJSONPointers();
void CONTROL_InitStoragePointers();

// Functions
//
void CONTROL_Init()
{
	// Переменные для конфигурации EndPoint
	Int16U EPIndexes[EP_COUNT] = {EP_CURRENT, EP_BATTERY_VOLTAGE, EP_REGULATOR_OUTPUT, EP_REGULATOR_ERR, EP_CUR_TABLE,
			EP_DAC_RAW_DATA, EP_ExtInfoData};

	Int16U EPSized[EP_COUNT] =
			{VALUES_x_SIZE, VALUES_x_SIZE, VALUES_x_SIZE, VALUES_x_SIZE, VALUES_x_SIZE, VALUES_x_SIZE, VALUES_EXT_INFO_SIZE};

	pInt16U EPCounters[EP_COUNT] = {(pInt16U)&CONTROL_Values_Counter, (pInt16U)&CONTROL_Values_Counter,
			(pInt16U)&CONTROL_Values_Counter, (pInt16U)&CONTROL_Values_Counter, (pInt16U)&CONTROL_Values_Counter,
			(pInt16U)&CONTROL_Values_Counter, (pInt16U)&CONTROL_ExtInfoCounter};

	pInt16U EPDatas[EP_COUNT] = {(pInt16U)CONTROL_ValuesCurrent, (pInt16U)CONTROL_ValuesBatteryVoltage,
			(pInt16U)CONTROL_RegulatorOutput, (pInt16U)CONTROL_RegulatorErr, (pInt16U)CONTROL_CurentTable,
			(pInt16U)CONTROL_DACRawData, (pInt16U)CONTROL_ExtInfoData};

	// Конфигурация сервиса работы DataTable и EEPROM
	EPROMServiceConfig EPROMService = {(FUNC_EPROM_WriteValues)&NFLASH_WriteDT, (FUNC_EPROM_ReadValues)&NFLASH_ReadDT};

	// Инициализация DataTable
	DT_Init(EPROMService, false);

	// Инициализация функций связанных с CAN NodeID
	Int16U NodeID = DataTable[REG_CFG_NODE_ID] ? DataTable[REG_CFG_NODE_ID] : CAN_SLAVE_NID;
	DT_SaveFirmwareInfo(NodeID, 0);
	INITCFG_ConfigCANFilters(NodeID);

	// Инициализация device profile
	DEVPROFILE_Init(&CONTROL_DispatchAction, &CycleActive, NodeID);
	DEVPROFILE_InitEPService(EPIndexes, EPSized, EPCounters, EPDatas);

	// Сброс значений
	DEVPROFILE_ResetControlSection();
	CONTROL_ResetToDefaultState();

	// Инициализация указателей на сохраняемые данные
	CONTROL_InitStoragePointers();
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
	DataTable[REG_RESULT_MAX_DAC] = 0;

	DEVPROFILE_ResetScopes(0);
	DEVPROFILE_ResetEPReadState();
}
//------------------------------------------

void CONTROL_ResetToDefaultState()
{
	CONTROL_ResetOutputRegisters();
	
	LL_LSLCurrentBoardLock(true);
	LL_PowerSupplyEnable(false);

	CONTROL_SetDeviceState(DS_None, SS_None);
}
//------------------------------------------

void CONTROL_Idle()
{
	CONTROL_LogicProcess();

	if (RequestSaveToFlash)
	{
		RequestSaveToFlash = false;
		STF_SaveDiagData();
	}

	DEVPROFILE_ProcessRequests();
	CONTROL_UpdateWatchDog();

	if(LowPriorityHandle)
	{
		LowPriorityHandle();
		LowPriorityHandle = NULL;
	}
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
				LL_PowerSupplyEnable(true);
			}
			else if(CONTROL_State != DS_Ready)
				*pUserError = ERR_OPERATION_BLOCKED;
			break;

		case ACT_DISABLE_POWER:
			if((CONTROL_State == DS_Ready) || ((CONTROL_State == DS_InProcess) && (CONTROL_SubState == SS_PowerPrepare)))
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
				CONTROL_ConfigStateCounter = CONTROL_TimeCounter + DataTable[REG_CONFIG_READY_STATE_TIMEOUT];
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
			CONTROL_SetDeviceState(DS_ConfigReady, SS_None);
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

			if((CONTROL_State == DS_ConfigReady) && (CONTROL_TimeCounter >= CONTROL_ConfigStateCounter))
				CONTROL_SetDeviceState(DS_Ready, SS_None);
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

		if(REGULATOR_Process(&RegulatorParams))
		{
			CONTROL_StopProcess();
			CONTROL_SetDeviceState(DS_InProcess, SS_WaitAfterPulse);
			CONTROL_GetMaxCurrentAndDAC();
			DataTable[REG_OP_RESULT] = OPRESULT_OK;
		}
	}
}
//-----------------------------------------------

void CONTROL_StartPrepare()
{
	MEASURE_DMABufferClear();
	CONTROL_CashVariables();

	CU_LoadConvertParams(CONTROL_GetCurrentRange(), RegulatorParams.CurrentTarget);
	CONTROL_SwitchCurrentRangeGain();

	CONTROL_ConfigPulseShape();
	CONTROL_CopyCurrentToEP();
}
//-----------------------------------------------

Int16U CONTROL_GetCurrentRange()
{
	if((RegulatorParams.CurrentTarget * 10) <= DataTable[REG_CURRENT_THRESHOLD_RANGE0])
		return CURRENT_RANGE_0;
	else if((RegulatorParams.CurrentTarget * 10) <= DataTable[REG_CURRENT_THRESHOLD_RANGE1])
		return CURRENT_RANGE_1;
	else
		return CURRENT_RANGE_2;
}
//-----------------------------------------------

void CONTROL_SwitchCurrentRangeGain()
{
	switch(CONTROL_GetCurrentRange())
	{
		case CURRENT_RANGE_0:
			LL_SetCurrentRange0();
			break;

		case CURRENT_RANGE_1:
			LL_SetCurrentRange1();
			break;

		default:
			LL_SetCurrentRange2();
			break;
	}
}
//-----------------------------------------------

void CONTROL_GetMaxCurrentAndDAC()
{
	// Поиск максимального значения ЦАП
	int i, MaxDACIndex = 0;
	for(i = 1; i < VALUES_x_SIZE; i++)
	{
		if(CONTROL_DACRawData[i] > CONTROL_DACRawData[MaxDACIndex])
			MaxDACIndex = i;
	}
	DataTable[REG_RESULT_MAX_DAC] = CONTROL_DACRawData[MaxDACIndex];

	// Поиск максимального тока
	float MaxCurrent = 0;
	Int16U CurrentCounter = 0, SearchZone = 1;
	for(i = (MaxDACIndex > SearchZone) ? (MaxDACIndex - SearchZone) : 0;
			i < (MaxDACIndex + SearchZone) && i < VALUES_x_SIZE; i++)
	{
		MaxCurrent += CONTROL_ValuesCurrent[i];
		CurrentCounter++;
	}
	DataTable[REG_RESULT_CURRENT] = MaxCurrent * 10 / CurrentCounter;
}
//-----------------------------------------------

void CONTROL_CashVariables()
{
	// Определение целевых значений уставки
	CONTROL_CurrentMaxValue = (float)DataTable[REG_CURRENT_PER_CURBOARD] / 10 * DataTable[REG_CURBOARD_QUANTITY];
	RegulatorParams.CurrentTarget = ((float)DataTable[REG_CURRENT_PULSE_VALUE]
			+ (float)(DataTable[REG_CFG_HIGH_CURRENT_SET] ? DataTable[REG_CURRENT_PULSE_VALUE_ADD] : 0)) / 10;

	if(RegulatorParams.CurrentTarget > CONTROL_CurrentMaxValue)
		RegulatorParams.CurrentTarget = CONTROL_CurrentMaxValue;

	// Кеширование коэффициентов регулятора
	Int16U CurrentRange = CONTROL_GetCurrentRange();

	RegulatorParams.Kp = (float)DataTable[REG_REGULATOR_RANGE0_Kp + CurrentRange * 2] / 1000;
	RegulatorParams.Ki = (float)DataTable[REG_REGULATOR_RANGE0_Ki + CurrentRange * 2] / 1000;

	RegulatorParams.RegulatorAlowedError = DataTable[REG_REGULATOR_ALLOWED_ERR];
	RegulatorParams.FollowingErrorCounterMax = DataTable[REG_FOLLOWING_ERR_CNT];

	RegulatorParams.KiTune = (CONTROL_CurrentMaxValue - RegulatorParams.CurrentTarget)
			* (float)DataTable[REG_REGULATOR_TF_Ki_RANG0 + CurrentRange] / 1e6;

	// Загрузка параметров ЦАП
	RegulatorParams.DebugMode = false;
	RegulatorParams.DACOffset = DataTable[REG_DAC_OFFSET];
	RegulatorParams.DACLimitValue = (DAC_MAX_VAL > DataTable[REG_DAC_OUTPUT_LIMIT_VALUE]) ? \
			DataTable[REG_DAC_OUTPUT_LIMIT_VALUE] : DAC_MAX_VAL;
}
//-----------------------------------------------

void CONTROL_PrepareForDebug(Int16U DACValue)
{
	if(DACValue > DAC_MAX_VAL)
		DACValue = DAC_MAX_VAL;

	RegulatorParams.DebugMode = true;
	RegulatorParams.CurrentTarget = DACValue;
	RegulatorParams.DACOffset = DataTable[REG_DAC_OFFSET];
}
//-----------------------------------------------

void CONTROL_ConfigPulseShape()
{
	int i;
	for(i = 0; i < PULSE_BUFFER_SIZE; ++i)
	{
		float Setpoint = RegulatorParams.CurrentTarget * sinf(PI * i / ((CURRENT_PULSE_WIDTH / TIMER15_uS) - 1));
		RegulatorParams.CurrentTable[i] = (Setpoint > 0) ? Setpoint : 0;
	}

	if(DataTable[REG_USE_LINEAR_DOWN])
	{
		float StartCurrent = CURRENT_TAIL_START_CURR;
		float StopCurrent = -CURRENT_TAIL_START_CURR;

		Int16U TopIndex = CURRENT_PULSE_WIDTH / TIMER15_uS / 2;

		// Поиск стартового индекса
		Int16U StartIndex = TopIndex;
		for (i = TopIndex; i < PULSE_BUFFER_SIZE; ++i)
		{
			if (RegulatorParams.CurrentTable[i] < StartCurrent)
			{
				StartIndex = i;
				break;
			}
		}

		// Дописываем плавно спадающий хвост
		float DecreaseStep = (StartCurrent - StopCurrent) / (PULSE_BUFFER_SIZE - StartIndex);
		for (i = StartIndex; i < PULSE_BUFFER_SIZE; ++i)
		{
			StartCurrent -= DecreaseStep;
			RegulatorParams.CurrentTable[i] = StartCurrent;
		}
	}
}
//-----------------------------------------------

void CONTROL_CopyCurrentToEP()
{
	for(int i = 0; i < PULSE_BUFFER_SIZE; ++i)
		CONTROL_CurentTable[i] = (Int16S)RegulatorParams.CurrentTable[i];
}
//-----------------------------------------------

void CONTROL_StopProcess()
{
	TIM_Stop(TIM15);
	LowPriorityHandle = &CONTROL_PostPulseSlowSequence;

	float AfterPulseCoefficient = RegulatorParams.CurrentTarget / CONTROL_CurrentMaxValue;
	CONTROL_AfterPulsePause = CONTROL_TimeCounter + DataTable[REG_AFTER_PULSE_PAUSE] * AfterPulseCoefficient;
	CONTROL_BatteryChargeTimeCounter = CONTROL_TimeCounter + DataTable[REG_BATTERY_RECHARGE_TIMEOUT];
}
//------------------------------------------

void CONTROL_PostPulseSlowSequence()
{
	LL_WriteDAC(0);
	LL_LSLCurrentBoardLock(true);
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
	CONTROL_HandleFanLogic(true);

	LL_LSLCurrentBoardLock(false);
	TIM_Reset(TIM15);
	TIM_Start(TIM15);
}
//-----------------------------------------------

void CONTROL_HandleFanLogic(bool IsImpulse)
{
	static uint32_t IncrementCounter = 0;
	static uint64_t FanOnTimeout = 0;

	if(DataTable[REG_FAN_CTRL])
	{
		// Увеличение счётчика в простое
		if (!IsImpulse)
			IncrementCounter++;

		// Включение вентилятора
		if ((IncrementCounter > ((uint32_t)DataTable[REG_FAN_OPERATE_PERIOD] * 1000)) || IsImpulse)
		{
			IncrementCounter = 0;
			FanOnTimeout = CONTROL_TimeCounter + ((uint32_t)DataTable[REG_FAN_OPERATE_TIME] * 1000);
			LL_Fan(true);
		}

		// Отключение вентилятора
		if (FanOnTimeout && (CONTROL_TimeCounter > FanOnTimeout))
		{
			FanOnTimeout = 0;
			LL_Fan(false);
		}
	}
	else
		LL_Fan(false);
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

void CONTROL_UpdateWatchDog()
{
	if(BOOT_LOADER_VARIABLE != BOOT_LOADER_REQUEST)
		IWDG_Refresh();
}
//------------------------------------------

void CONTROL_InitStoragePointers()
{
	STF_AssignPointer(0, (Int32U)&DataTable[REG_CURRENT_PULSE_VALUE]);

	STF_AssignPointer(1, (Int32U)&DataTable[REG_DEV_STATE]);
	STF_AssignPointer(2, (Int32U)&DataTable[REG_FAULT_REASON]);
	STF_AssignPointer(3, (Int32U)&DataTable[REG_DISABLE_REASON]);
	STF_AssignPointer(4, (Int32U)&DataTable[REG_WARNING]);
	STF_AssignPointer(5, (Int32U)&DataTable[REG_PROBLEM]);
	STF_AssignPointer(6, (Int32U)&DataTable[REG_OP_RESULT]);
	STF_AssignPointer(7, (Int32U)&DataTable[REG_SUB_STATE]);

	STF_AssignPointer(8, (Int32U)&DataTable[REG_RESULT_CURRENT]);
	STF_AssignPointer(9, (Int32U)&DataTable[REG_BATTERY_VOLTAGE]);

	STF_AssignPointer(10, (Int32U)CONTROL_ValuesBatteryVoltage);
	STF_AssignPointer(11, (Int32U)CONTROL_ValuesCurrent);
	STF_AssignPointer(12, (Int32U)CONTROL_RegulatorOutput);
	STF_AssignPointer(13, (Int32U)CONTROL_RegulatorErr);
	STF_AssignPointer(14, (Int32U)CONTROL_CurentTable);
	STF_AssignPointer(15, (Int32U)CONTROL_DACRawData);
	STF_AssignPointer(16, (Int32U)&CONTROL_Values_Counter);
}
//------------------------------------------

void CONTROL_InitJSONPointers()
{
	Itm1Min = DataTable[REG_CFG_CURRENT_LIMIT_MIN] ? DataTable[REG_CFG_CURRENT_LIMIT_MIN] : CURRENT_SETPOINT_MIN;
	Itm1Max = DataTable[REG_I2DAC_CUST_RANGE0];

	Itm2Min = DataTable[REG_I2DAC_CUST_RANGE0];
	Itm2Max = DataTable[REG_I2DAC_CUST_RANGE1];

	Itm3Min = DataTable[REG_I2DAC_CUST_RANGE1];
	Itm3Max = DataTable[REG_CFG_CURRENT_LIMIT_MAX] ? DataTable[REG_CFG_CURRENT_LIMIT_MAX] : CURRENT_SETPOINT_MAX;

	JSON_AssignPointer(0, &Itm1Min);
	JSON_AssignPointer(1, &Itm1Max);

	JSON_AssignPointer(2, &Itm2Min);
	JSON_AssignPointer(3, &Itm2Max);

	JSON_AssignPointer(4, &Itm3Min);
	JSON_AssignPointer(5, &Itm3Max);
}
//------------------------------------------
