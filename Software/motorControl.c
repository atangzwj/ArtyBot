/*
 * motorControl.c
 *
 *  Created on: Jul 14, 2017
 *      Author: Arvin Tang
 */

/************ Include Files ************/

#include <stdlib.h>
#include "motorControl.h"
#include "MotorSpeedPosition.h"


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

   getEdgeCounts(MSP_BASEADDR, m1, m2);

   // Compute wheel speeds in RPM
   // The full computation is 0.25 * 60 / 48 * sens * CLK_FREQ / clk
   // 0.25 turn of the magnetic encoder disk per sensor edge,
   // 60 sec/min to convert rev/sec to rev/min,
   // 48 is the gearbox ratio of the motor
   // Constants at beginning of expression have been combined to 0.3125
   motor_speed[0] = 0.3125 * m1[0] * CLK_FREQ / m1[1];
   motor_speed[1] = 0.3125 * m2[0] * CLK_FREQ / m2[1];
   clearSpeedCounters(MSP_BASEADDR);
}

// Return distance traveled by motor1 or motor2, whichever is greater
int16_t getDistanceTraveled() {
   int16_t motor_pos[2];
   getMotorPositions(MSP_BASEADDR, motor_pos);
   return (motor_pos[0] + motor_pos[1]) / 2;
}
