﻿#ifndef __INITCONFIG_H
#define __INITCONFIG_H

#include "stdinc.h"

//Functions
//
Boolean INITCFG_ConfigSystemClock();
void INITCFG_ConfigIO();
void INITCFG_ConfigCAN();
void INITCFG_ConfigUART();
void INITCFG_ConfigTimer15();
void INITCFG_ConfigTimer7();
void INITCFG_ConfigTimer6();
void INITCFG_ConfigWatchDog();
void INITCFG_ConfigADC();
void INITCFG_ConfigDAC();
void INITCFG_ConfigDMA();
void INITCFG_ConfigExtInterrupt();

#endif //__INITCONFIG_H
