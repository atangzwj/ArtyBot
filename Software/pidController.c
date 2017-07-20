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

#define K_PROP_POS 0.00004
#define K_INTG_POS 0.000015
#define K_DIFF_POS 0

#define K_PROP_SPEEDPOS 0.0005
#define K_INTG_SPEEDPOS 0.0002
#define K_DIFF_SPEEDPOS 0

int err_sum_speed[2]  = {0, 0};
int err_prev_speed[2] = {0, 0};

int err_sum_pos  = 0;
int err_prev_pos = 0;

// Takes an int array containing current speeds for motor1, motor2, respectively
// Takes a double array to store new duty cycle for motor1, motor2, respectively
// Assumes that this function gets called at regular time intervals
void getSpeedCorrection(int speed[], double duty_cycle[]) {
	int error_m1 = SP_SPEED - speed[0]; // Current error
   int error_m2 = SP_SPEED - speed[1];

   err_sum_speed[0] += error_m1; // Accumulated error
   err_sum_speed[1] += error_m2;

   duty_cycle[0] =   K_PROP_SPEED *  error_m1
                   + K_INTG_SPEED *  err_sum_speed[0]
                   + K_DIFF_SPEED * (error_m1 - err_prev_speed[0]);
   duty_cycle[1] =   K_PROP_SPEED *  error_m2
                   + K_INTG_SPEED *  err_sum_speed[1]
                   + K_DIFF_SPEED * (error_m2 - err_prev_speed[1]);

   if (duty_cycle[0] < 0) {
      duty_cycle[0] = 0;
   }
   if (duty_cycle[1] < 0) {
      duty_cycle[1] = 0;
   }

   err_prev_speed[0] = error_m1; // Current error becomes previous error for
   err_prev_speed[1] = error_m2; // next sample
}

void getPosCorrection(int pos_diff, double duty_cycle[]) {
   double pwm_min = 0.6;
   double pwm_max = 0.75;

   err_sum_pos += pos_diff;

   duty_cycle[0] = duty_cycle[0] - K_PROP_POS * pos_diff
                                 - K_INTG_POS * err_sum_pos;
   duty_cycle[1] = duty_cycle[1] + K_PROP_POS * pos_diff
                                 + K_INTG_POS * err_sum_pos;

   if (duty_cycle[0] < pwm_min) {
      duty_cycle[0] = pwm_min;
   } else if (duty_cycle[0] > pwm_max) {
      duty_cycle[0] = pwm_max;
   }

   if (duty_cycle[1] < pwm_min) {
      duty_cycle[1] = pwm_min;
   } else if (duty_cycle[1] > pwm_max) {
      duty_cycle[1] = pwm_max;
   }
}

// Take int array containing current speeds of motors, int containing difference
// between motor1 and motor2 position (m1 - m2), and double array for storing
// new duty cycles for motor1 and motor2, respectively
// Client must maintain positional difference between the two motors
void getSpeedPosCorrection(int speed[], int pos_diff, double duty_cycle[]) {
	// Current errors
	int spd_error_m1 = SP_SPEED - speed[0];
	int spd_error_m2 = SP_SPEED - speed[1];

	// Accumulated errors
	err_sum_speed[0] += spd_error_m1;
	err_sum_speed[1] += spd_error_m2;

	err_sum_pos += pos_diff;

	// New duty cycle computation
	duty_cycle[0] =   K_PROP_SPEEDPOS *  spd_error_m1
						 + K_INTG_SPEEDPOS *  err_sum_speed[0]
					    + K_DIFF_SPEEDPOS * (spd_error_m1 - err_prev_speed[0])
						 - K_PROP_POS 		 *  pos_diff
						 - K_INTG_POS 		 *  err_sum_pos
						 - K_DIFF_POS 		 * (pos_diff - err_prev_pos);

	duty_cycle[1] =   K_PROP_SPEEDPOS *  spd_error_m2
						 + K_INTG_SPEEDPOS *  err_sum_speed[1]
					    + K_DIFF_SPEEDPOS * (spd_error_m2 - err_prev_speed[1])
						 + K_PROP_POS 		 *  pos_diff
						 + K_INTG_POS 		 *  err_sum_pos
						 + K_DIFF_POS 		 * (pos_diff - err_prev_pos);

	// Previous errors
	err_prev_speed[0] = spd_error_m1; // Current errors become previous errors
	err_prev_speed[1] = spd_error_m2; // for next sample

	err_prev_pos = pos_diff;
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
