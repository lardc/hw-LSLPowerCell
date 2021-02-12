#ifndef __DEV_OBJ_DIC_H
#define __DEV_OBJ_DIC_H

// �������
#define ACT_ENABLE_POWER				1	// ��������� �����
#define ACT_DISABLE_POWER				2	// ���������� �����
#define ACT_CLR_FAULT					3	// ������� ���� fault
#define ACT_CLR_WARNING					4	// ������� ���� warning

#define ACT_DBG_POWER_SUPPLY_CTRL		50	// �������� ������ SCRelayBoard
#define ACT_DBG_PULSE					51	// ������������ ������������ �������� �� �������� ������������
#define ACT_DBG_SET_CURRENT_RANGE		52	// ��������� ��������� �� ���� (0, 1, 2)
#define ACT_DBG_FAN_CTRL				53	// �������� ������ �����������

#define ACT_CONFIG_UNIT					100	// ������������ �����
#define ACT_SOFTWARE_START				101	// ����� ���������
#define ACT_STOP_PROCESS				102	// ���� ���������

#define ACT_SAVE_TO_ROM					200	// ���������� ���������������� ������ �� FLASH ����������
#define ACT_RESTORE_FROM_ROM			201	// �������������� ������ �� FLASH
#define ACT_RESET_TO_DEFAULT			202	// ����� DataTable � ��������� �� ���������

#define ACT_BOOT_LOADER_REQUEST			320	// ���������� ���������� � ����� ��������������������
// -----------------------------

// ��������
// ����������� ��������
#define REG_CURRENT_THRESHOLD_RANGE0	0	// ������� ����� �� ���� ��������� 0 (� * 10)
#define REG_CURRENT_THRESHOLD_RANGE1	1	// ������� ����� �� ���� ��������� 1 (� * 10)
#define REG_BATTERY_VOLTAGE_THRESHOLD	2	// ����� ������ ������� ������������� (� * 10)
#define REG_DAC_OUTPUT_LIMIT_VALUE		3	// ����������� ������ ��� (0 - 4095)
#define REG_BATTERY_FULL_CHRAGE_TIMEOUT	4	// ����� �������� ������� ������ ������������� (��)
#define REG_BATTERY_RECHARGE_TIMEOUT	5	// ����� �������� �������� ����� �������� (��)
#define REG_SHUNT_RESISTANCE			6	// ������������� ����� (����)
#define REG_REGULATOR_QI_MAX			7	// ����������� ������ ������������ ������������
#define REG_FAN_CTRL					8	// ��������� ���������� ������������
#define REG_AFTER_PULSE_PAUSE			9	// ����� ����� �������� (��)
#define REG_CURRENT_PER_CURBOARD		10	// ������������ ��������� ���� � ����� CurrentBoard (� * 10)
#define REG_CURBOARD_QUANTITY			11	// ���������� CurrentBoard
#define REG_SCOPE_STEP					12	// ��� ���������� ������������ ��������
#define REG_FAN_OPERATE_PERIOD			13	// ������ ������ ����������� (���)
#define REG_FAN_OPERATE_TIME			14	// ����� ����������� ��������� ����������� (���)
#define REG_DAC_OFFSET					15	// �������� ������� � ���
#define REG_CONFIG_READY_STATE_TIMEOUT	16	// ������� ���������� ����� � ��������� DS_ConfigReady
// 17 - 19
#define REG_I_TO_DAC_RANGE0_K			20	// �������� 0 - ����������� ��������� � �������� ���
#define REG_I_TO_DAC_RANGE0_B			21	// �������� 0 - �������� ��������� � �������� ���
#define REG_I_TO_DAC_RANGE1_K			22	// �������� 1 - ����������� ��������� � �������� ���
#define REG_I_TO_DAC_RANGE1_B			23	// �������� 1 - �������� ��������� � �������� ���
#define REG_I_TO_DAC_RANGE2_K			24	// �������� 2 - ����������� ��������� � �������� ���
#define REG_I_TO_DAC_RANGE2_B			25	// �������� 2 - �������� ��������� � �������� ���
//
#define REG_ADC_VOLTAGE_K				26	// ����������� ������ ���������� K
#define REG_ADC_VOLTAGE_B				27	// �������� ������ ���������� B
//
#define REG_K_AMP_RANGE0				28	// ����������� �������� ���� � ������ 0 (*100)
#define REG_K_AMP_RANGE1				29	// ����������� �������� ���� � ������ 1 (*100)
#define REG_K_AMP_RANGE2				30	// ����������� �������� ���� � ������ 2 (*100)
//
#define REG_ADC_I_RANGE0_P2				31	// �������� 0 - ����������� ������ ���������� �2 �1�6
#define REG_ADC_I_RANGE0_P1				32	// �������� 0 - ����������� ������ ���������� �1 x1000
#define REG_ADC_I_RANGE0_P0				33	// �������� 0 - �������� ������ ���������� �0
#define REG_ADC_I_RANGE0_N				34	// �������� 0 - ��������� ������������ ������ ����������
#define REG_ADC_I_RANGE0_D				35	// �������� 0 - ����������� ������������ ������ ����������
#define REG_ADC_I_RANGE0_B				36	// �������� 0 - �������� ������ ���������� B
//
#define REG_ADC_I_RANGE1_P2				37	// �������� 1 - ����������� ������ ���������� �2 �1�6
#define REG_ADC_I_RANGE1_P1				38	// �������� 1 - ����������� ������ ���������� �1 x1000
#define REG_ADC_I_RANGE1_P0				39	// �������� 1 - �������� ������ ���������� �0
#define REG_ADC_I_RANGE1_N				40	// �������� 1 - ��������� ������������ ������ ����������
#define REG_ADC_I_RANGE1_D				41	// �������� 1 - ����������� ������������ ������ ����������
#define REG_ADC_I_RANGE1_B				42	// �������� 1 - �������� ������ ���������� B
//
#define REG_ADC_I_RANGE2_P2				43	// �������� 2 - ����������� ������ ���������� �2 �1�6
#define REG_ADC_I_RANGE2_P1				44	// �������� 2 - ����������� ������ ���������� �1 x1000
#define REG_ADC_I_RANGE2_P0				45	// �������� 2 - �������� ������ ���������� �0
#define REG_ADC_I_RANGE2_N				46	// �������� 2 - ��������� ������������ ������ ����������
#define REG_ADC_I_RANGE2_D				47	// �������� 2 - ����������� ������������ ������ ����������
#define REG_ADC_I_RANGE2_B				48	// �������� 2 - �������� ������ ���������� B
//
#define REG_REGULATOR_RANGE0_Kp			49	// �������� 0 - ���������������� ����������� ����������
#define REG_REGULATOR_RANGE0_Ki			50	// �������� 0 - ������������ ����������� ����������
#define REG_REGULATOR_RANGE1_Kp			51	// �������� 1 - ���������������� ����������� ����������
#define REG_REGULATOR_RANGE1_Ki			52	// �������� 1 - ������������ ����������� ����������
#define REG_REGULATOR_RANGE2_Kp			53	// �������� 2 - ���������������� ����������� ����������
#define REG_REGULATOR_RANGE2_Ki			54	// �������� 2 - ������������ ����������� ����������
//
#define REG_REGULATOR_TF_Ki_RANG0		55	// �������� 0 - ����������� ���������� �������� Ki �� ���� (� ��. (dKi / dI) * 1000)
#define REG_REGULATOR_TF_Ki_RANG1		56	// �������� 1 - ����������� ���������� �������� Ki �� ���� (� ��. (dKi / dI) * 1000)
#define REG_REGULATOR_TF_Ki_RANG2		57	// �������� 2 - ����������� ���������� �������� Ki �� ���� (� ��. (dKi / dI) * 1000)

