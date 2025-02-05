module check # (
    parameter DW    = 32,
    parameter DELAY = 0
)
(
    input wire         clk,
    input wire         rst,
    input wire         up_valid,
    input wire[DW-1:0] up_data,
    output wire        up_ready
);

wire w_ch_lg1_up_ready;
wire w_sk_buff1_down_valid;
wire[DW-1:0] w_sk_buff1_down_data;

skid_buffer # (.DW(DW)) sk_buff1
(
    .clk(clk),
    .rst(rst),
    .up_valid  (up_valid),
    .up_data   (up_data),
    .down_ready(w_ch_lg1_up_ready),
    .up_ready  (up_ready),
    .down_valid(w_sk_buff1_down_valid),
    .down_data (w_sk_buff1_down_data)
);

check_logic # (.DW(DW), .DELAY(DELAY)) ch_lg1
(
    .clk(clk),
    .rst(rst),
    .up_valid  (w_sk_buff1_down_valid),
    .up_data   (w_sk_buff1_down_data),
    .up_ready  (w_ch_lg1_up_ready)
);

endmodule
