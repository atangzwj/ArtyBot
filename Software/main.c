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
#include "artyBotLib.h"
#include "motorControl.h"
#include "platform.h"

#include "microblaze_sleep.h"
/************ Function Definitions ************/

int main() {
   init_platform();

   initIO();
   artyBotInit();

   int sw0 = XGpio_DiscreteRead(xgpio1, SW_CHANNEL) & 0x1;
   while (!sw0) {
      sw0 = XGpio_DiscreteRead(xgpio1, SW_CHANNEL) & 0x1;
   }

   setDirForward();
   drive(48 * 4 * 6);

   stop();
   usleep(100);

   setDirBackward();
   drive(48 * 4 * 6);

   stop();

   cleanup_platform();
   return 0;
}
