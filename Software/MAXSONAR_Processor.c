
/************ Include Files ************/

#include "MAXSONAR_Processor.h"
#include "xil_io.h"


/************ Function Definitions ************/

u8 getDistance(u32 baseAddr) {
   return Xil_In8(baseAddr + DISTANCE_REG_OFFSET);
}

u32 getDistanceReg(u32 baseAddr) {
   return Xil_In32(baseAddr + DISTANCE_REG_OFFSET);
}

int dataIsValid(u32 baseAddr) {
   return (int) Xil_In32(baseAddr + DATA_VALID_OFFSET);
}
