/*
 * motorControl.h
 *
 *  Created on: Jul 14, 2017
 *      Author: AdminUser
 */

#ifndef SRC_MOTORCONTROL_H_
#define SRC_MOTORCONTROL_H_

/************ Include Files ************/

#include "xgpio.h"


/************ Macro Definitions ************/

#define PWM_BASEADDR XPAR_PWM_0_PWM_AXI_BASEADDR
#define PWM_PER      0x00029000 // 2ms
#define PWM_DUTY     0x00014800 // 50% duty cycle
#define PWM_M1       0
#define PWM_M2       1

#define GPIO_0_DEV_ID XPAR_AXI_GPIO_0_DEVICE_ID
#define GPIO_1_DEV_ID XPAR_AXI_GPIO_1_DEVICE_ID

#define M1_CHANNEL 1
#define M2_CHANNEL 2
#define SW_CHANNEL 1

#define MOTOR1_FORWARD  XGpio_DiscreteWrite(xgpio0, M1_CHANNEL, 0x0)
#define MOTOR1_BACKWARD XGpio_DiscreteWrite(xgpio0, M1_CHANNEL, 0x1)
#define MOTOR2_FORWARD  XGpio_DiscreteWrite(xgpio0, M2_CHANNEL, 0x1)
#define MOTOR2_BACKWARD XGpio_DiscreteWrite(xgpio0, M2_CHANNEL, 0x0)

#define EDGE_COUNTER_0_BASEADDR XPAR_EDGE_COUNTER_0_S00_AXI_BASEADDR
#define EDGE_COUNTER_1_BASEADDR XPAR_EDGE_COUNTER_1_S00_AXI_BASEADDR
#define SENS_COUNT_OFFSET 0x04
#define CLK_COUNT_OFFSET  0x08

#define CLK_FREQ 83333333


/************ Global Variables ************/

XGpio *xgpio0; // Pmod Bridge
XGpio *xgpio1; // Switches

XGpio_Config *xgpio_cfg0;
XGpio_Config *xgpio_cfg1;


/************ Function Prototypes ************/

void initIO();

void measureSpeed(int motor_speed[]);

void getEdgeCounts(u32 baseaddr, int edges[]);

void clearCounts();

#endif /* SRC_MOTORCONTROL_H_ */
