﻿#ifndef __BOARD_H
#define __BOARD_H

#include "stm32f30x.h"

#include "ZwRCC.h"
#include "ZwGPIO.h"
#include "ZwNCAN.h"
#include "ZwTIM.h"
#include "ZwDAC.h"
#include "ZwDMA.h"
#include "ZwADC.h"
#include "ZwEXTI.h"
#include "ZwSCI.h"
#include "ZwIWDG.h"
#include "ZwNFLASH.h"

// Определения для выходных портов
GPIO_PortPinSettingMacro GPIO_FAN				= {GPIOA, Pin_1};
GPIO_PortPinSettingMacro GPIO_PS_CTRL			= {GPIOA, Pin_2};
GPIO_PortPinSettingMacro GPIO_SYNC_CTRL			= {GPIOB, Pin_3};
GPIO_PortPinSettingMacro GPIO_IFB_R0			= {GPIOB, Pin_6};
GPIO_PortPinSettingMacro GPIO_IFB_R1			= {GPIOB, Pin_5};
GPIO_PortPinSettingMacro GPIO_LED				= {GPIOB, Pin_15};

// Определения для входных портов
GPIO_PortPinSettingMacro GPIO_SYNC				= {GPIOB, Pin_4};

// Определения для портов альтернативных функций
GPIO_PortPinSettingMacro GPIO_ALT_CAN_RX		= {GPIOA, Pin_11};
GPIO_PortPinSettingMacro GPIO_ALT_CAN_TX		= {GPIOA, Pin_12};
GPIO_PortPinSettingMacro GPIO_ALT_UART1_TX		= {GPIOA, Pin_9};
GPIO_PortPinSettingMacro GPIO_ALT_UART1_RX		= {GPIOA, Pin_10};

#endif // __BOARD_H
