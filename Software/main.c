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

//   int16_t motor_pos[2];
//   getMotorPositions(motor_pos);
//   while (1) {
//      xil_printf("%3d %3d\r", motor_pos[0], motor_pos[1]);
//      getMotorPositions(motor_pos);
//   }


//   turnRight(180);
//   while (sw0) {
//      sw0 = XGpio_DiscreteRead(xgpio1, SW_CHANNEL) & 0x1;
//   }
//   while (!sw0) {
//      sw0 = XGpio_DiscreteRead(xgpio1, SW_CHANNEL) & 0x1;
//   }
//   turnLeft(180);

//   driveForward(50);
//   turnRight(180);
//   driveForward(50);
//   turnRight(180);

   driveForward(48);
   turnRight(90);
   driveForward(48);
   turnLeft(45);
   driveBackward(48 * 1.414);
   turnLeft(45);


//   driveBackward(72 * 1.414);
//   turnLeft(45);

   cleanup_platform();
   return 0;
}
