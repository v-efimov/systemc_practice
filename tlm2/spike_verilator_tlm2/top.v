module top # (
    parameter DW  = 32
)
(
    input wire                  clk,
    input wire                  rst_n,
    input wire                  up_valid,
    input wire[DW-1:0]  up_data,
    output wire                 up_ready,
    output wire                 down_valid,
    output wire[DW-1:0] down_data,
    input wire                  down_ready
);

wire         w_sk_buff1_down_valid;
wire[DW-1:0] w_sk_buff1_down_data;
wire         w_io_blk1_up_ready;

wire         w_io_blk1_down_valid;
wire[DW-1:0] w_io_blk1_down_data;
wire         w_rg_sl1_up_ready;


skid_buffer # (.DW(DW)) sk_buff1
(
    .clk(clk),
    .rst_n(rst_n),
    .up_valid  (up_valid),
    .up_data   (up_data),
    .up_ready  (up_ready),
    .down_valid(w_sk_buff1_down_valid),
    .down_data (w_sk_buff1_down_data),
    .down_ready(w_io_blk1_up_ready)
);

io_block # (.DW(DW), .SIGNAL_WIDTH(4), .ID_WIDTH(4), .ADDR_WIDTH(8), .DATA_WIDTH(16)) io_blk1
(
    .clk(clk),
    .rst_n(rst_n),
    .up_valid  (w_sk_buff1_down_valid),
    .up_data   (w_sk_buff1_down_data),
    .up_ready  (w_io_blk1_up_ready),
    .down_valid(w_io_blk1_down_valid),
    .down_data (w_io_blk1_down_data),
    .down_ready(w_rg_sl1_up_ready)
);

reg_slice # (.DW(DW)) rg_sl1
(
    .clk(clk),
    .rst_n(rst_n),
    .up_valid  (w_io_blk1_down_valid),
    .up_data   (w_io_blk1_down_data),
    .up_ready(w_rg_sl1_up_ready),
    .down_valid(down_valid),
    .down_data (down_data),
    .down_ready(down_ready)
);

endmodule
