#ifndef __GLOBAL_H
#define __GLOBAL_H

// Definitions
// 
#define	SCCI_TIMEOUT_TICKS						1000	// Таймаут интерфейса SCCI (в мс)
#define EP_WRITE_COUNT							0		// Количество массивов для записи
#define EP_COUNT								3		// Количество массивов для чтения
#define VALUES_x_SIZE							300		// Размер массивов
#define ENABLE_LOCKING							FALSE	// Защита NV регистров паролем
#define CURRENT_PULSE_WIDTH						10000	// Длительность импульса тока (мкс)
#define PI										3.1416f	// Значение числа Пи
#define DAC_MAX_VAL								0x0FFF	// Максимальное значение ЦАП
#define	CURRENT_RANGE_QUANTITY					3		// Количество диапазонов измерения тока
#define CURRENT_RANGE_0							0		// Диапазон 0
#define CURRENT_RANGE_1							1		// Диапазон 1
#define CURRENT_RANGE_2							2		// Диапазон 2

// Временные параметры
#define TIME_LED_BLINK							500		// Мигание светодиодом (в мс)

#endif //  __GLOBAL_H
