/*
 * main.c
 *
 *  Created on: Jun 29, 2017
 *      Author: Arvin Tang
 *
 *  Main program for the ArtyBot.
 */

#include "microblaze_sleep.h"
#include "pidController.h"
#include "artyBot.h"

int main() {
   initIO();
   print("Running PID controller program\n\r");
   PWM_Set_Period(PWM_BASEADDR, PWM_PER);


   PWM_Disable(PWM_BASEADDR); // Disable PWM before changing motor directions

   MOTOR1_FORWARD;
   MOTOR2_FORWARD;

   print("[  m1 ,  m2 ] (RPM)\n\r");

   int motor_speed[2];
   int sw0 = 0;
   double duty_coeff[2] = {0.4, 0.4};
   while (1) {
      PWM_Set_Duty(PWM_BASEADDR, (u32) (duty_coeff[0] * PWM_PER), PWM_M1);
      PWM_Set_Duty(PWM_BASEADDR, (u32) (duty_coeff[1] * PWM_PER), PWM_M2);

      sw0 = XGpio_DiscreteRead(xgpio1, SW_CHANNEL) & 0x1;
      if (sw0) {
         PWM_Enable(PWM_BASEADDR);
         getSpeedCorrection(motor_speed, duty_coeff);
      } else {
         PWM_Disable(PWM_BASEADDR);
         resetErrors();
      }
      clearCounts();
      usleep(100000);
      measureSpeed(motor_speed);
      xil_printf("[ %3d , %3d ]\r", motor_speed[0], motor_speed[1]);
   }
   return 0;
}
