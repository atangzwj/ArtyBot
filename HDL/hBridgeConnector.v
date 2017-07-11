`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/30/2017 01:09:14 PM
// Design Name: 
// Module Name: hBridgeConnector
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

// Module to connect AXI GPIO and PWM signals to Pmod Bridge
module hBridgeConnector(
   input  [3:0] in_top_bus_I,
   output [3:0] in_top_bus_T,
   output [3:0] in_top_bus_O,
   output [3:0] in_bottom_bus_O,
   output [3:0] in_bottom_bus_T,
   input  [3:0] in_bottom_bus_I,
   output [3:0] gpio_io_i,
   input  [3:0] gpio_io_o,
   input  [3:0] gpio_io_t,
   output [3:0] gpio2_io_i,
   input  [3:0] gpio2_io_o,
   input  [3:0] gpio2_io_t,
   input  [1:0] pwm,
   output       m1_feedback,
   output       m2_feedback
   );

   /* Top row */
   assign in_top_bus_T = 4'b1100; // Disable/enable buffers
   
   assign in_top_bus_O[0] = pwm[0];                      // EN1
   assign in_top_bus_O[1] = gpio_io_o[0];// | gpio_io_o[1]; // DIR1
   assign m1_feedback     = in_top_bus_I[2];             // S1A
   
   /* Bottom row */
   assign in_bottom_bus_T = 4'b1100; // Disable/enable buffers
   
   assign in_bottom_bus_O[0] = pwm[1];                        // EN2
   assign in_bottom_bus_O[1] = gpio2_io_o[0];// | gpio2_io_o[1]; // DIR2
   assign m2_feedback        = in_bottom_bus_I[2];            // S2A
   
   /* Unused ports */
   assign in_top_bus_O[3:2]    = 2'b00;
   assign in_bottom_bus_O[3:2] = 2'b00;
   assign gpio_io_i            = 4'b0000;
   assign gpio2_io_i           = 4'b0000;
endmodule
