#ifndef __GLOBAL_H
#define __GLOBAL_H

#include "SysConfig.h"

// Definitions
// 
#define	SCCI_TIMEOUT_TICKS						1000	// ������� ���������� SCCI (� ��)
#define EP_WRITE_COUNT							0		// ���������� �������� ��� ������
#define EP_COUNT								6		// ���������� �������� ��� ������
#define ENABLE_LOCKING							FALSE	// ������ NV ��������� �������
#define CURRENT_PULSE_WIDTH						10000	// ������������ �������� ���� (���)
#define CURRENT_PULSE_TAIL						5000	// ������������ ������ �������� ���� (���)
#define PI										3.1416f	// �������� ����� ��
#define DAC_MAX_VAL								0x0FFF	// ������������ �������� ���
#define	CURRENT_RANGE_QUANTITY					3		// ���������� ���������� ��������� ����
#define CURRENT_RANGE_0							0		// �������� 0
#define CURRENT_RANGE_1							1		// �������� 1
#define CURRENT_RANGE_2							2		// �������� 2

// ������ �������� EP
#define VALUES_x_SIZE	((CURRENT_PULSE_WIDTH + CURRENT_PULSE_TAIL) / TIMER15_uS)

// ��������� ���������
#define TIME_LED_BLINK							500		// ������� ����������� (� ��)

#endif //  __GLOBAL_H
