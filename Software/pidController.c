/*
 * pidController.c
 *
 *  Created on: Jul 13, 2017
 *      Author: Arvin Tang
 */

#include "artyBot.h"
#include "pidController.h"

#define SP     100
#define K_PROP 0.004
#define K_INTG 0.0005
#define K_DIFF 0

#define ERROR_M1 (SP - speed[0])
#define ERROR_M2 (SP - speed[1])

int curr_speed[2] = {0, 0};
int err_sum[2] = {0, 0};
int err_prev[2] = {0, 0};

// Takes an int array containing current speeds for motor1, motor2, respectively
// Takes a double array to store coefficient to multiply by PWM_PER to get
// new duty cycle
// Assumes that this function gets called at regular time intervals
void getSpeedCorrection(int speed[], double duty_coeff[]) {
   err_sum[0] += ERROR_M1;
   err_sum[1] += ERROR_M2;

   duty_coeff[0] = K_PROP * ERROR_M1
                      + K_INTG * err_sum[0]
                      + K_DIFF * (ERROR_M1 - err_prev[0]);
   duty_coeff[1] = K_PROP * ERROR_M2
                      + K_INTG * err_sum[1]
                      + K_DIFF * (ERROR_M2 - err_prev[1]);

   err_prev[0] = ERROR_M1;
   err_prev[1] = ERROR_M2;
}

// Reset accumulated error and previous error to 0
void resetErrors() {
   err_sum[0] = 0;
   err_sum[1] = 0;

   err_prev[0] = 0;
   err_prev[1] = 0;
}
