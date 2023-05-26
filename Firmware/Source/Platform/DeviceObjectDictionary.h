﻿#ifndef __DEV_OBJ_DIC_H
#define __DEV_OBJ_DIC_H

// Команды
#define ACT_ENABLE_POWER				1	// Включение блока
#define ACT_DISABLE_POWER				2	// Выключение блока
#define ACT_CLR_FAULT					3	// Очистка всех fault
#define ACT_CLR_WARNING					4	// Очистка всех warning

#define ACT_DBG_POWER_SUPPLY_CTRL		50	// Проверка работы SCRelayBoard
#define ACT_DBG_PULSE					51	// Формирование управляющего импульса на затворах транзисторов
#define ACT_DBG_SET_CURRENT_RANGE		52	// Установка диапазона по току (0, 1, 2)
#define ACT_DBG_FAN_CTRL				53	// Проверка работы вентилятора

#define ACT_CONFIG_UNIT					100	// Конфигурация блока
#define ACT_SOFTWARE_START				101	// Старт измерения
#define ACT_STOP_PROCESS				102	// Стоп измерения

#define ACT_SAVE_TO_ROM					200	// Сохранение пользовательских данных во FLASH процессора
#define ACT_RESTORE_FROM_ROM			201	// Восстановление данных из FLASH
#define ACT_RESET_TO_DEFAULT			202	// Сброс DataTable в состояние по умолчанию

#define ACT_BOOT_LOADER_REQUEST			320	// Перезапуск процессора с целью перепрограммирования
#define ACT_WRITE_LABEL1				321	// Записать первую метку: LSLPowerCell v.1.2
#define ACT_READ_SYMBOL					330	// Выполнить чтение символа из памяти
#define ACT_SELECT_MEM_LABEL			331	// Переместить указатель считывания в область метки
// -----------------------------

// Регистры
// Сохраняемые регистры
#define REG_CURRENT_THRESHOLD_RANGE0	0	// Верхний порог по току диапазона 0 (А * 10)
#define REG_CURRENT_THRESHOLD_RANGE1	1	// Верхний порог по току диапазона 1 (А * 10)
#define REG_BATTERY_VOLTAGE_THRESHOLD	2	// Порог заряда батареи конденсаторов (В * 10)
#define REG_DAC_OUTPUT_LIMIT_VALUE		3	// Ограничение выхода ЦАП (0 - 4095)
#define REG_BATTERY_FULL_CHRAGE_TIMEOUT	4	// Время таймаута полного заряда конденсаторов (мс)
#define REG_BATTERY_RECHARGE_TIMEOUT	5	// Время таймаута дозаряда после импульса (мс)
#define REG_SHUNT_RESISTANCE			6	// Сопротивление шунта (мкОм)
#define REG_REGULATOR_QI_MAX			7	// Ограничение уровня интегральной составляющей
#define REG_FAN_CTRL					8	// Включение управления вентилятором
#define REG_AFTER_PULSE_PAUSE			9	// Пауза после импульса (мс)
#define REG_CURRENT_PER_CURBOARD		10	// Максимальная амплитуда тока с одной CurrentBoard (А * 10)
#define REG_CURBOARD_QUANTITY			11	// Количество CurrentBoard
#define REG_SCOPE_STEP					12	// Шаг сохранения оцифрованных значений
#define REG_FAN_OPERATE_PERIOD			13	// Период работы вентилятора (сек)
#define REG_FAN_OPERATE_TIME			14	// Время включенного состояния вентилятора (сек)
#define REG_DAC_OFFSET					15	// Смещение сигнала с ЦАП
#define REG_CONFIG_READY_STATE_TIMEOUT	16	// Таймаут нахождения блока в состоянии DS_ConfigReady

