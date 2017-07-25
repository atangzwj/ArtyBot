/*
 * artyBot.c
 *
 *  Created on: Jul 5, 2017
 *      Author: Arvin Tang
 *
 *  ArtyBot function definitions
 */


/************ Include Files ************/

#include <stdlib.h>
#include "artyBot.h"
#include "microblaze_sleep.h"
#include "motorControl.h"
#include "pidController.h"
#include "PWM.h"


/************ Function Definitions ************/

// Turns both motors on at half speed when SW0 is toggled on (milestone 2)
void motorSwitch() {
   PWM_Set_Period(PWM_BASEADDR, PWM_PER);
   PWM_Set_Duty(PWM_BASEADDR, PWM_DUTY, PWM_M1);
   PWM_Set_Duty(PWM_BASEADDR, PWM_DUTY, PWM_M2);

   PWM_Disable(PWM_BASEADDR); // Disable PWM before changing motor directions

   MOTOR1_FORWARD; // Set motor directions to forward
   MOTOR2_FORWARD;

   int sw0 = XGpio_DiscreteRead(xgpio1, SW_CHANNEL) & 0x1;

   while (1) {
      if (sw0) {
         PWM_Enable(PWM_BASEADDR);
      } else {
         PWM_Disable(PWM_BASEADDR);
      }
      sw0 = XGpio_DiscreteRead(xgpio1, SW_CHANNEL) & 0x1;
   }
}

// Displays the speed of both wheels in RPM on terminal emulator every half
// second, SW0 will turn on the motors (milestone 3)
void displaySpeed() {
   PWM_Set_Period(PWM_BASEADDR, PWM_PER);
   PWM_Set_Duty(PWM_BASEADDR, PWM_DUTY, PWM_M1);
   PWM_Set_Duty(PWM_BASEADDR, PWM_DUTY, PWM_M2);

   PWM_Disable(PWM_BASEADDR); // Disable PWM before changing motor directions

   MOTOR1_FORWARD; // Set motor directions to forward
   MOTOR2_FORWARD;

   print("[  m1 ,  m2 ] (RPM)\n\r");

   int motor_speed[2];
   int sw0 = 0;
   while (1) {
      sw0 = XGpio_DiscreteRead(xgpio1, SW_CHANNEL) & 0x1;
      if (sw0) {
         PWM_Enable(PWM_BASEADDR);
      } else {
         PWM_Disable(PWM_BASEADDR);
      }
      clearSpeedCounters();      // Clear counters
      usleep(500000);            // Sleep half second
      measureSpeed(motor_speed); // Take measurements
      xil_printf("[ %3d , %3d ]\r", motor_speed[0], motor_speed[1]);
   }
}

// Drives bot forward with speed control system when SW0 is on (milestone 4)
void driveStraightSpeedControl() {
   PWM_Set_Period(PWM_BASEADDR, PWM_PER);

   PWM_Disable(PWM_BASEADDR); // Disable PWM before changing motor directions

   MOTOR1_FORWARD;  // Set motor directions to forward
   MOTOR2_FORWARD;

   int speed_sp = (XGpio_DiscreteRead(xgpio1, SW_CHANNEL) >> 1) * 10 + 30;

   int motor_speed[2];
   measureSpeed(motor_speed);
   int sw0 = 0;
   double duty_cycle[2] = {0.4, 0.4};
   while (1) {
      PWM_Set_Duty(PWM_BASEADDR, (u32) (duty_cycle[0] * PWM_PER), PWM_M1);
      PWM_Set_Duty(PWM_BASEADDR, (u32) (duty_cycle[1] * PWM_PER), PWM_M2);

      sw0 = XGpio_DiscreteRead(xgpio1, SW_CHANNEL) & 0x1;
      if (sw0) {
         PWM_Enable(PWM_BASEADDR);
         getSpeedCorrection(speed_sp, motor_speed, duty_cycle);
      } else {
         PWM_Disable(PWM_BASEADDR);
         resetErrors();
      }
      clearSpeedCounters();
      usleep(40000);
      measureSpeed(motor_speed);
      speed_sp = (XGpio_DiscreteRead(xgpio1, SW_CHANNEL) >> 1) * 10 + 30;
   }
}

