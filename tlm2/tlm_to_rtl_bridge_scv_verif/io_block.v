module io_block #(parameter DW = 32, parameter SIGNAL_WIDTH = 4, parameter ID_WIDTH = 4, parameter ADDR_WIDTH = 8, parameter DATA_WIDTH = 16)
(
input                   clk,
input                   rst_n,
input                   up_valid,
input  [(DW-1):0]       up_data,
output                  up_ready,
output                  down_valid,
output [(DW-1):0]       down_data,
input                   down_ready
);

wire en;
wire write_or_read;
wire ram_we;

wire [(SIGNAL_WIDTH - 1):0] signal_bus_in, signal_bus_out_d;
wire [(ID_WIDTH - 1):0] id_bus_in, id_bus_out_d;
wire [(ADDR_WIDTH - 1):0] addr_bus_in, addr_bus_out_d;
wire [(DATA_WIDTH - 1):0] data_bus_in, data_memread, data_bus_out_d;

reg down_valid_q;
reg [(SIGNAL_WIDTH - 1):0] signal_bus_out_q;
reg [(ID_WIDTH - 1):0] id_bus_out_q;
reg [(ADDR_WIDTH - 1):0] addr_bus_out_q;
reg [(DATA_WIDTH - 1):0] data_bus_out_q;

assign signal_bus_in = up_data[(DATA_WIDTH + ADDR_WIDTH + ID_WIDTH)+:SIGNAL_WIDTH];
assign id_bus_in = up_data[(DATA_WIDTH + ADDR_WIDTH)+:ID_WIDTH];
assign addr_bus_in = up_data[(DATA_WIDTH)+:ADDR_WIDTH];
assign data_bus_in = up_data[0+:DATA_WIDTH];

assign signal_bus_out_d = signal_bus_in;    //We just copy signal on the input to the output but the output shall be replaced with combinatorial logic that generates status
assign id_bus_out_d = id_bus_in;
assign addr_bus_out_d = addr_bus_in;
assign data_bus_out_d = (write_or_read) ? data_bus_in : data_memread;

assign en = up_valid & down_ready;
assign write_or_read = signal_bus_in[0];
assign ram_we = en & write_or_read;

//Pseudo Dual port RAM with async read (aka REGISTER FILE)
reg [DATA_WIDTH-1:0] ram[2**ADDR_WIDTH-1:0];

always @ (posedge clk)
    begin
        if (ram_we)
        ram[addr_bus_in] <= data_bus_in;
    end
assign data_memread = ram[addr_bus_in];

always @(posedge clk)
    if (down_ready)
    begin
        signal_bus_out_q <= signal_bus_out_d;
        id_bus_out_q <= id_bus_out_d;
        addr_bus_out_q <= addr_bus_out_d;
        data_bus_out_q <= data_bus_out_d;
    end

always @(posedge clk)
    if (~rst_n)
        down_valid_q <= 1'b0;
    else if (down_ready)
        down_valid_q <= up_valid;

assign down_valid = down_valid_q;
assign down_data = {signal_bus_out_q, id_bus_out_q, addr_bus_out_q, data_bus_out_q};
assign up_ready = down_ready;

endmodule
