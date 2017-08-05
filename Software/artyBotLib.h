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

void setDirForward();

void setDirBackward();

void setDirLeft();

void setDirRight();

void drive(int16_t target_distance);

void stop();

#endif /* SRC_ARTYBOTLIB_H_ */
