#ifndef __DEV_OBJ_DIC_H
#define __DEV_OBJ_DIC_H

// Команды
#define ACT_ENABLE_POWER				1	// Включение блока
#define ACT_DISABLE_POWER				2	// Выключение блока
#define ACT_CLR_FAULT					3	// Очистка всех fault
#define ACT_CLR_WARNING					4	// Очистка всех warning

#define ACT_DBG_PULSE_SYNC				50	// Одиночный импусль в линию SYNC_1
#define ACT_DBG_SET_CURRENT_RANGE		51	// Установка диапазона по току (0, 1, 2)
#define ACT_DBG_POWER_SUPPLY_EN			52	// Проверка активности источников питания на 500В

#define ACT_CONFIG_UNIT					100	// Конфигурация блока
#define ACT_SOFTWARE_START				101	// Старт измерения
#define ACT_STOP_PROCESS				102	// Стоп измерения

#define ACT_SAVE_TO_ROM					200	// Сохранение пользовательских данных во FLASH процессора
#define ACT_RESTORE_FROM_ROM			201	// Восстановление данных из FLASH
#define ACT_RESET_TO_DEFAULT			202	// Сброс DataTable в состояние по умолчанию

#define ACT_BOOT_LOADER_REQUEST			320	// Перезапуск процессора с целью перепрограммирования
// -----------------------------

// Регистры
// Сохраняемые регистры
#define REG_CURRENT_LEVEL_RANGE0		0	// Верхний порог по току диапазона 0 (А * 10)
#define REG_CURRENT_LEVEL_RANGE1		1	// Верхний порог по току диапазона 1 (А * 10)
#define REG_SHUNT_RESISTANCE			2	// Сопротивление шунта (мкОм)
#define REG_FAN_CTRL					5	// Включение управления вентилятором
#define REG_AFTER_PULSE_PAUSE			6	// Пауза после импульса (мс)
#define REG_CURRENT_PER_CURBOARD		3	// Максимальная амплитуда тока с одной CurrentBoard (А)
#define REG_CURBOARD_QUANTITY			4	// Количество CurrentBoard
// 5 - 9
#define REG_I_TO_DAC_RANGE0_K			2	// Диапазон 0 - Коэффициент пересчета в значение ЦАП
#define REG_I_TO_DAC_RANGE0_B			3	// Диапазон 0 - Смещение пересчета в значение ЦАП
#define REG_I_TO_DAC_RANGE1_K			2	// Диапазон 1 - Коэффициент пересчета в значение ЦАП
#define REG_I_TO_DAC_RANGE1_B			3	// Диапазон 1 - Смещение пересчета в значение ЦАП
#define REG_I_TO_DAC_RANGE2_K			2	// Диапазон 2 - Коэффициент пересчета в значение ЦАП
#define REG_I_TO_DAC_RANGE2_B			3	// Диапазон 2 - Смещение пересчета в значение ЦАП

#define REG_ADC_VOLTAGE_K				44	// Коэффициент грубой подстройки K
#define REG_ADC_VOLTAGE_B				45	// Смещение грубой подстройки B
//
#define REG_ADC_I_RANGE0_P2				51	// Диапазон 0 - Коэффициент тонкой подстройки Р2 х1е6
#define REG_ADC_I_RANGE0_P1				52	// Диапазон 0 - Коэффициент тонкой подстройки Р1 x1000
#define REG_ADC_I_RANGE0_P0				53	// Диапазон 0 - Смещение тонкой подстройки Р0
#define REG_ADC_I_RANGE0_N				54	// Диапазон 0 - Числитель коэффициента грубой подстройки
#define REG_ADC_I_RANGE0_D				55	// Диапазон 0 - Знаменатель коэффициента грубой подстройки
#define REG_ADC_I_RANGE0_B				56	// Диапазон 0 - Смещение тонкой подстройки B
//
#define REG_ADC_I_RANGE1_P2				57	// Диапазон 1 - Коэффициент тонкой подстройки Р2 х1е6
#define REG_ADC_I_RANGE1_P1				58	// Диапазон 1 - Коэффициент тонкой подстройки Р1 x1000
#define REG_ADC_I_RANGE1_P0				59	// Диапазон 1 - Смещение тонкой подстройки Р0
#define REG_ADC_I_RANGE1_N				60	// Диапазон 1 - Числитель коэффициента грубой подстройки
#define REG_ADC_I_RANGE1_D				61	// Диапазон 1 - Знаменатель коэффициента грубой подстройки
#define REG_ADC_I_RANGE1_B				62	// Диапазон 1 - Смещение тонкой подстройки B
//
#define REG_ADC_I_RANGE2_P2				63	// Диапазон 2 - Коэффициент тонкой подстройки Р2 х1е6
#define REG_ADC_I_RANGE2_P1				64	// Диапазон 2 - Коэффициент тонкой подстройки Р1 x1000
#define REG_ADC_I_RANGE2_P0				65	// Диапазон 2 - Смещение тонкой подстройки Р0
#define REG_ADC_I_RANGE2_N				66	// Диапазон 2 - Числитель коэффициента грубой подстройки
#define REG_ADC_I_RANGE2_D				67	// Диапазон 2 - Знаменатель коэффициента грубой подстройки
#define REG_ADC_I_RANGE2_B				68	// Диапазон 2 - Смещение тонкой подстройки B
//
#define REG_REGULATOR_RANGE0_Kp			80	// Диапазон 0 - Пропорциональный коэффициент регулятора
#define REG_REGULATOR_RANGE0_Ki			81	// Диапазон 0 - Интегральный коэффициент регулятора
#define REG_REGULATOR_RANGE1_Kp			80	// Диапазон 1 - Пропорциональный коэффициент регулятора
#define REG_REGULATOR_RANGE1_Ki			81	// Диапазон 1 - Интегральный коэффициент регулятора
#define REG_REGULATOR_RANGE2_Kp			80	// Диапазон 2 - Пропорциональный коэффициент регулятора
#define REG_REGULATOR_RANGE2_Ki			81	// Диапазон 2 - Интегральный коэффициент регулятора

#define REG_CURRENT_PULSE_VALUE			128	// Задание амплитуды импульса тока (А * 10)

// Несохраняемы регистры чтения-записи
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


// Operation results
#define OPRESULT_NONE					0	// No information or not finished
#define OPRESULT_OK						1	// Operation was successful
#define OPRESULT_FAIL					2	// Operation failed

//  Fault and disable codes
#define DF_NONE							0

// Problem
#define PROBLEM_NONE					0
#define PROBLEM_FOLOWING_ERROR			1

//  Warning
#define WARNING_NONE					0
#define WARNING_CURRENT_CUTOFF			1

//  User Errors
#define ERR_NONE						0
#define ERR_CONFIGURATION_LOCKED		1	//  Устройство защищено от записи
#define ERR_OPERATION_BLOCKED			2	//  Операция не может быть выполнена в текущем состоянии устройства
#define ERR_DEVICE_NOT_READY			3	//  Устройство не готово для смены состояния
#define ERR_WRONG_PWD					4	//  Неправильный ключ

// Endpoints
#define EP_CURRENT						1
#define EP_REGULATOR_ERR				2

#endif //  __DEV_OBJ_DIC_H
