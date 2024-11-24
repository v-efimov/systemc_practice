module custom_logic #(parameter DW = 6)
(
input                   clk,
input                   rst,
input  [(DW-1):0]       up_data,
input                   up_valid,
output                  up_ready,
output [(DW-1):0]       down_data,
output                  down_valid,
input                   down_ready
);

reg [7:0] counter;
wire clear_condition;
wire odd;
wire enable;
wire clear;
wire clear_or_rst;
wire [(DW-1):0] even_data;

//BEGIN CUSTOM LOGIC BLOCK

//WR_COUNTER
always @(posedge clk)
if (clear_or_rst) begin
 counter <= '0;
 end else if (enable) begin
 counter <= counter + 1;
 end

assign odd = up_data[0];
assign even_data = up_data * up_data;
assign clear_condition = odd || (counter == 8'b00000001);
assign enable = up_valid && down_ready && ~clear_condition;
assign clear = up_valid && down_ready && clear_condition;
assign clear_or_rst = clear || rst;

assign down_data = odd ? up_data : even_data;
assign down_valid = up_valid;
assign up_ready = down_ready & clear_condition;

//END CUSTOM LOGIC BLOCK

endmodule
