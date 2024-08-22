module generator_logic # (
    parameter DW    = 16,
    parameter DELAY = 0
)
(
    input wire           clk,
    input wire           rst,
    input wire           down_ready,
    output wire          down_valid,
    output wire[DW-1:0]  down_data
);

wire   handshake;
assign handshake = down_valid && down_ready;



wire   up_valid;
assign up_valid = 1'b1;



wire[DW-1:0] up_data;
reg[DW-1:0]  cnt;

assign       up_data = cnt;

always @(posedge clk) begin
    if (rst) begin
        cnt <= 0;
    end else if (handshake) begin
        cnt <= cnt + 1;
    end
end



reg[$clog2(DELAY):0]  fast_cnt;
wire[$clog2(DELAY):0] fast_cnt_d;
wire   fast_incr;
wire   fast_rst;

assign fast_incr  = down_ready && up_valid;
assign fast_rst   = handshake;

assign fast_cnt_d = fast_rst ? 0 : (fast_incr ? fast_cnt + 1 : fast_cnt);

always @(posedge clk) begin 
    if (rst) begin
        fast_cnt <= 0;
    end else begin
        fast_cnt <= fast_cnt_d;
    end
end



assign down_valid = up_valid && (fast_cnt == DELAY);
assign down_data  = (fast_cnt == DELAY) ? up_data : down_data;

endmodule
