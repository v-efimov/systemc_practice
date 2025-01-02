module tb
# (
  parameter DATA_WIDTH = 32, DELAY_GEN = 0, DELAY_CHK = 0
);

  //--------------------------------------------------------------------------
  // Signals to drive Device Under Test - DUT

  logic clk;
  logic rst;


  //--------------------------------------------------------------------------
  // DUT instantiation
  top # (.DATA_WIDTH (DATA_WIDTH), .DELAY_GEN (DELAY_GEN), .DELAY_CHK (DELAY_CHK))
DUT1
(       .clk(clk),
        .rst(rst),
        .gen_down_valid(up_data),
        .gen_down_data(up_valid),
        .gen_down_ready(up_tlast),
        .chk_up_valid(up_tuser),
        .chk_up_data(up_ready),
        .chk_up_ready(down_data));

  //--------------------------------------------------------------------------

  // Driving clock

  initial
  begin
    forever #5 clk = ~ clk;
  end

  //--------------------------------------------------------------------------
  // Driving reset and control signals

  initial
  begin
    `ifdef __ICARUS__
      $dumpvars;
    `else
      $dumpfile("dump.vcd");
      $dumpvars;
    `endif

    //------------------------------------------------------------------------
    // Initialization
    clk = 1;
    rst = 0;

    //------------------------------------------------------------------------
    // Reset

    repeat (3) @ (negedge clk);
    rst = '1;
    repeat (3) @ (negedge clk);
    rst = '0;
    @(negedge clk);

    //------------------------------------------------------------------------

    @(posedge clk);
    $display ("*** run simulation");
    repeat (80) @ (negedge clk);

    //------------------------------------------------------------------------
    $finish;
  end

  //--------------------------------------------------------------------------

endmodule
