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


/************ Function Definitions ************/

int main() {
   init_platform();

   artyBotInit();

   int sw0 = XGpio_DiscreteRead(xgpio1, SW_CHANNEL) & 0x1;
   while (sw0) {
      sw0 = XGpio_DiscreteRead(xgpio1, SW_CHANNEL) & 0x1;
   }
   while (!sw0) {
      sw0 = XGpio_DiscreteRead(xgpio1, SW_CHANNEL) & 0x1;
   }

   driveForward(72);
   swingTurnRight(90);
   driveForward(72);
   swingTurnLeft(45);
   driveBackward(72 * 1.414);
   swingTurnLeft(45);

   cleanup_platform();
   return 0;
}
