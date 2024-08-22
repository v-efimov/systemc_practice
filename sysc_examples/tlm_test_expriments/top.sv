module top # (
    parameter DATA_WIDTH  = 16,
    parameter DELAY_GEN   = 2,
    parameter DELAY_CHECK = 1
)
(
    input wire                  clk,
    input wire                  rst
);

wire ready;
wire valid;
wire[DATA_WIDTH-1:0] data;

generator # (.DW(DATA_WIDTH), .DELAY(DELAY_GEN)) gen1
(
    .clk(clk),
    .rst(rst),
    .ready_i(ready),
    .valid_o(valid),
    .data_o(data)
);
check # (.DW(DATA_WIDTH), .DELAY(DELAY_CHECK)) check1
(
    .clk(clk),
    .rst(rst),
    .valid_i(valid),
    .data_i(data),
    .ready_o(ready)
);

endmodule
