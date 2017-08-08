/*
 * artyBotLib.h
 *
 *  Created on: Aug 3, 2017
 *      Author: Arvin Tang
 */

#ifndef SRC_ARTYBOTLIB_H_
#define SRC_ARTYBOTLIB_H_

/************ Include Files ************/

#include <stdint.h>


/************ Function Prototypes ************/

void artyBotInit();

void driveForward(double distance);

void driveBackward(double distance);

void turnLeft(int degrees);

void turnRight(int degrees);

#endif /* SRC_ARTYBOTLIB_H_ */
