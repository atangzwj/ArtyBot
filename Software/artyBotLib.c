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

int isStopped();

void turn(double arclength);


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
}

// Drive bot backward by given distance (in cm)
void driveBackward(double distance) {
   setDirBackward();
   drive(distance, 0);
}

// Turn bot to the left by given number of degrees from forward
void turnLeft(int degrees) {
   double arclength = (double) FULL_TURN_ARCLENGTH * (degrees / 360.0);
   setDirLeft();
   drive(arclength, 1);
}

// Turn bot to the right by given number of degrees from forward
void turnRight(int degrees) {
   double arclength = (double) FULL_TURN_ARCLENGTH * (degrees / 360.0);
   setDirRight();
   drive(arclength, 1);
}

void setDirForward() {
   PWM_Disable(PWM_BASEADDR);
   usleep(500);
   MOTOR1_FORWARD;
   MOTOR2_FORWARD;
   clearPosCounter();
}

void setDirBackward() {
   PWM_Disable(PWM_BASEADDR);
   usleep(500);
   MOTOR1_BACKWARD;
   MOTOR2_BACKWARD;
   clearPosCounter();
}

void setDirLeft() {
   PWM_Disable(PWM_BASEADDR);
   usleep(500);
   MOTOR1_BACKWARD;
   MOTOR2_FORWARD;
   clearPosCounter();
}

void setDirRight() {
   PWM_Disable(PWM_BASEADDR);
   usleep(500);
   MOTOR1_FORWARD;
   MOTOR2_BACKWARD;
   clearPosCounter();
}

void drive(double distance, int is_turning) {
   int16_t distance_converted = (int16_t) (distance * 9.4); // cm to sens edges
   int16_t pos_diff = getPositionDifference();
   double duty_cycle[2] = {0, 0};
   getPosCorrection(pos_diff, duty_cycle);

   int16_t distance_traveled = getDistanceTraveled();

   PWM_Set_Duty(PWM_BASEADDR, (u32) (duty_cycle[0] * PWM_PER), PWM_M1);
   PWM_Set_Duty(PWM_BASEADDR, (u32) (duty_cycle[1] * PWM_PER), PWM_M2);
   PWM_Enable(PWM_BASEADDR);


   int err1_sum = 0;
   int err2_sum = 0;

   int err1_prev = 0;
   int err2_prev = 0;

   while (distance_traveled < distance_converted) {
      usleep(SAMPLE_PER);
      pos_diff = getPositionDifference();
      getPosCorrection(pos_diff, duty_cycle);
      if (is_turning) {
         duty_cycle[0] -= 0.24;
         duty_cycle[1] -= 0.24;
         int motor_speed[2];
         measureSpeed(motor_speed);

         int err1 = motor_speed[0] - 30;
         int err2 = motor_speed[1] - 30;

         err1_sum += err1;
         err2_sum += err2;

         duty_cycle[0] -= 0.0076 * err1 + 0.0024 * err1_sum + 0.0076 * err1_prev;
         duty_cycle[1] -= 0.0076 * err2 + 0.0024 * err2_sum + 0.0076 * err2_prev;

         err1_prev = err1;
         err2_prev = err2;
      }
      PWM_Set_Duty(PWM_BASEADDR, (u32) (duty_cycle[0] * PWM_PER), PWM_M1);
      PWM_Set_Duty(PWM_BASEADDR, (u32) (duty_cycle[1] * PWM_PER), PWM_M2);
      distance_traveled = getDistanceTraveled();
   }
   PWM_Disable(PWM_BASEADDR);

   while (!isStopped());
}

int isStopped() {
   int motor_speed[2];
   clearSpeedCounters();
   usleep(SAMPLE_PER);
   measureSpeed(motor_speed);
   return motor_speed[0] + motor_speed[1] == 0;
}

void turn(double arclength) {
   int16_t distance_converted = (int16_t) (arclength * 9.4); // cm to sens edges

   int16_t distance_traveled = getDistanceTraveled();
   PWM_Set_Duty(PWM_BASEADDR, (u32) (0.8 * PWM_PER), PWM_M1);
   PWM_Set_Duty(PWM_BASEADDR, (u32) (0.8 * PWM_PER), PWM_M2);
   PWM_Enable(PWM_BASEADDR);

   while (distance_traveled < distance_converted) {
      usleep(SAMPLE_PER);
      distance_traveled = getDistanceTraveled();
   }
}
