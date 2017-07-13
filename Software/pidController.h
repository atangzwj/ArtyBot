/*
 * pidController.h
 *
 *  Created on: Jul 13, 2017
 *      Author: Arvin Tang
 */

#ifndef SRC_PIDCONTROLLER_H_
#define SRC_PIDCONTROLLER_H_

void getSpeedCorrection(int speed[], double duty[]);

void resetErrors();

#endif /* SRC_PIDCONTROLLER_H_ */
