
#ifndef MAXSONAR_PROCESSOR_H
#define MAXSONAR_PROCESSOR_H


/************ Include Files ************/

#include "xil_types.h"


/************ Macro Definitions ************/

#define DISTANCE_REG_OFFSET 0x00
#define DATA_VALID_OFFSET   0x04


/************ Function Prototypes ************/

u8 getDistance(u32 baseAddr);

u32 getDistanceReg(u32 baseAddr);

int dataIsValid(u32 baseAddr);

#endif // MAXSONAR_PROCESSOR_H
