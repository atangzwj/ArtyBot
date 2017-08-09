/*
 * pidController.c
 *
 *  Created on: Jul 13, 2017
 *      Author: Arvin Tang
 */

/************ Include Files ************/

#include "pidController.h"


/************ Macro Definitions ************/

#define K_PROP_POS 0.014
#define K_INTG_POS 0.0012
#define K_DIFF_POS 0.0014

#define BASE_DUTY_CYCLE 0.6


/************ Global Variables ************/

static int pos_diff_sum  = 0;
static int pos_diff_prev = 0;


/************ Function Definitions ************/

// Take position difference between motor1 and motor2 as an int and an array of
// doubles to store corrected duty cycles for motor1, motor2, respectively
// Assumes that this function gets called at regular time intervals
void getPosCorrection(int pos_diff, double duty_cycle[]) {
   pos_diff_sum += pos_diff;

   double correction =   K_PROP_POS *  pos_diff
                       + K_INTG_POS *  pos_diff_sum
                       + K_DIFF_POS * (pos_diff - pos_diff_prev);

   duty_cycle[0] = BASE_DUTY_CYCLE;
   duty_cycle[1] = BASE_DUTY_CYCLE;
   if (correction < 0) {
      duty_cycle[0] -= correction; // Motor1 lagging, speed up
   } else {
      duty_cycle[1] += correction; // Motor2 lagging, speed up
   }

   pos_diff_prev = pos_diff;

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
   pos_diff_sum  = 0;
   pos_diff_prev = 0;
}
