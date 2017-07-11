/*
 * main.c
 *
 *  Created on: Jun 29, 2017
 *      Author: Arvin Tang
 *
 *  Main program for the ArtyBot.
 */

#include "microblaze_sleep.h"
#include "artyBot.h"

int main() {
   initIO();

   PWM_Set_Period(PWM_BASEADDR, PWM_PER);
   PWM_Set_Duty(PWM_BASEADDR, PWM_DUTY, PWM_M1);
   PWM_Set_Duty(PWM_BASEADDR, PWM_DUTY, PWM_M2);

   PWM_Disable(PWM_BASEADDR); // Disable PWM before changing motor directions

   MOTOR1_FORWARD; // Set motor directions to forward
   MOTOR2_FORWARD;

   //PWM_Enable(PWM_BASEADDR);
   print("BEGINNING TEST\n\r");
   int i = 0;
   clearCounts();
   for (; i < 10; i++) {
      clearCounts();
      usleep(100000);
      displaySpeed();
   }

   displaySpeed();

   PWM_Disable(PWM_BASEADDR);
   return 0;
}
