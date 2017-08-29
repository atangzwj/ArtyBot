
/************ Include Files ************/

#include "MotorSpeedPosition.h"
#include "xil_io.h"


/************ Function Definitions ************/

// Take 2 int arrays for storing sensor edge and clock edge counts for computing
// speed of each motor
void getEdgeCounts(u32 baseAddr, int m1[], int m2[]) {
   m1[0] = (int) (Xil_In32(baseAddr + M1_POS_OFFSET) >> 16);
   m1[1] = (int)  Xil_In32(baseAddr + CLK_OFFSET);

   m2[0] = (int) (Xil_In32(baseAddr + M2_POS_OFFSET) >> 16);
   m2[1] = (int)  Xil_In32(baseAddr + CLK_OFFSET);
}

// Clear the registers storing counts of sensor edges and clock edges for
// computing speed
void clearSpeedCounters(u32 baseAddr) {
   Xil_Out8(baseAddr + CLEAR_OFFSET, 0x1);
   Xil_Out8(baseAddr + CLEAR_OFFSET, 0x0);
}

// Store positions of motor1, motor2 in given array
void getMotorPositions(u32 baseAddr, int16_t motor_pos[]) {
   motor_pos[0] = Xil_In16(baseAddr + M1_POS_OFFSET);
   motor_pos[1] = Xil_In16(baseAddr + M2_POS_OFFSET);
}

// Return the difference in sensor edges between motor1 and motor2
int16_t getPositionDifference(u32 baseAddr) {
   return (int16_t) Xil_In16(baseAddr + POS_DIFF_OFFSET);
}

// Clear the cumulative position counters for both motors
void clearPosCounter(u32 baseAddr) {
   Xil_Out8(baseAddr + CLEAR_OFFSET, 0x2);
   Xil_Out8(baseAddr + CLEAR_OFFSET, 0x0);
}
