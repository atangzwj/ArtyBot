/*
 * artyBot.h
 *
 *  Created on: Jul 5, 2017
 *      Author: Arvin Tang
 *
 *  Declarations of ArtyBot's functions
 */

#ifndef SRC_ARTYBOT_H_
#define SRC_ARTYBOT_H_

/************ Include Files ************/

#include "xparameters.h"
#include "PWM.h"


/************ Function Prototypes ************/

void motorSwitch();

void displaySpeed();

void driveStraightSpeedControl();

void driveStraightPosControl();

void driveStraightSpeedPosControl();

#endif /* SRC_ARTYBOT_H_ */
