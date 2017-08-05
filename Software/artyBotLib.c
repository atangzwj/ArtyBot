/*
 * artyBotLib.c
 *
 *  Created on: Aug 3, 2017
 *      Author: Arvin Tang
 */

/************ Include Files ************/

#include "artyBotLib.h"
#include "microblaze_sleep.h"
#include "motorControl.h"
#include "pidController.h"
#include "PWM.h"

/************ Macro Definitions ************/

#define SAMPLE_PER 25000 // 25 ms => 40 Hz sample frequency


/************ Function Definitions ************/

void artyBotInit() {
   clearPosCounter();
   clearSpeedCounters();
   PWM_Set_Period(PWM_BASEADDR, PWM_PER);
   PWM_Disable(PWM_BASEADDR);
}

void setDirForward() {
   PWM_Disable(PWM_BASEADDR);
   MOTOR1_FORWARD;
   MOTOR2_FORWARD;
}

void setDirBackward() {
   PWM_Disable(PWM_BASEADDR);
   MOTOR1_BACKWARD;
   MOTOR2_BACKWARD;
}

void setDirLeft() {
   PWM_Disable(PWM_BASEADDR);
   MOTOR1_BACKWARD;
   MOTOR2_FORWARD;
}

void setDirRight() {
   PWM_Disable(PWM_BASEADDR);
   MOTOR1_FORWARD;
   MOTOR2_BACKWARD;
}

void drive(int16_t target_distance) {
   int16_t pos_diff = getPositionDifference();
   double duty_cycle[2];
   getPosCorrection(pos_diff, duty_cycle);

   int16_t distance_traveled = getDistanceTraveled();

   PWM_Set_Duty(PWM_BASEADDR, (u32) (duty_cycle[0] * PWM_PER), PWM_M1);
   PWM_Set_Duty(PWM_BASEADDR, (u32) (duty_cycle[1] * PWM_PER), PWM_M2);
   PWM_Enable(PWM_BASEADDR);

   while (distance_traveled < target_distance) {
      usleep(SAMPLE_PER);
      pos_diff = getPositionDifference();
      getPosCorrection(pos_diff, duty_cycle);
      PWM_Set_Duty(PWM_BASEADDR, (u32) (duty_cycle[0] * PWM_PER), PWM_M1);
      PWM_Set_Duty(PWM_BASEADDR, (u32) (duty_cycle[1] * PWM_PER), PWM_M2);
      distance_traveled = getDistanceTraveled();
   }
}

void stop() {
   PWM_Disable(PWM_BASEADDR);
}
