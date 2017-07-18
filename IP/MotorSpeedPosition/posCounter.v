`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 07/17/2017 04:20:34 PM
// Design Name: 
// Module Name: posCounter
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


module posCounter (
   input  wire        clk,
   output reg  [31:0] pos1,
   output reg  [31:0] pos2,
   input  wire        sensor,
   input  wire        clear,
   input  wire        subtract,
   input  wire [31:0] distance
);
   
   reg sensor_prev;
   
   initial begin
      pos1        = 32'b0;
      pos2        = 32'b0;
      sensor_prev =  1'b0;
   end
   
   always @ (posedge clk) begin
      if (clear) begin
         pos1        <= 32'b0;
         sensor_prev <= sensor;
      end else if (subtract) begin
         pos2        <= pos2 - distance;
         sensor_prev <= sensor;
      end else if (~sensor_prev & sensor) begin
         pos1        <= pos1 + 1'b1;
         pos2        <= pos2 + 1'b1;
         sensor_prev <= sensor;
      end else begin
         sensor_prev <= sensor;
      end
   end
endmodule

module posCounter_testbench ();
   reg         clk;
   wire [31:0] pos1;
   wire [31:0] pos2;
   reg         sensor;
   reg         clear;
   reg         subtract;
   wire [31:0] distance;

   posCounter dut (
      .clk(clk),
      .pos1(pos1),
      .pos2(pos2),
      .sensor(sensor),
      .clear(clear),
      .subtract(subtract),
      .distance(distance)
   );

   parameter CLOCK_PERIOD = 10;
   initial begin
      clk <= 0;
      forever #(CLOCK_PERIOD / 2) clk <= ~clk;
   end

   assign distance = 32'b11;

   initial begin
      sensor <= 0; clear <= 0; subtract <= 0; @(posedge clk);
                   clear <= 1;                @(posedge clk);
      sensor <= 1; clear <= 0;                @(posedge clk);
      sensor <= 0;                            @(posedge clk);
      sensor <= 1;                            @(posedge clk);
      sensor <= 0;                            @(posedge clk);
      sensor <= 1;                            @(posedge clk);
      sensor <= 0;                            @(posedge clk);
      sensor <= 1;                            @(posedge clk);
      sensor <= 0;                            @(posedge clk);
      sensor <= 1;                            @(posedge clk);
      sensor <= 0;                            @(posedge clk);
      sensor <= 1;                            @(posedge clk);
      sensor <= 0;                            @(posedge clk);
      sensor <= 1;                            @(posedge clk);
      sensor <= 0;                            @(posedge clk);
                               subtract <= 1; @(posedge clk);
                               subtract <= 0; @(posedge clk);
      sensor <= 1;                            @(posedge clk);
      sensor <= 0;                            @(posedge clk);
      sensor <= 1;                            @(posedge clk);
      sensor <= 0;                            @(posedge clk);
      $stop;
   end
endmodule
