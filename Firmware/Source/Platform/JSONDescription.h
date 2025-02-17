#ifndef __JSON_DESCRIPTION_H
#define __JSON_DESCRIPTION_H

#include "Global.h"
#include "stdinc.h"

#define JSON_POINTERS_SIZE 32

extern Int16U Itm1Min, Itm1Max, Itm2Min, Itm2Max, Itm3Min, Itm3Max;

extern void* JSONPointers[JSON_POINTERS_SIZE];

void JSON_AssignPointer(Int16U Index, void* Pointer);

#endif // __JSON_DESCRIPTION_H