#define REG_CURRENT_PULSE_VALUE			128	// ������� ��������� �������� ���� (� * 10)

// ������������ �������� ������-������
#define REG_DBG							150	// ������� ������ �������

// �������� ������ ������
#define REG_DEV_STATE					192	// ������� ���������
#define REG_FAULT_REASON				193	// ������� Fault
#define REG_DISABLE_REASON				194	// ������� Disable
#define REG_WARNING						195	// ������� Warning
#define REG_PROBLEM						196	// ������� Problem
#define REG_OP_RESULT					197	// ������� ���������� ��������
#define REG_SUB_STATE					198	// ������� ���������������� ���������

#define REG_RESULT_CURRENT				200	// ����������� �������� �������� ���� (� * 10)
#define REG_BATTERY_VOLTAGE				201	// ���������� �� ������� ������������� (� * 10)
// -----------------------------
#define REG_FWINFO_SLAVE_NID			256	// Device CAN slave node ID
#define REG_FWINFO_MASTER_NID			257	// Device CAN master node ID (if presented)
// 258 - 259
#define REG_FWINFO_STR_LEN				260	// Length of the information string record
#define REG_FWINFO_STR_BEGIN			261	// Begining of the information string record


// Operation results
#define OPRESULT_NONE					0	// No information or not finished
#define OPRESULT_OK						1	// Operation was successful
#define OPRESULT_FAIL					2	// Operation failed

//  Fault and disable codes
#define DF_NONE							0

// Problem
#define PROBLEM_NONE					0
#define PROBLEM_BATTERY					1

//  Warning
#define WARNING_NONE					0

//  User Errors
#define ERR_NONE						0
#define ERR_CONFIGURATION_LOCKED		1	//  ���������� �������� �� ������
#define ERR_OPERATION_BLOCKED			2	//  �������� �� ����� ���� ��������� � ������� ��������� ����������
#define ERR_DEVICE_NOT_READY			3	//  ���������� �� ������ ��� ����� ���������
#define ERR_WRONG_PWD					4	//  ������������ ����

// Endpoints
#define EP_CURRENT						1
#define	EP_BATTERY_VOLTAGE				2
#define EP_REGULATOR_OUTPUT				3
#define EP_REGULATOR_ERR				4

#endif //  __DEV_OBJ_DIC_H
