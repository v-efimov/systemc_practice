module check # (
    parameter DW    = 16,
    parameter DELAY = 1
)
(
    input wire         clk,
    input wire         rst,
    input wire         valid_i,
    input wire[DW-1:0] data_i,
    output wire        ready_o
);

wire w_ch_lg1_up_ready;
wire w_sk_buff1_down_valid;
wire[DW-1:0] w_sk_buff1_down_data;

checker_logic # (.DW(DW), .DELAY(DELAY)) ch_lg1  
(
    .clk(clk), 
    .rst(rst),
    .up_valid  (w_sk_buff1_down_valid), 
    .up_data   (w_sk_buff1_down_data),
    .up_ready  (w_ch_lg1_up_ready)
);
skid_buffer # (.DW(DW)) sk_buff1
(
    .clk(clk), 
    .rst(rst), 
    .up_valid  (valid_i), 
    .up_data   (data_i), 
    .down_ready(w_ch_lg1_up_ready), 
    .up_ready  (ready_o),
    .down_valid(w_sk_buff1_down_valid),
    .down_data (w_sk_buff1_down_data)
);

endmodule
