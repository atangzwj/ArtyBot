/*
 * artyBotLib.c
 *
 *  Created on: Aug 3, 2017
 *      Author: Arvin Tang
 */

/************ Include Files ************/

#include "artyBotLib.h"
#include "motorControl.h"
#include "PWM.h"


/************ Macro Definitions ************/




/************ Function Definitions ************/

void setDirForward() {
   PWM_Disable(PWM_BASEADDR);
   MOTOR1_FORWARD;
   MOTOR2_FORWARD;
}

void forward() {
   int16_t pos_diff = getPositionDifference();
   PWM_Enable(PWM_BASEADDR);
}

void stop() {
   PWM_Disable(PWM_BASEADDR);
}
