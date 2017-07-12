/*
 * artyBot.c
 *
 *  Created on: Jul 5, 2017
 *      Author: Arvin Tang
 *
 *  ArtyBot function definitions
 */

#include <stdlib.h>
#include "xil_io.h"
#include "microblaze_sleep.h"
#include "artyBot.h"

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

// Turns both motors on at half speed when SW0 is toggled on (milestone 2)
void motorSwitch() {
   PWM_Set_Period(PWM_BASEADDR, PWM_PER);
   PWM_Set_Duty(PWM_BASEADDR, PWM_DUTY, PWM_M1);
   PWM_Set_Duty(PWM_BASEADDR, PWM_DUTY, PWM_M2);

   PWM_Disable(PWM_BASEADDR); // Disable PWM before changing motor directions

   MOTOR1_FORWARD; // Set motor directions to forward
   MOTOR2_FORWARD;

   int sw0 = XGpio_DiscreteRead(xgpio1, SW_CHANNEL) & 0x1;

   while (1) {
      if (sw0) {
         PWM_Enable(PWM_BASEADDR);
      } else {
         PWM_Disable(PWM_BASEADDR);
      }
      sw0 = XGpio_DiscreteRead(xgpio1, SW_CHANNEL) & 0x1;
   }
}

// Displays the speed of both wheels in RPM on terminal emulator every half
// second, SW0 will turn on the motors (milestone 3)
void displaySpeed() {
   PWM_Set_Period(PWM_BASEADDR, PWM_PER);
   PWM_Set_Duty(PWM_BASEADDR, PWM_DUTY, PWM_M1);
   PWM_Set_Duty(PWM_BASEADDR, PWM_DUTY, PWM_M2);

   PWM_Disable(PWM_BASEADDR); // Disable PWM before changing motor directions

   MOTOR1_FORWARD; // Set motor directions to forward
   MOTOR2_FORWARD;

   print("[  m1 ,  m2 ] (RPM)\n\r");

   int motor_speed[2];
   int sw0 = 0;
   while (1) {
      sw0 = XGpio_DiscreteRead(xgpio1, SW_CHANNEL) & 0x1;
      if (sw0) {
         PWM_Enable(PWM_BASEADDR);
      } else {
         PWM_Disable(PWM_BASEADDR);
      }
      clearCounts();             // Clear counters
      usleep(500000);            // Sleep half second
      measureSpeed(motor_speed); // Take measurements
      xil_printf("[ %3d , %3d ]\r", motor_speed[0], motor_speed[1]);
   }
}

// Takes an int array to store the angular speeds of the wheel on motors 1 and
// 2, respectively, in RPM
// Clears the counts after taking measurements
void measureSpeed(int motor_speed[]) {
   int m1[2];
   int m2[2];

   getEdgeCounts(EDGE_COUNTER_0_BASEADDR, m1);
   getEdgeCounts(EDGE_COUNTER_1_BASEADDR, m2);

   // Compute wheel speeds in RPM
   // The full computation is 0.25 * 60 / 48 * sens * CLK_FREQ / clk
   // 0.25 turn of the magnetic encoder disk per sensor edge,
   // 60 sec/min to convert rev/sec to rev/min,
   // 48 is the gearbox ratio of the motor
   // Constants at beginning of expression have been combined to 0.3125
   motor_speed[0] = 0.3125 * m1[0] * CLK_FREQ / m1[1];
   motor_speed[1] = 0.3125 * m2[0] * CLK_FREQ / m2[1];
   clearCounts();
}

// Take the base address of an EdgeCounter module and an int array
// The number of sensor edges and clock edges are stored in the given array in
// that order
void getEdgeCounts(uintptr_t baseaddr, int edges[]) {
   edges[0] = (int) Xil_In32(baseaddr + SENS_COUNT_OFFSET);
   edges[1] = (int) Xil_In32(baseaddr + CLK_COUNT_OFFSET);
}

// Clear the registers storing counts of sensor edges and clock edges
void clearCounts() {
   Xil_Out8(EDGE_COUNTER_0_BASEADDR, 0x1);
   Xil_Out8(EDGE_COUNTER_1_BASEADDR, 0x1);

   Xil_Out8(EDGE_COUNTER_0_BASEADDR, 0x0);
   Xil_Out8(EDGE_COUNTER_1_BASEADDR, 0x0);
}
