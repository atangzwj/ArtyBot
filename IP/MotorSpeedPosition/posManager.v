`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 07/17/2017 04:24:19 PM
// Design Name: 
// Module Name: posManager
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


module posManager # (
   parameter integer ROLLOVER = 15
) ( 
   input  wire        clk,
   output wire [31:0] pos11,
   output wire [31:0] pos12,
   output wire [31:0] pos21,
   output wire [31:0] pos22,
   output reg  [31:0] count_clk,
   input  wire        m1,
   input  wire        m2,
   input  wire        clear
);

   initial begin
      count_clk = 32'b0;
   end

   always @ (posedge clk) begin
      if (clear) count_clk <= 32'b0;
      else       count_clk <= count_clk + 1'b1;
   end

   wire subtract;
   assign subtract = pos12[ROLLOVER] | pos22[ROLLOVER];

   reg [31:0] distance;
   always @ (*) begin
      if (pos12 < pos22) distance = pos12;
      else               distance = pos22;
   end

   posCounter pos_counter1 (
      .clk(clk),
      .pos1(pos11),
      .pos2(pos12),
      .sensor(m1),
      .clear(clear),
      .subtract(subtract),
      .distance(distance)
   );

   posCounter pos_counter2 (
      .clk(clk),
      .pos1(pos21),
      .pos2(pos22),
      .sensor(m2),
      .clear(clear),
      .subtract(subtract),
      .distance(distance)
   );
endmodule

module posManager_testbench ();
   reg         clk;
   wire [31:0] pos11, pos12, pos21, pos22, count_clk;
   reg         m1, m2, clear;

   posManager # (
      .ROLLOVER(3)
   ) dut (
      .clk(clk),
      .pos11(pos11),
      .pos12(pos12),
      .pos21(pos21),
      .pos22(pos22),
      .count_clk(count_clk),
      .m1(m1),
      .m2(m2),
      .clear(clear)
   );

   parameter CLOCK_PERIOD = 10;
   initial begin
      clk <= 0;
      forever #(CLOCK_PERIOD / 2) clk <= ~clk;
   end

   integer i;
   initial begin
      m1 <= 0; m2 <= 0; clear <= 0; @(posedge clk);
                        clear <= 1; @(posedge clk);
                        clear <= 0; @(posedge clk);
      m1 <= 1; m2 <= 1;             @(posedge clk);
      m1 <= 0; m2 <= 0;             @(posedge clk);
      m1 <= 1; m2 <= 1;             @(posedge clk);
      m1 <= 0; m2 <= 0;             @(posedge clk);
      m1 <= 1; m2 <= 1;             @(posedge clk);
      m1 <= 0; m2 <= 0;             @(posedge clk);
      for (i = 0; i < 16; i = i + 1) begin
         m1 <= 0; m2 <= 0; @(posedge clk);
         m1 <= 1; m2 <= 1; @(posedge clk);
      end
                        clear <= 1; @(posedge clk);
                        clear <= 0; @(posedge clk);
      for (i = 0; i < 16; i = i + 1) begin
         m1 <= 0; m2 <= 0; @(posedge clk);
         m1 <= 1; m2 <= 1; @(posedge clk);
      end
      $stop;
   end
endmodule
