module check_logic # (
    parameter DW = 32,
    parameter DELAY = 0
)
(
    input wire clk,
    input wire rst,
    input wire up_valid,
    input wire [DW-1:0] up_data,
    output wire up_ready
);

    reg down_ready;
    always @(posedge clk) begin
        if (rst) begin
            down_ready <= 0;
        end else begin
            down_ready <= 1;
        end
    end

    reg[DW-1:0]  fast_cnt;
    wire[DW-1:0] fast_cnt_d;
    wire fast_incr;
    wire fast_rst;

    assign fast_incr = up_valid && down_ready;
    assign fast_rst  = up_valid && up_ready || rst;
    assign fast_cnt_d = fast_incr ? (fast_rst ? 0 : (fast_cnt + 1)) :
                                    (fast_rst ? 0 : fast_cnt);

    always @(posedge clk) begin
        begin
            fast_cnt <= fast_cnt_d;
        end
    end

    assign up_ready = down_ready && (fast_cnt == DELAY);

endmodule
