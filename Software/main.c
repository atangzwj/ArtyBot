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


/************ Function Definitions ************/

int main() {
   init_platform();

   initIO();
   clearPosCounter();
   driveStraight();

   cleanup_platform();
   return 0;
}
