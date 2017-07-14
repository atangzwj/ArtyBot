/*
 * main.c
 *
 *  Created on: Jun 29, 2017
 *      Author: Arvin Tang
 *
 *  Main program for the ArtyBot.
 */

#include "motorControl.h"
#include "artyBot.h"

int main() {
   initIO();
   driveStraightSpeedControl();

   return 0;
}
