/*
 * main.c
 *
 *  Created on: Jun 29, 2017
 *      Author: Arvin Tang
 *
 *  Main program for the ArtyBot.
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

   int sw0 = READ_SW0;
   while (sw0) {
      sw0 = READ_SW0;
   }
   while (!sw0) {
      sw0 = READ_SW0;
   }

   avoidWalls();

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

int isBlocked() {
   return getDistance(SENSOR_BASEADDR, CLK_FREQ) < 8;
}

void avoidWalls() {
   int sw0 = READ_SW0;
   while (!sw0) {
      sw0 = READ_SW0;
   }
   while (sw0) {
      driveForwardContinuous(1);
      while (isBlocked()) {
         turnRightContinuous(10);
      }
      sw0 = READ_SW0;
   }
}

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

   JSTK2_end(pmodJSTK2);
}

void joystickSensorDrive() {
   PmodJSTK2 *pmodJSTK2 = (PmodJSTK2*) calloc(1, sizeof(PmodJSTK2));
   JSTK2_begin(pmodJSTK2, PMODJSTK2_SPI_ADDR, PMODJSTK2_GPIO_ADDR, CLK_FREQ);

   JSTK2_Position pos;
   u8 trigger = JSTK2_getBtns(pmodJSTK2) & 0x2;
   while (!trigger) {
      pos = JSTK2_getPosition(pmodJSTK2);
      u8 x = pos.XData;
      u8 y = pos.YData;

      trigger = JSTK2_getBtns(pmodJSTK2) & 0x2;

      if (x < 96) {
         turnLeftContinuous(5);
      } else if (x > 160) {
         turnRightContinuous(5);
      } else if (!isBlocked() && y < 96) {
         driveForwardContinuous(1);
      } else if (y > 160) {
         driveBackwardContinuous(1);
      }
   }

   JSTK2_end(pmodJSTK2);
}
