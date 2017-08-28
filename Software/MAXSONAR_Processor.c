
/************ Include Files ************/

#include "MAXSONAR_Processor.h"
#include "xil_io.h"


/************ Function Definitions ************/

u32 getDistance(u32 baseAddr, int clk_freq) {
   u64 pulse_len = (u64) getPulseLength(baseAddr);
	return (u32) (pulse_len * 1000000 / 147 / clk_freq);
}

u32 getPulseLength(u32 baseAddr) {
   return Xil_In32(baseAddr + PULSE_LEN_OFFSET);
}
