module check_logic # (
    parameter DW = 16,
    parameter DELAY = 4
)
(
    input wire clk,
    input wire rst,
    input wire up_valid,
    input wire [DW-1:0] up_data,
    output wire up_ready
);

    wire down_ready;
    assign down_ready = 1'b1;

    reg[DW-1:0]  fast_cnt;
    wire[DW-1:0] fast_cnt_d;
    wire fast_incr;
    wire fast_rst;

    assign fast_incr = up_valid && down_ready;
    assign fast_rst  = up_valid && up_ready;
    assign fast_cnt_d = fast_incr ? (fast_rst ? 0 : (fast_cnt + 1)) :
                                    (fast_rst ? 0 : fast_cnt);

    always @(posedge clk) begin
        if (rst) begin
            fast_cnt <= 0;
        end else begin
            fast_cnt <= fast_cnt_d;
        end
    end

    assign up_ready = down_ready && (fast_cnt == DELAY);

endmodule
