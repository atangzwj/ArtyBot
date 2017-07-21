/*
 * pidController.h
 *
 *  Created on: Jul 13, 2017
 *      Author: Arvin Tang
 */

#ifndef SRC_PIDCONTROLLER_H_
#define SRC_PIDCONTROLLER_H_

void getSpeedCorrection(int speed_sp, int speed[], double duty[]);

void getPosCorrection(int pos_diff, double duty_cycle[]);

void getSpeedPosCorrection(int speed[], int pos_diff, double duty_cycle[]);

void resetErrors();

#endif /* SRC_PIDCONTROLLER_H_ */
