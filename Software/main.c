/*
 * main.c
 *
 *  Created on: Jun 29, 2017
 *      Author: Arvin Tang
 *
 *  Main program for the ArtyBot.
 */

/************ Include Files ************/

#include "artyBot.h"
#include "motorControl.h"
#include "platform.h"
#include "microblaze_sleep.h"


/************ Function Definitions ************/

int main() {
   init_platform();

   initIO();

   clearPosCounter();
   driveStraightPosControlDebug();

   cleanup_platform();
   return 0;
}
