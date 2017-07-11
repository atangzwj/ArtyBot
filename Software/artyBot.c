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
#include "artyBot.h"

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

//  (milestone 3)
void displaySpeed() {
   double motor_speed[2];
   print("[ m1 , m2 ] = [ ");
      measureSpeed(motor_speed);
      xil_printf("%2.2f , %2.2f ]\n\r", motor_speed[0], motor_speed[1]);
}

// Takes an int array to store the speeds of motors 1 and 2, respectively, in
// revolutions per second
void measureSpeed(double motor_speed[]) {
   int m1_sensor = Xil_In32(EDGE_COUNTER_0_BASEADDR + SENS_COUNT_OFFSET);
   int m1_clk = Xil_In32(EDGE_COUNTER_0_BASEADDR + CLK_COUNT_OFFSET);

   int m2_sensor = Xil_In32(EDGE_COUNTER_1_BASEADDR + SENS_COUNT_OFFSET);
   int m2_clk = Xil_In32(EDGE_COUNTER_1_BASEADDR + CLK_COUNT_OFFSET);

   motor_speed[0] = 0.25 * m1_sensor * CLK_FREQ / m1_clk;
   motor_speed[1] = 0.25 * m2_sensor * CLK_FREQ / m2_clk;
   clearCounts();
}

void clearCounts() {
   Xil_Out8(EDGE_COUNTER_0_BASEADDR, 0x1);
   Xil_Out8(EDGE_COUNTER_1_BASEADDR, 0x1);

   Xil_Out8(EDGE_COUNTER_0_BASEADDR, 0x0);
   Xil_Out8(EDGE_COUNTER_1_BASEADDR, 0x0);
}





















