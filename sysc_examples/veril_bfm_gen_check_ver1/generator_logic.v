module generator_logic # (
    parameter DW = 32,
    parameter DELAY = 0
)
(
    input wire          clk,
    input wire          down_ready,
    input wire          rst,
    output              down_valid,
    output reg [DW-1:0] down_data
);

    reg up_valid;
    always @(posedge clk) begin
        if (rst) begin
            up_valid <= 0;
        end else begin
            up_valid <= 1;
        end
    end

    wire up_ready;

    assign down_valid = up_valid && (fast_cnt == DELAY);
    assign up_ready = down_ready && (fast_cnt == DELAY);

    wire fast_incr;
    wire fast_rst;

    assign fast_incr = up_valid && down_ready;
    assign fast_rst  = down_valid && up_ready || rst;
    assign fast_cnt_d = fast_incr ? (fast_rst ? '0 : (fast_cnt + 1)) :
                                    (fast_rst ? '0 : fast_cnt);
    reg[15:0]  fast_cnt;
    wire[15:0] fast_cnt_d;

    always @(posedge clk) begin
        begin
            fast_cnt <= fast_cnt_d;
        end
    end


    wire data_incr;
    wire [DW-1:0] data_d;

    assign data_incr = down_valid && up_ready;
    assign data_d = data_incr ? (down_data + 1) : down_data;


    always @(posedge clk) begin
        if (rst) begin
            down_data <= '0;
        end else begin
            down_data <= data_d;
        end
    end

endmodule
