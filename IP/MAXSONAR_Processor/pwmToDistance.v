`timescale 1ns / 1ps

module pwmToDistance (
   input  wire       clk,
   input  wire       reset,
   output reg  [7:0] distance,
   output reg        new_dist,
   input  wire       pwm
);

   reg [1:0] ps, ns;
   parameter
   init  = 2'b00,
   first = 2'b01,
   low   = 2'b10,
   high  = 2'b11; 

   reg        pwm_prev;
   reg [31:0] count_clk;
   reg [7:0]  count_inches;
   initial begin
      ps           = init;
      pwm_prev     =  1'b1;
      count_clk    = 32'b0;
      count_inches =  8'b0;
      new_dist     =  1'b0;
   end

   always @ (*) begin
      case (ps)
         init:  if (~pwm_prev & pwm) ns = first;
                else                 ns = init;
         first: if (pwm)             ns = first;
                else                 ns = low;
         high:  if (pwm)             ns = high;
                else                 ns = low;
         low:   if (pwm)             ns = high;
                else                 ns = low;
      endcase
 
      new_dist = ps[1] & ~ps[0]; // True in low state
   end

   always @ (posedge clk) begin
      if (reset) begin
         pwm_prev  <=  1'b1;
         count_clk <= 32'b0;
         distance  <=  8'bX;
      end else if (pwm) begin
         if (count_clk < 32'd12250) begin
            count_clk <= count_clk + 1'b1;
         end else begin
            count_clk    <= 32'b0;
            count_inches <= count_inches + 1'b1;
         end
      end else begin
         distance     <= count_inches;
         count_clk    <= 32'b0;
         count_inches <=  8'b0;
      end
      
      pwm_prev <= pwm;
   end

   always @ (posedge clk) begin
      if (reset) ps <= init;
      else       ps <= ns;
   end
endmodule

module pwmToDistance_testbench ();
   reg        clk, reset;
   wire [7:0] distance;
   wire       new_dist;
   reg        pwm;
   
   pwmToDistance dut (
      .clk(clk),
      .reset(reset),
      .distance(distance),
      .new_dist(new_dist),
      .pwm(pwm)
   );
   
   parameter CLK_PER = 10;
   initial begin
      clk <= 1;
      forever #(CLK_PER / 2) clk <= ~clk;
   end
   
   integer i = 0;
   initial begin
      reset <= 0; pwm <= 0; #12;
      reset <= 1;           #12;
      reset <= 0;           #12;
                  pwm <= 1; #122500;
                  pwm <= 0; #24;
                  pwm <= 1; #200000;
                  pwm <= 0; #40;
                  pwm <= 1; #360000;
                  pwm <= 0; #12;
      $stop;
   end
endmodule
