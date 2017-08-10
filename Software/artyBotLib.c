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
//   print("DONE\n\r");
}

// Turn bot to the right by given number of degrees from forward
void turnRight(int degrees) {
   double arclength = (double) FULL_TURN_ARCLENGTH * (degrees / 360.0);
   setDirRight();
   drive(arclength, 1);
//   print("DONE\n\r");
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

   while (distance_traveled < distance_converted) {
      usleep(SAMPLE_PER);
      pos_diff = getPositionDifference();
      getPosCorrection(pos_diff, duty_cycle);
      if (is_turning) {
//         duty_cycle[0] *= 0.84;
//         duty_cycle[1] *= 0.84;


         double progress = ((double) distance_traveled) / distance_converted;
         int motor_speed[2];
         measureSpeed(motor_speed);

         int slow_down = 0;
         if (!slow_down && progress > 0.7) {
            slow_down = motor_speed[0] > 60 || motor_speed[1] > 60;
         }

         if (slow_down) {
            double speed_coeff = (motor_speed[0] - 20) * 0.01;
            duty_cycle[0] *= speed_coeff;
            duty_cycle[1] *= speed_coeff;
         }

//         if (progress > 0.7) {
//            duty_cycle[0] *= 0.6;
//            duty_cycle[1] *= 0.6;
//         } else if (progress > 0.6) {
//            duty_cycle[0] *= 0.7;
//            duty_cycle[1] *= 0.7;
//         }
//         int16_t m1_dist = Xil_In16(MSP_BASEADDR + M1_POS_OFFSET);
//         int16_t m2_dist = Xil_In16(MSP_BASEADDR + M2_POS_OFFSET);
//         xil_printf("%d  %d\r", (int) m1_dist, (int) m2_dist);
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

// Turn on motors until wheels travel given distance (in cm)
//void drive(double distance) {
//   int16_t distance_converted = (int16_t) (distance * 9.4); // cm to sens edges
//   int16_t pos_diff = getPositionDifference();
//   double duty_cycle[2] = {0, 0};
//   getPosCorrection(pos_diff, duty_cycle);
//
//   int16_t distance_traveled = getDistanceTraveled();
//
//   PWM_Set_Duty(PWM_BASEADDR, (u32) (duty_cycle[0] * PWM_PER), PWM_M1);
//   PWM_Set_Duty(PWM_BASEADDR, (u32) (duty_cycle[1] * PWM_PER), PWM_M2);
//   PWM_Enable(PWM_BASEADDR);
//
//   while (distance_traveled < distance_converted) {
//      usleep(SAMPLE_PER);
//      pos_diff = getPositionDifference();
//      getPosCorrection(pos_diff, duty_cycle);
//      PWM_Set_Duty(PWM_BASEADDR, (u32) (duty_cycle[0] * PWM_PER), PWM_M1);
//      PWM_Set_Duty(PWM_BASEADDR, (u32) (duty_cycle[1] * PWM_PER), PWM_M2);
//      distance_traveled = getDistanceTraveled();
//   }
//}

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
