module generator # (
    parameter DW    = 32,
    parameter DELAY = 0
)
(
    input wire          clk,
    input wire          rst,
    input wire          down_ready,
    output wire         down_valid,
    output wire[DW-1:0] down_data
);

wire         w_rg_sl1_up_ready;
wire         w_gen_lg1_down_valid;
wire[DW-1:0] w_gen_lg1_down_data;



reg_slice # (.DW(DW)) rg_sl1
(
    .clk(clk),
    .rst(rst),
    .down_ready(down_ready),
    .up_valid  (w_gen_lg1_down_valid),
    .up_data   (w_gen_lg1_down_data),
    .down_valid(down_valid),
    .down_data (down_data),
    .up_ready(w_rg_sl1_up_ready)
);

generator_logic # (.DW(DW), .DELAY(DELAY)) gen_lg1
(
    .clk(clk),
    .rst(rst),
    .down_ready(w_rg_sl1_up_ready),
    .down_valid(w_gen_lg1_down_valid),
    .down_data (w_gen_lg1_down_data)
);

endmodule
