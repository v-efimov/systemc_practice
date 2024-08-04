// DESCRIPTION: Verilator: Verilog example module
//
// This file ONLY is placed under the Creative Commons Public Domain, for
// any use, without warranty, 2003 by Wilson Snyder.
// SPDX-License-Identifier: CC0-1.0
// ======================================================================

// This is intended to be a complex example of several features, please also
// see the simpler examples/make_hello_c.

module top
  (
   // Declare some signals so we can see how I/O works
   input              clk,
   input              reset_l,
   input              udrvn,
   input [31:0]       udrvnd
   );

   reg [31:0] count_c;
   always_ff @ (posedge clk) begin
      if (!reset_l) begin
         count_c <= 32'h0;
      end
      else begin
         count_c <= count_c + 1;
         if (count_c >= 300) begin
            $display("All finished\n");
            // This write is a magic value the Makefile uses to make sure the
            // test completes successfully.
            $write("*-* All Finished *-*\n");
            $finish;
         end
      end
   end


endmodule