#define REG_I_TO_DAC_RANGE0_K			20	// Диапазон 0 - Коэффициент пересчета в значение ЦАП
#define REG_I_TO_DAC_RANGE0_B			21	// Диапазон 0 - Смещение пересчета в значение ЦАП
#define REG_I_TO_DAC_RANGE1_K			22	// Диапазон 1 - Коэффициент пересчета в значение ЦАП
#define REG_I_TO_DAC_RANGE1_B			23	// Диапазон 1 - Смещение пересчета в значение ЦАП
#define REG_I_TO_DAC_RANGE2_K			24	// Диапазон 2 - Коэффициент пересчета в значение ЦАП
#define REG_I_TO_DAC_RANGE2_B			25	// Диапазон 2 - Смещение пересчета в значение ЦАП
//
#define REG_ADC_VOLTAGE_K				26	// Коэффициент грубой подстройки K
#define REG_ADC_VOLTAGE_B				27	// Смещение грубой подстройки B
//
#define REG_K_AMP_RANGE0				28	// Коэффициент усиления тока в канале 0 (*100)
#define REG_K_AMP_RANGE1				29	// Коэффициент усиления тока в канале 1 (*100)
#define REG_K_AMP_RANGE2				30	// Коэффициент усиления тока в канале 2 (*100)
//
#define REG_ADC_I_RANGE0_P2				31	// Диапазон 0 - Коэффициент тонкой подстройки Р2 х1е6
#define REG_ADC_I_RANGE0_P1				32	// Диапазон 0 - Коэффициент тонкой подстройки Р1 x1000
#define REG_ADC_I_RANGE0_P0				33	// Диапазон 0 - Смещение тонкой подстройки Р0
#define REG_ADC_I_RANGE0_N				34	// Диапазон 0 - Числитель коэффициента грубой подстройки
#define REG_ADC_I_RANGE0_D				35	// Диапазон 0 - Знаменатель коэффициента грубой подстройки
#define REG_ADC_I_RANGE0_B				36	// Диапазон 0 - Смещение тонкой подстройки B
//
#define REG_ADC_I_RANGE1_P2				37	// Диапазон 1 - Коэффициент тонкой подстройки Р2 х1е6
#define REG_ADC_I_RANGE1_P1				38	// Диапазон 1 - Коэффициент тонкой подстройки Р1 x1000
#define REG_ADC_I_RANGE1_P0				39	// Диапазон 1 - Смещение тонкой подстройки Р0
#define REG_ADC_I_RANGE1_N				40	// Диапазон 1 - Числитель коэффициента грубой подстройки
#define REG_ADC_I_RANGE1_D				41	// Диапазон 1 - Знаменатель коэффициента грубой подстройки
#define REG_ADC_I_RANGE1_B				42	// Диапазон 1 - Смещение тонкой подстройки B
//
#define REG_ADC_I_RANGE2_P2				43	// Диапазон 2 - Коэффициент тонкой подстройки Р2 х1е6
#define REG_ADC_I_RANGE2_P1				44	// Диапазон 2 - Коэффициент тонкой подстройки Р1 x1000
#define REG_ADC_I_RANGE2_P0				45	// Диапазон 2 - Смещение тонкой подстройки Р0
#define REG_ADC_I_RANGE2_N				46	// Диапазон 2 - Числитель коэффициента грубой подстройки
#define REG_ADC_I_RANGE2_D				47	// Диапазон 2 - Знаменатель коэффициента грубой подстройки
#define REG_ADC_I_RANGE2_B				48	// Диапазон 2 - Смещение тонкой подстройки B
//
#define REG_REGULATOR_RANGE0_Kp			49	// Диапазон 0 - Пропорциональный коэффициент регулятора
#define REG_REGULATOR_RANGE0_Ki			50	// Диапазон 0 - Интегральный коэффициент регулятора
#define REG_REGULATOR_RANGE1_Kp			51	// Диапазон 1 - Пропорциональный коэффициент регулятора
#define REG_REGULATOR_RANGE1_Ki			52	// Диапазон 1 - Интегральный коэффициент регулятора
#define REG_REGULATOR_RANGE2_Kp			53	// Диапазон 2 - Пропорциональный коэффициент регулятора
#define REG_REGULATOR_RANGE2_Ki			54	// Диапазон 2 - Интегральный коэффициент регулятора
//
#define REG_REGULATOR_TF_Ki_RANG0		55	// Диапазон 0 - Коэффициент подстройки значения Ki от тока (в ед. (dKi / dI) * 1000)
#define REG_REGULATOR_TF_Ki_RANG1		56	// Диапазон 1 - Коэффициент подстройки значения Ki от тока (в ед. (dKi / dI) * 1000)
#define REG_REGULATOR_TF_Ki_RANG2		57	// Диапазон 2 - Коэффициент подстройки значения Ki от тока (в ед. (dKi / dI) * 1000)
//
#define REG_CURRENT_SETPOINT_MIN		58	// Минимальное задание по току (А х10)
#define REG_CURRENT_SETPOINT_MAX		59	// Максимальное задание по току (А х10)

// Несохраняемые регистры чтения-записи
#define REG_CURRENT_PULSE_VALUE			128	// Задание амплитуды импульса тока (А * 10)
#define REG_USE_LINEAR_DOWN				130	// Спад тока идёт по линейному закону

#define REG_DBG							150	// Регистр режима Отладки

// Регистры только чтение
#define REG_DEV_STATE					192	// Регистр состояния
#define REG_FAULT_REASON				193	// Регистр Fault
#define REG_DISABLE_REASON				194	// Регистр Disable
#define REG_WARNING						195	// Регистр Warning
#define REG_PROBLEM						196	// Регистр Problem
#define REG_OP_RESULT					197	// Регистр результата операции
#define REG_SUB_STATE					198	// Регистр вспомогательного состояния

#define REG_RESULT_CURRENT				200	// Достигнутое значение импульса тока (А * 10)
#define REG_BATTERY_VOLTAGE				201	// Напряжение на батарее конденсаторов (В * 10)
// -----------------------------
#define REG_FWINFO_SLAVE_NID			256	// Device CAN slave node ID
#define REG_FWINFO_MASTER_NID			257	// Device CAN master node ID (if presented)
// 258 - 259
#define REG_FWINFO_STR_LEN				260	// Length of the information string record
#define REG_FWINFO_STR_BEGIN			261	// Begining of the information string record
//
#define REG_MEM_SYMBOL					299	// Считанный по адресу памяти символ
// -----------------------------

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
#define ERR_CONFIGURATION_LOCKED		1	//  Устройство защищено от записи
#define ERR_OPERATION_BLOCKED			2	//  Операция не может быть выполнена в текущем состоянии устройства
#define ERR_DEVICE_NOT_READY			3	//  Устройство не готово для смены состояния
#define ERR_WRONG_PWD					4	//  Неправильный ключ

// Endpoints
#define EP_CURRENT						1
#define	EP_BATTERY_VOLTAGE				2
#define EP_REGULATOR_OUTPUT				3
#define EP_REGULATOR_ERR				4
#define EP_CUR_TABLE					5
#define EP_DAC_RAW_DATA					6

#endif //  __DEV_OBJ_DIC_H
