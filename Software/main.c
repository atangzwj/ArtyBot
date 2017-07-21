/*
 * main.c
 *
 *  Created on: Jun 29, 2017
 *      Author: Arvin Tang
 *
 *  Main program for the ArtyBot.
 */

#include "artyBot.h"
#include "motorControl.h"
#include "platform.h"

int main() {
	init_platform();

   initIO();

   clearPosCounter();
   driveStraightSpeedPosControl();

   cleanup_platform();
   return 0;
}
