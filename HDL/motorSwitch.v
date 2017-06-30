`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 06/28/2017 09:34:40 AM
// Design Name: 
// Module Name: motorSwitch
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

// SW[0] turns on the motors and drives the bot forward at full speed
module motorSwitch (JB, SW);
   output [7:0] JB;
   input  [3:0] SW;

   assign JB[1] = 1'b1; // DIR1
   assign JB[5] = 1'b0; // DIR2
   assign JB[0] = SW[0]; // EN1
   assign JB[4] = SW[0]; // EN2
endmodule
