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
   input  [1:0] pwm
   );
   
   assign in_top_bus_O[0] = pwm[0];                      // EN1
   assign in_top_bus_O[1] = gpio_io_o[0] | gpio_io_o[1]; // DIR1
   assign gpio_io_i[3:2] = in_top_bus_O[3:2];            // S1B and S1A
   
   assign in_bottom_bus_O[0] = pwm[1];                        // EN2
   assign in_bottom_bus_O[1] = gpio2_io_o[0] | gpio2_io_o[1]; // DIR2
   assign gpio2_io_i[3:2] = in_bottom_bus_O[3:2];             // S2B and S2A
endmodule
