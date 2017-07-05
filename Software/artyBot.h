/*
 * artyBot.h
 *
 *  Created on: Jul 5, 2017
 *      Author: Arvin Tang
 *
 *  Declarations of ArtyBot's functions
 */

#ifndef SRC_ARTYBOT_H_
#define SRC_ARTYBOT_H_

/************ Include Files ************/

#include "xparameters.h"
#include "xgpio.h"
#include "PWM.h"


/************ Macro Definitions ************/

#define PWM_BASEADDR XPAR_PWM_0_PWM_AXI_BASEADDR
#define PWM_PER      0x00029000 // 2ms
#define PWM_DUTY     0x00014800 // 50% duty cycle
#define PWM_0        0
#define PWM_1        1

#define GPIO_0_DEV_ID XPAR_AXI_GPIO_0_DEVICE_ID
#define GPIO_1_DEV_ID XPAR_AXI_GPIO_1_DEVICE_ID

#define MOTOR1_CHANNEL 1
#define MOTOR2_CHANNEL 2
#define SW_CHANNEL     1

#define MOTOR1_FORWARD  XGpio_DiscreteWrite(xgpio0, MOTOR1_CHANNEL, 0x3)
#define MOTOR1_BACKWARD XGpio_DiscreteWrite(xgpio0, MOTOR1_CHANNEL, 0x0)
#define MOTOR2_FORWARD  XGpio_DiscreteWrite(xgpio0, MOTOR2_CHANNEL, 0x0)
#define MOTOR2_BACKWARD XGpio_DiscreteWrite(xgpio0, MOTOR1_CHANNEL, 0x3)


/************ Global Variables ************/

XGpio *xgpio0; // Pmod Bridge
XGpio *xgpio1; // Switches
XGpio_Config *xgpio_cfg0;
XGpio_Config *xgpio_cfg1;


/************ Function Prototypes ************/

void initIO();

#endif /* SRC_ARTYBOT_H_ */
