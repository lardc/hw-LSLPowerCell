#ifndef __LABEL_DESCRIPTION_H
#define __LABEL_DESCRIPTION_H

// Include
#include "stdinc.h"
#include "FirmwareLabel.h"

// Definitions
#define LABEL_START_ADDRESS_SECTOR			0x7800
#define LABEL_START_ADDRESS_SHIFT			0
#define LABEL_START_ADDRESS					(LABEL_START_ADDRESS_SECTOR + LABEL_START_ADDRESS_SHIFT)

// Variables
extern const Label BoardLabels[];
extern const Int16U BoardLabelsSize;

#endif // __LABEL_DESCRIPTION_H
