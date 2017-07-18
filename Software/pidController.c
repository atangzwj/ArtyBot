/*
 * pidController.c
 *
 *  Created on: Jul 13, 2017
 *      Author: Arvin Tang
 */

#include "pidController.h"

#define SP_SPEED     100
#define K_PROP_SPEED 0.004
#define K_INTG_SPEED 0.0005
#define K_DIFF_SPEED 0

#define K_PROP_POS 0
#define K_INTG_POS 0
#define K_DIFF_POS 0

int err_sum_speed[2]  = {0, 0};
int err_prev_speed[2] = {0, 0};

int err_sum_pos  = 0;
int err_prev_pos = 0;

// Takes an int array containing current speeds for motor1, motor2, respectively
// Takes a double array to store new duty cycle for motor1, motor2, respectively
// Assumes that this function gets called at regular time intervals
void getSpeedCorrection(int speed[], double duty_cycle[]) {
   int error_m1 = SP_SPEED - speed[0];
   int error_m2 = SP_SPEED - speed[1];

   err_sum_speed[0] += error_m1;
   err_sum_speed[1] += error_m2;

   duty_cycle[0] =   K_PROP_SPEED *  error_m1
                   + K_INTG_SPEED *  err_sum_speed[0]
                   + K_DIFF_SPEED * (error_m1 - err_prev_speed[0]);
   duty_cycle[1] =   K_PROP_SPEED *  error_m2
                   + K_INTG_SPEED *  err_sum_speed[1]
                   + K_DIFF_SPEED * (error_m2 - err_prev_speed[1]);

   err_prev_speed[0] = error_m1;
   err_prev_speed[1] = error_m2;
}

// Takes an int that is difference between motor1 and motor2 position and double
// array to store new duty cycle for motor1, motor2, respectively
// Client must maintain positional difference between the two motors
void getPositionCorrection(int pos_diff, double duty_cycle[]) {
   err_sum_pos += pos_diff;

//   duty_cycle[0] = K_PROP_POS * pos_diff
//                      + K_INTG_POS * err_sum_pos
//                      + K_DIFF_POS * (pos_diff - err_prev_pos);
}

// Reset accumulated error and previous error to 0
void resetErrors() {
   err_sum_speed[0] = 0;
   err_sum_speed[1] = 0;

   err_prev_speed[0] = 0;
   err_prev_speed[1] = 0;
}
