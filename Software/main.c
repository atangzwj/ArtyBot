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

   driveForward(100);
   turnRight(90);
   driveForward(100);
   turnLeft(180);
   driveForward(100);
   turnRight(90);
   driveBackward(100);

   cleanup_platform();
   return 0;
}
