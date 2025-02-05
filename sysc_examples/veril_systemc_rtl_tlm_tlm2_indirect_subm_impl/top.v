module top # (
    parameter DATA_WIDTH  = 32,
    parameter DELAY_GEN   = 0,
    parameter DELAY_CHK   = 0
)
(
    input wire                  clk,
    input wire                  rst,
    output wire                 gen_down_valid,
    output wire[DATA_WIDTH-1:0] gen_down_data,
    input wire                  gen_down_ready,
    input wire                  chk_up_valid,
    input wire[DATA_WIDTH-1:0]  chk_up_data,
    output wire                 chk_up_ready
);

generator # (.DW(DATA_WIDTH), .DELAY(DELAY_GEN)) gen1
(
    .clk(clk),
    .rst(rst),
    .down_ready(gen_down_ready),
    .down_valid(gen_down_valid),
    .down_data(gen_down_data)
);

check # (.DW(DATA_WIDTH), .DELAY(DELAY_CHK)) chk1
(
    .clk(clk),
    .rst(rst),
    .up_ready(chk_up_ready),
    .up_valid(chk_up_valid),
    .up_data(chk_up_data)
);

endmodule
