module generator # (
    parameter DW    = 16,
    parameter DELAY = 0
)
(
    input wire          clk,
    input wire          rst,
    input wire          ready_i,
    output wire         valid_o,
    output wire[DW-1:0] data_o
);

wire         w_rg_sl1_up_ready;
wire         w_gen_lg1_down_valid;
wire[DW-1:0] w_gen_lg1_down_data;



reg_slice # (.DW(DW)) rg_sl1 
(
    .clk(clk), 
    .rst(rst), 
    .down_ready(ready_i), 
    .up_valid  (w_gen_lg1_down_valid), 
    .up_data   (w_gen_lg1_down_data),
    .down_valid(valid_o), 
    .down_data (data_o),
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
