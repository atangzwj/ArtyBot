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

void drive(double distance);

void turn(double arclength);

void delayUntilStop();


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
   drive(distance);
}

// Drive bot backward by given distance (in cm)
void driveBackward(double distance) {
   setDirBackward();
   drive(distance);
}

// Turn bot to the left by given number of degrees from forward
void turnLeft(int degrees) {
   double arclength = (double) FULL_TURN_ARCLENGTH * (degrees / 360.0);
   setDirLeft();
   turn(arclength);
}

// Turn bot to the right by given number of degrees from forward
void turnRight(int degrees) {
   double arclength = (double) FULL_TURN_ARCLENGTH * (degrees / 360.0);
   setDirRight();
   turn(arclength);
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

void drive(double distance) {
   int16_t dist_converted = (int16_t) (distance * 9.4); // cm to sens edges
   int16_t pos_diff = getPositionDifference();
   double duty_cycle[2];
   getPosCorrection(pos_diff, duty_cycle);

   int16_t dist_traveled = getDistanceTraveled();

   PWM_Set_Duty(PWM_BASEADDR, (u32) (duty_cycle[0] * PWM_PER), PWM_M1);
   PWM_Set_Duty(PWM_BASEADDR, (u32) (duty_cycle[1] * PWM_PER), PWM_M2);
   PWM_Enable(PWM_BASEADDR);

   while (dist_traveled < dist_converted) {
      usleep(SAMPLE_PER);
      pos_diff = getPositionDifference();
      getPosCorrection(pos_diff, duty_cycle);
      PWM_Set_Duty(PWM_BASEADDR, (u32) (duty_cycle[0] * PWM_PER), PWM_M1);
      PWM_Set_Duty(PWM_BASEADDR, (u32) (duty_cycle[1] * PWM_PER), PWM_M2);
      dist_traveled = getDistanceTraveled();
   }
   PWM_Disable(PWM_BASEADDR);
   delayUntilStop();
}

void turn(double arclength) {
   int16_t dist_converted = (int16_t) (arclength * 9.4); // cm to sens edges

   int motor_speed[2];
   measureSpeed(motor_speed);

   int16_t motor_pos[2];
   getMotorPositions(motor_pos);

   double duty_cycle[2];
   getSpeedCorrection(40, motor_speed, duty_cycle);

   PWM_Set_Duty(PWM_BASEADDR, (u32) (duty_cycle[0] * PWM_PER), PWM_M1);
   PWM_Set_Duty(PWM_BASEADDR, (u32) (duty_cycle[1] * PWM_PER), PWM_M2);
   PWM_Enable(PWM_BASEADDR);

   while (motor_pos[0] < dist_converted || motor_pos[1] < dist_converted) {
      usleep(SAMPLE_PER);
      getMotorPositions(motor_pos);
      measureSpeed(motor_speed);
      getSpeedCorrection(40, motor_speed, duty_cycle);
      PWM_Set_Duty(PWM_BASEADDR, (u32) (duty_cycle[0] * PWM_PER), PWM_M1);
      PWM_Set_Duty(PWM_BASEADDR, (u32) (duty_cycle[1] * PWM_PER), PWM_M2);
      if (motor_pos[0] >= dist_converted) {
         PWM_Set_Duty(PWM_BASEADDR, (u32) 0, PWM_M1);
      }
      if (motor_pos[1] >= dist_converted) {
         PWM_Set_Duty(PWM_BASEADDR, (u32) 0, PWM_M2);
      }
   }
   PWM_Disable(PWM_BASEADDR);
   delayUntilStop();
}

void delayUntilStop() {
   int motor_speed[2];
   clearSpeedCounters();
   measureSpeed(motor_speed);
   for (int i = 0; i < 3; i++) {
      while (motor_speed[0] + motor_speed[1]) {
         usleep(SAMPLE_PER);
         measureSpeed(motor_speed);
      }
      usleep(SAMPLE_PER);
      measureSpeed(motor_speed);
      if (motor_speed[0] + motor_speed[1]) {
         i = -1; // Didn't truly stop, reset counter to 0;
      }
   }
}
