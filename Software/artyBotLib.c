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

#define FULL_TURN_ARCLENGTH 48.7


/************ Function Prototypes ************/

void setDirForward();

void setDirBackward();

void setDirLeft();

void setDirRight();

void drive(double distance, int is_turning);


/************ Function Definitions ************/

void artyBotInit() {
   initIO();
   clearPosCounter();
   clearSpeedCounters();
   PWM_Set_Period(PWM_BASEADDR, PWM_PER);
   PWM_Disable(PWM_BASEADDR);
}

// Drive bot forward by given distance (in cm)
void driveForward(double distance) {
   setDirForward();
   drive(distance, 0);
   PWM_Disable(PWM_BASEADDR);
}

// Drive bot backward by given distance (in cm)
void driveBackward(double distance) {
   setDirBackward();
   drive(distance, 0);
   PWM_Disable(PWM_BASEADDR);
}

// Turn bot to the left by given number of degrees from forward
void turnLeft(int degrees) {
   double arclength = (double) FULL_TURN_ARCLENGTH * (degrees / 360.0);
   setDirLeft();
   drive(arclength, 1);
   PWM_Disable(PWM_BASEADDR);
}

// Turn bot to the right by given number of degrees from forward
void turnRight(int degrees) {
   double arclength = (double) FULL_TURN_ARCLENGTH * (degrees / 360.0);
   setDirRight();
   drive(arclength, 1);
   PWM_Disable(PWM_BASEADDR);
}

void setDirForward() {
   PWM_Disable(PWM_BASEADDR);
   MOTOR1_FORWARD;
   MOTOR2_FORWARD;
   clearPosCounter();
}

void setDirBackward() {
   PWM_Disable(PWM_BASEADDR);
   MOTOR1_BACKWARD;
   MOTOR2_BACKWARD;
   clearPosCounter();
}

void setDirLeft() {
   PWM_Disable(PWM_BASEADDR);
   MOTOR1_BACKWARD;
   MOTOR2_FORWARD;
   clearPosCounter();
}

void setDirRight() {
   PWM_Disable(PWM_BASEADDR);
   MOTOR1_FORWARD;
   MOTOR2_BACKWARD;
   clearPosCounter();
}

// Turn on motors until wheels travel given distance (in cm)
void drive(double distance, int is_turning) {
   int16_t distance_converted = (int16_t) (distance * 9.4); // cm to sens edges
   int16_t pos_diff = getPositionDifference();
   double duty_cycle[2];
   getPosCorrection(pos_diff, duty_cycle);

   int16_t distance_traveled = getDistanceTraveled();

   PWM_Set_Duty(PWM_BASEADDR, (u32) (duty_cycle[0] * PWM_PER), PWM_M1);
   PWM_Set_Duty(PWM_BASEADDR, (u32) (duty_cycle[1] * PWM_PER), PWM_M2);
   PWM_Enable(PWM_BASEADDR);

   while (distance_traveled < distance_converted) {
      usleep(SAMPLE_PER);
      pos_diff = getPositionDifference();
      getPosCorrection(pos_diff, duty_cycle);
      PWM_Set_Duty(PWM_BASEADDR, (u32) (duty_cycle[0] * PWM_PER), PWM_M1);
      PWM_Set_Duty(PWM_BASEADDR, (u32) (duty_cycle[1] * PWM_PER), PWM_M2);
      distance_traveled = getDistanceTraveled();
      if (is_turning) {
         PWM_Disable(PWM_BASEADDR);
         usleep(7776);
         PWM_Enable(PWM_BASEADDR);
      }
   }
}
