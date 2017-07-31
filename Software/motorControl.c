/*
 * motorControl.c
 *
 *  Created on: Jul 14, 2017
 *      Author: Arvin Tang
 */

/************ Include Files ************/

#include <stdlib.h>
#include "motorControl.h"


/************ Function Definitions ************/

// Initialize Arty Board inputs and outputs
void initIO() {
   // Initialize XGpio structs
   xgpio0 = (XGpio*) calloc(1, sizeof(XGpio));
   xgpio1 = (XGpio*) calloc(1, sizeof(XGpio));
   XGpio_Initialize(xgpio0, GPIO_0_DEV_ID);
   XGpio_Initialize(xgpio1, GPIO_1_DEV_ID);

   // Initialize XGpio_Config structs
   xgpio_cfg0 = XGpio_LookupConfig(GPIO_0_DEV_ID);
   xgpio_cfg1 = XGpio_LookupConfig(GPIO_1_DEV_ID);

   // Set directions
   XGpio_SetDataDirection(xgpio0, M1_CHANNEL, 0xC);
   XGpio_SetDataDirection(xgpio0, M2_CHANNEL, 0xC);
   XGpio_SetDataDirection(xgpio1, SW_CHANNEL, 0xF);
}

// Takes an int array to store the angular speeds of the wheel on motors 1 and
// 2, respectively, in RPM
// Clears the counts after taking measurements
void measureSpeed(int motor_speed[]) {
   int m1[2];
   int m2[2];

   getEdgeCounts(m1, m2);

   // Compute wheel speeds in RPM
   // The full computation is 0.25 * 60 / 48 * sens * CLK_FREQ / clk
   // 0.25 turn of the magnetic encoder disk per sensor edge,
   // 60 sec/min to convert rev/sec to rev/min,
   // 48 is the gearbox ratio of the motor
   // Constants at beginning of expression have been combined to 0.3125
   motor_speed[0] = 0.3125 * m1[0] * CLK_FREQ / m1[1];
   motor_speed[1] = 0.3125 * m2[0] * CLK_FREQ / m2[1];
   clearSpeedCounters();
}

// Take 2 int arrays for storing sensor edge and clock edge counts for computing
// speed of each motor
void getEdgeCounts(int m1[], int m2[]) {
   m1[0] = (int) (Xil_In32(MSP_BASEADDR + M1_POS_OFFSET) >> 16);
   m1[1] = (int)  Xil_In32(MSP_BASEADDR + CLK_OFFSET);

   m2[0] = (int) (Xil_In32(MSP_BASEADDR + M2_POS_OFFSET) >> 16);
   m2[1] = (int)  Xil_In32(MSP_BASEADDR + CLK_OFFSET);
}

// Clear the registers storing counts of sensor edges and clock edges for
// computing speed
void clearSpeedCounters() {
   Xil_Out8(MSP_BASEADDR + CLEAR_OFFSET, 0x1);
   Xil_Out8(MSP_BASEADDR + CLEAR_OFFSET, 0x0);
}

// Return the difference in sensor positive edges between motor1 and motor2
int16_t getPositionDifference() {
   return (int16_t) Xil_In16(MSP_BASEADDR + POS_DIFF_OFFSET);
}
 // Clear the cumulative position counters for both motors
void clearPosCounter() {
   Xil_Out8(MSP_BASEADDR + CLEAR_OFFSET, 0x2);
   Xil_Out8(MSP_BASEADDR + CLEAR_OFFSET, 0x0);
}
