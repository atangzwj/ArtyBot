/*
 * main.c
 *
 *  Created on: Jun 29, 2017
 *      Author: Arvin Tang
 *
 *  Main program for the ArtyBot.
 */

#include "platform.h"
#include "motorControl.h"
#include "artyBot.h"
#include "microblaze_sleep.h"

int main() {
	init_platform();

   initIO();
   displaySpeed();

   cleanup_platform();
   return 0;
}
