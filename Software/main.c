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

//   turnRight(180);
//   turnLeft(180);

   turnRight(45);
   turnRight(45);
   turnRight(90);
   turnRight(60);
   turnRight(120);
   driveForward(72);
   turnRight(180);
   driveForward(72);
   turnRight(180);
   turnLeft(60);
   turnLeft(90);
   turnLeft(45);
   turnLeft(120);
   turnLeft(45);

//   driveBackward(72 * 1.414);
//   turnLeft(45);

   cleanup_platform();
   return 0;
}
