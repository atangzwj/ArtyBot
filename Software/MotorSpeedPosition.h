
#ifndef MOTORSPEEDPOSITION_H
#define MOTORSPEEDPOSITION_H


/************ Include Files ************/

#include "xil_types.h"


/************ Macro Definitions ************/

#define CLEAR_OFFSET    0x00
#define M1_POS_OFFSET   0x04 // {m1_pos1, m1_pos2} (use pos1 for speed, pos2 for
#define M2_POS_OFFSET   0x08 // {m2_pos1, m2_pos2}  cumulative pos)
#define POS_DIFF_OFFSET 0x0C
#define CLK_OFFSET      0x10 // Resets with pos1 counters above


/************ Function Prototypes ************/

void getEdgeCounts(u32 baseAddr, int m1[], int m2[]);

void clearSpeedCounters(u32 baseAddr);

void getMotorPositions(u32 baseAddr, int16_t motor_pos[]);

int16_t getPositionDifference(u32 baseAddr);

void clearPosCounter(u32 baseAddr);

#endif // MOTORSPEEDPOSITION_H
