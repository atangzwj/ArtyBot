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
#include "sensor.h"

#include "microblaze_sleep.h"
/************ Function Prototypes ************/

void drawPolygon(int n, int sideLength);


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

   while (1) {
      u8 dist = getDistance(SENSOR_BASEADDR);
      xil_printf("dist = %3d\r", dist);
      usleep(500000);
   }

   cleanup_platform();
   return 0;
}

// Drive the bot to trace an n-sided regular polygon with given side length
void drawPolygon(int n, int sideLength) {
   for (int i = 0; i < n; i++) {
      driveForward(sideLength);
      turnRight(360 / n);
   }
}
