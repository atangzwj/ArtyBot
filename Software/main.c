/*
 * main.c
 *
 *  Created on: Jun 29, 2017
 *      Author: Arvin Tang
 *
 *  This file contains four demos for the ArtyBot:
 *      drawPolygon(),
 *      avoidWalls(),
 *      joystickDrive(),
 *      joystickSensorDrive().
 *  Inside of main(), uncomment the demo that you would like to run.
 *  drawPolygon() and avoidWalls() wait for sw0 to be toggled on before running
 */

/************ Include Files ************/

#include <stdlib.h>
#include "artyBotLib.h"
#include "MAXSONAR_Processor.h"
#include "microblaze_sleep.h"
#include "motorControl.h"
#include "platform.h"
#include "PmodJSTK2.h"


/************ Macro Definitions ************/

#define SENSOR_BASEADDR XPAR_MAXSONAR_PROCESSOR_0_S00_AXI_BASEADDR

#define PMODJSTK2_SPI_ADDR  XPAR_PMODJSTK2_0_AXI_LITE_SPI_BASEADDR
#define PMODJSTK2_GPIO_ADDR XPAR_PMODJSTK2_0_AXI_LITE_GPIO_BASEADDR

#define READ_SW0 XGpio_DiscreteRead(xgpio1, SW_CHANNEL) & 0x1


/************ Function Prototypes ************/

void drawPolygon(int n, int sideLength);

int isBlocked();

void avoidWalls();

void joystickDrive();

void joystickSensorDrive();


/************ Function Definitions ************/

int main() {
   init_platform();

   artyBotInit();

//   drawPolygon(4, 72);
//   avoidWalls();
//   joystickDrive();
   joystickSensorDrive();

   artyBotEnd();

   cleanup_platform();
   return 0;
}

// Drive the bot when sw0 toggled on to trace an n-sided regular polygon with
// given side length
void drawPolygon(int n, int sideLength) {
   int sw0 = READ_SW0;
   while (!sw0) {
      sw0 = READ_SW0;
   }
   for (int i = 0; i < n; i++) {
      driveForward(sideLength);
      turnRight(360 / n);
   }
}

// Returns logical true if PmodMAXSONAR detects object within 8 inches
int isBlocked() {
   return getDistance(SENSOR_BASEADDR, CLK_FREQ) < 8;
}

// Drive the bot forward when sw0 toggled on, if obstacle detected in front,
// turn 90 degrees to the right until path is clear and then move forward
void avoidWalls() {
   int sw0 = READ_SW0;
   while (!sw0) {
      sw0 = READ_SW0;
   }
   while (sw0) {
      driveForwardContinuous(1);
      while (isBlocked()) {
         turnRight(90);
      }
      sw0 = READ_SW0;
   }
}

// Drive the bot forward, backward, left, or right based on PmodJSTK2 input
void joystickDrive() {
   PmodJSTK2 *pmodJSTK2 = (PmodJSTK2*) calloc(1, sizeof(PmodJSTK2));
   JSTK2_begin(pmodJSTK2, PMODJSTK2_SPI_ADDR, PMODJSTK2_GPIO_ADDR, CLK_FREQ);

   JSTK2_DataPacket dp;
   u8 trig = 0;
   while (!trig) {
      dp = JSTK2_getDataPacket(pmodJSTK2);
      u16 x = dp.XData;
      u16 y = dp.YData;

      trig = dp.Trigger;

      if (x < 376) {
         turnLeftContinuous(5);
      } else if (x > 627) {
         turnRightContinuous(5);
      } else if (y < 376) {
         driveBackwardContinuous(1);
      } else if (y > 627) {
         driveForwardContinuous(1);
      }
   }

   free(pmodJSTK2);
   JSTK2_end(pmodJSTK2);
}

// Drive the bot forward, backward, left, or right based on PmodJSTK2 input
// If obstacle detected in front, driving forward will be disabled
void joystickSensorDrive() {
   PmodJSTK2 *pmodJSTK2 = (PmodJSTK2*) calloc(1, sizeof(PmodJSTK2));
   JSTK2_begin(pmodJSTK2, PMODJSTK2_SPI_ADDR, PMODJSTK2_GPIO_ADDR, CLK_FREQ);

   JSTK2_DataPacket dp;
   u8 trig = 0;
   while (!trig) {
      dp = JSTK2_getDataPacket(pmodJSTK2);
      u16 x = dp.XData;
      u16 y = dp.YData;

      trig = dp.Trigger;

      if (x < 376) {
         turnLeftContinuous(5);
      } else if (x > 627) {
         turnRightContinuous(5);
      } else if (y < 376) {
         driveBackwardContinuous(1);
      } else if (!isBlocked() && y > 627) {
         driveForwardContinuous(1);
      }
   }

   free(pmodJSTK2);
   JSTK2_end(pmodJSTK2);
}
