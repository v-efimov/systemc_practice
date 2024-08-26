module generator_logic # (
    parameter DW = 16,
    parameter DELAY = 4
)
(
    input wire clk,
    input wire down_ready,
    input wire rst,
    output down_valid,
    output [DW-1:0] down_data
);

    wire up_valid;
    wire up_ready;

    assign up_valid = 1'b1;
    assign down_valid = up_valid && (fast_cnt == DELAY);
    assign up_ready = down_ready && (fast_cnt == DELAY);

    reg[DW-1:0]  fast_cnt;
    wire[DW-1:0] fast_cnt_d;

    wire fast_incr;
    wire fast_rst;

    always @(posedge clk) begin
        if (rst) begin
            fast_cnt <= 0;
        end else begin
            fast_cnt <= fast_cnt_d;
        end
    end

    assign fast_incr = up_valid && down_ready;
    assign fast_rst  = down_valid && up_ready;
    assign fast_cnt_d = fast_incr ? (fast_rst ? 0 : (fast_cnt + 1)) :
                                    (fast_rst ? 0 : fast_cnt);


    wire data_incr;
    reg[DW-1:0] data_d;

    always @(posedge clk) begin
        if (rst) begin
            data_d <= 'b0;
        end else begin
            data_d <= down_data;
        end
    end

    assign data_incr = down_valid && up_ready;
    assign down_data = data_incr ? (data_d + 1) : data_d;

endmodule
