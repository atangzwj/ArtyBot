
#ifndef MAXSONAR_PROCESSOR_H
#define MAXSONAR_PROCESSOR_H


/************ Include Files ************/

#include "xil_types.h"


/************ Macro Definitions ************/

#define PULSE_LEN_OFFSET 0x00


/************ Function Prototypes ************/

u32 getDistance(u32 baseAddr, int clk_freq);

u32 getPulseLength(u32 baseAddr);

#endif // MAXSONAR_PROCESSOR_H
