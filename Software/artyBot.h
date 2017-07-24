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


typedef struct sample_data {
//   int m1_speed;
//   int m2_speed;
   int pos_diff;
   double m1_duty;
   double m2_duty;
} sample_data;


/************ Function Prototypes ************/

void motorSwitch();

void displaySpeed();

void driveStraightSpeedControl();

void driveStraightPosControl();

void driveStraightSpeedPosControl();

void driveStraightPosControlDebug();

#endif /* SRC_ARTYBOT_H_ */
