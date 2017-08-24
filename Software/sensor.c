/*
 * sensor.c
 *
 *  Created on: Aug 21, 2017
 *      Author: Arvin Tang
 */

/************ Include Files ************/

#include "sensor.h"
#include "motorControl.h"


/************ Function Definitions ************/

int isBlocked() {
   return getDistance(SENSOR_BASEADDR, CLK_FREQ) < 8;
}