void driveStraightPosControl() {
   PWM_Set_Period(PWM_BASEADDR, PWM_PER);

   PWM_Disable(PWM_BASEADDR); // Disable PWM before changing motor directions

   MOTOR1_FORWARD; // Set motor directions to forward
   MOTOR2_FORWARD;

   int pos_diff = getPositionDifference();

   int sw0 = 0;
   double duty_cycle[2] = {0.5, 0.5};
   while (1) {
      PWM_Set_Duty(PWM_BASEADDR, (u32) (duty_cycle[0] * PWM_PER), PWM_M1);
      PWM_Set_Duty(PWM_BASEADDR, (u32) (duty_cycle[1] * PWM_PER), PWM_M2);

      sw0 = XGpio_DiscreteRead(xgpio1, SW_CHANNEL) & 0x1;
      if (sw0) {
         PWM_Enable(PWM_BASEADDR);
         duty_cycle[0] = 0.5;
         duty_cycle[1] = 0.5;
         getPosCorrection(pos_diff, duty_cycle);
      } else {
         PWM_Disable(PWM_BASEADDR);
         clearPosCounter();
         resetErrors();
      }
      usleep(40000);
      pos_diff = getPositionDifference();
   }
}

void driveStraightSpeedPosControl() {
   PWM_Set_Period(PWM_BASEADDR, PWM_PER);

   PWM_Disable(PWM_BASEADDR); // Disable PWM before changing motor directions

   MOTOR1_FORWARD; // Set motor directions to forward
   MOTOR2_FORWARD;

   int speed_sp = (XGpio_DiscreteRead(xgpio1, SW_CHANNEL) >> 1) * 10 + 30;

   int motor_speed[2];
   measureSpeed(motor_speed);

   int pos_diff = getPositionDifference();

   int sw0 = 0;
   double duty_cycle[2] = {0.5, 0.5};
   while (1) {
      PWM_Set_Duty(PWM_BASEADDR, (u32) (duty_cycle[0] * PWM_PER), PWM_M1);
      PWM_Set_Duty(PWM_BASEADDR, (u32) (duty_cycle[1] * PWM_PER), PWM_M2);

      sw0 = XGpio_DiscreteRead(xgpio1, SW_CHANNEL) & 0x1;
      if (sw0) {
         PWM_Enable(PWM_BASEADDR);
         getSpeedCorrection(speed_sp, motor_speed, duty_cycle);
         getPosCorrection(pos_diff, duty_cycle);
      } else {
         PWM_Disable(PWM_BASEADDR);
         clearPosCounter();
         resetErrors();
      }
      usleep(40000);
      measureSpeed(motor_speed);
      pos_diff = getPositionDifference();
      speed_sp = (XGpio_DiscreteRead(xgpio1, SW_CHANNEL) >> 1) * 10 + 30;
   }
}

void driveStraightPosControlDebug() {
   PWM_Set_Period(PWM_BASEADDR, PWM_PER);

   PWM_Disable(PWM_BASEADDR); // Disable PWM before changing motor directions

   MOTOR1_FORWARD; // Set motor directions to forward
   MOTOR2_FORWARD;

   sample_data *data_arr = (sample_data*) (calloc(250, sizeof(sample_data)));
   if (data_arr == NULL) {
      print("Memory allocation failed\n\r");
      return;
   }
   int pos_diff = getPositionDifference();

   double duty_cycle[2] = {0.65, 0.65};
   int sw0 = XGpio_DiscreteRead(xgpio1, SW_CHANNEL) & 0x1;
   while (!sw0) {
      sw0 = XGpio_DiscreteRead(xgpio1, SW_CHANNEL) & 0x1;
   }
   for (int i = 0; i < 250; i++) {
      (data_arr[i]).pos_diff = pos_diff;
      (data_arr[i]).m1_duty = duty_cycle[0];
      (data_arr[i]).m2_duty = duty_cycle[1];

      PWM_Set_Duty(PWM_BASEADDR, (u32) (duty_cycle[0] * PWM_PER), PWM_M1);
      PWM_Set_Duty(PWM_BASEADDR, (u32) (duty_cycle[1] * PWM_PER), PWM_M2);

      PWM_Enable(PWM_BASEADDR);

      duty_cycle[0] = 0.65;
      duty_cycle[1] = 0.65;
      getPosCorrection(pos_diff, duty_cycle);

      clearSpeedCounters();
      usleep(40000);
      pos_diff = getPositionDifference();
   }
   PWM_Disable(PWM_BASEADDR);
   int sw3 = XGpio_DiscreteRead(xgpio1, SW_CHANNEL) & 0x8;
   while (!sw3) {
      sw3 = XGpio_DiscreteRead(xgpio1, SW_CHANNEL) & 0x8;
   }
   for (int i = 0; i < 250; i++) {
      xil_printf("%3d   ", i);
      xil_printf("%3d   ", data_arr[i].pos_diff);
      xil_printf("%2d %2d\n\r", (int) (data_arr[i].m1_duty * 100),
                                (int) (data_arr[i].m2_duty * 100));
   }
}
