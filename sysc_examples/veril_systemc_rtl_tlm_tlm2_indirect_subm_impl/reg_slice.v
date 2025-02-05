module reg_slice # (
    parameter DW = 32
)
(
    input wire         clk,
    input wire         rst,
    input wire         down_ready,
    input wire         up_valid,
    input wire[DW-1:0] up_data,
    output reg         down_valid,
    output reg[DW-1:0] down_data,
    output wire        up_ready
);

assign up_ready = !down_valid || down_ready;

always @(posedge clk) begin
    if (rst) begin
        down_valid <= 0;
    end else if (up_ready) begin
        down_valid <= up_valid;
    end
end

always @(posedge clk) begin
    if (up_ready) begin
        down_data <= up_data;
    end
end

endmodule
