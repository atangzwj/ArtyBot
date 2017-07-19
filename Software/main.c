/*
 * main.c
 *
 *  Created on: Jun 29, 2017
 *      Author: Arvin Tang
 *
 *  Main program for the ArtyBot.
 */

#include "platform.h"
#include "motorControl.h"
#include "artyBot.h"
#include "microblaze_sleep.h"

int main() {
	init_platform();

   initIO();
   print("\n\r");
   clearPosCounter();
   driveStraightPosControl();
   //driveStraightSpeedControl();
   //driveStraightSpeedPosControl();
/*
   PWM_Set_Period(PWM_BASEADDR, PWM_PER);
   PWM_Set_Duty(PWM_BASEADDR, PWM_DUTY, PWM_M1);
   PWM_Set_Duty(PWM_BASEADDR, PWM_DUTY, PWM_M2);

   PWM_Disable(PWM_BASEADDR); // Disable PWM before changing motor directions

   MOTOR1_FORWARD; // Set motor directions to forward
   MOTOR2_FORWARD;

   int pos[2];
   print("\n\raccumulated positions:\n\r");
   int sw0 = 0;
   clearPosCounter();
   while (1) {
      sw0 = XGpio_DiscreteRead(xgpio1, SW_CHANNEL) & 0x1;
      if (sw0) {
         PWM_Enable(PWM_BASEADDR);
      } else {
         PWM_Disable(PWM_BASEADDR);
      }
      getPosition(pos);
      xil_printf("%5x  %5x\r", pos[0], pos[1]);
   }
*/
   cleanup_platform();
   return 0;
}
