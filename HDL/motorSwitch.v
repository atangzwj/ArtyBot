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
module motorSwitch (jb, sw);
   output [7:0] jb;
   input  [3:0] sw;

   assign jb[1] = 1'b1; // DIR1
   assign jb[5] = 1'b0; // DIR2
   assign jb[0] = sw[0]; // EN1
   assign jb[4] = sw[0]; // EN2
endmodule
