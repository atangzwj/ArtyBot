/*
 * pidController.c
 *
 *  Created on: Jul 13, 2017
 *      Author: Arvin Tang
 */

/************ Include Files ************/

#include "pidController.h"


/************ Macro Definitions ************/

#define K_PROP_SPEED 0.004
#define K_INTG_SPEED 0.0005

#define K_PROP_POS 0.00007
#define K_INTG_POS 0.000028


/************ Global Variables ************/

int err_sum_speed[2]  = {0, 0};
int err_prev_speed[2] = {0, 0};

int err_sum_pos  = 0;
int err_prev_pos = 0;


/************ Function Definitions ************/

// Takes an int array containing current speeds for motor1, motor2, respectively
// Takes a double array to store new duty cycle for motor1, motor2, respectively
// Assumes that this function gets called at regular time intervals
void getSpeedCorrection(int speed_sp, int speed[], double duty_cycle[]) {
   int error_m1 = speed_sp - speed[0]; // Current error
   int error_m2 = speed_sp - speed[1];

   err_sum_speed[0] += error_m1; // Accumulated error
   err_sum_speed[1] += error_m2;

   duty_cycle[0] =   K_PROP_SPEED *  error_m1
                   + K_INTG_SPEED *  err_sum_speed[0];
   duty_cycle[1] =   K_PROP_SPEED *  error_m2
                   + K_INTG_SPEED *  err_sum_speed[1];

   // Bound duty cycles between 0 and 1
   if (duty_cycle[0] < 0) {
      duty_cycle[0] = 0.0;
   } else if (duty_cycle[0] > 1) {
      duty_cycle[0] = 1.0;
   }

   if (duty_cycle[1] < 0) {
      duty_cycle[1] = 0.0;
   } else if (duty_cycle[1] > 1) {
      duty_cycle[1] = 1.0;
   }
}

// Take position difference between motor1 and motor2 as int and a double array
// to store new duty cycle for motor1, motor2, respectively
// Assumes that this function gets called at regular time intervals
void getPosCorrection(int pos_diff, double duty_cycle[]) {
   err_sum_pos += pos_diff;

   if (pos_diff > 24) {
      duty_cycle[0] = 0;
   } else if (pos_diff < -24) {
      duty_cycle[1] = 0;
   } else if (pos_diff > 10) {
      duty_cycle[0] /= 3;
   } else if (pos_diff < -10) {
      duty_cycle[1] /= 3;
   } else if (pos_diff > 6) {
      duty_cycle[0] /= 2;
   } else if (pos_diff < -6) {
      duty_cycle[1] /= 2;
   } else if (pos_diff > 3) {
      duty_cycle[0] /= 1.5;
   } else if (pos_diff < -3) {
      duty_cycle[1] /= 1.5;
   }

   // Bound duty cycles between 0 and 1
   if (duty_cycle[0] < 0) {
      duty_cycle[0] = 0.0;
   } else if (duty_cycle[0] > 1) {
      duty_cycle[0] = 1.0;
   }

   if (duty_cycle[1] < 0) {
      duty_cycle[1] = 0.0;
   } else if (duty_cycle[1] > 1) {
      duty_cycle[1] = 1.0;
   }
}

// Reset accumulated error and previous error to 0
void resetErrors() {
   err_sum_speed[0] = 0;
   err_sum_speed[1] = 0;

   err_prev_speed[0] = 0;
   err_prev_speed[1] = 0;

   err_sum_pos  = 0;
   err_prev_pos = 0;
}
