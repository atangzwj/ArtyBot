`timescale 1ns / 1ps

module distanceRegister (
   input  wire        clk,
   input  wire        reset,
   output reg  [31:0] distReg,
   output reg         data_valid,
   input  wire [7:0]  distance,
   input  wire        new_dist
);

   reg new_dist_prev;
   initial begin
      new_dist_prev =  1'b1;
      distReg       = 32'b0;
      data_valid    =  1'b0;
   end

   always @ (posedge clk) begin
      if (reset) begin
         new_dist_prev <=  1'b1;
         distReg       <= 32'b0;
         data_valid    <=  1'b0;
      end else if (~new_dist_prev & new_dist) begin
         distReg    <= {distReg[23:0], distance};
         data_valid <= 1'b1;
      end

      new_dist_prev <= new_dist;
   end
endmodule

module distanceRegister_testbench ();
   reg         clk, reset;
   wire [31:0] distReg;
   wire        data_valid;
   reg  [7:0]  distance;
   reg         new_dist;

   distanceRegister dut (
      .clk(clk),
      .reset(reset),
      .distReg(distReg),
      .data_valid(data_valid),
      .distance(distance),
      .new_dist(new_dist)
   );

   parameter CLK_PER = 10;
   initial begin
      clk <= 1;
      forever #(CLK_PER / 2) clk <= ~clk;
   end

   initial begin
      reset <= 0; distance <= 8'h00; new_dist <= 0; @(posedge clk);
      reset <= 1;                                   @(posedge clk);
      reset <= 0; distance <= 8'h88;                @(posedge clk);
                                     new_dist <= 1; @(posedge clk);
                                                    @(posedge clk);
                                                    @(posedge clk);
                                                    @(posedge clk);
                  distance <= 8'hCC; new_dist <= 0; @(posedge clk);
                                     new_dist <= 1; @(posedge clk);
                                                    @(posedge clk);
                                     new_dist <= 0; @(posedge clk);
                  distance <= 8'hAA; new_dist <= 1; @(posedge clk);
                                     new_dist <= 0; @(posedge clk);
                  distance <= 8'hCA; new_dist <= 1; @(posedge clk);
                                     new_dist <= 0; @(posedge clk);
                  distance <= 8'h75; new_dist <= 1; @(posedge clk);
                  distance <= 8'h93;                @(posedge clk);
                                     new_dist <= 0; @(posedge clk);
                                     new_dist <= 1; @(posedge clk);
                                     new_dist <= 0; @(posedge clk);
      $stop;
   end
endmodule
