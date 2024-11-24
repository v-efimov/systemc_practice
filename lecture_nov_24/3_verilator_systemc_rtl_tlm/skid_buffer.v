//This is implementation of a Skid Buffer with mux and register that is described on:
//https://chipmunklogic.com/digital-logic-design/designing-skid-buffers-for-pipelines/

//QUOTE FROM "CHIPMUNKLOGIC.COM":
//Skid Buffer is a specially designed buffer with a mux and register.
//The mux simply forwards (bypasses) input data to output as long as Receiver is ready.
//If Receiver is not ready, and Sender sends valid data, Skid Buffer allows the data to "skid"
//and come to stop by storing it in a buffer. In this way, stalling ("stopping") need not happen immediately,
//but only in the next clock cycle. The mux is switched to forward data in the buffer. When Receiver is ready,
//data in the buffer is sampled by Receiver. The mux is switched again and the input data is forwarded to Receiver
//in subsequent cycles. This allows the registering of ready signal from Receiver as we can now manage that extra
//data from Sender by buffering it inside if necessary.
//END QUOTE.

//This implementation of the skid-buffer has external "up_ready" signal registered (i.e. without combinational logic).
//This is because "up_ready" signal is coming directly from a bit of FSM state register as described in
//"Coding And Scripting Techniques For FSM Designs With Synthesis-Optimized, Glitch-Free Outputs"
//by Clifford E. Cummings article.

module skid_buffer # (
    parameter DW = 16
)
(
    input           clk,
    input           rst,
    input[DW-1:0]   up_data,
    input           up_valid,
    output reg      up_ready,
    output[DW-1:0]  down_data,
    output          down_valid,
    input           down_ready
);

wire we;
wire sel_d, up_ready_d;
reg sel;

//DATAPATH
reg[DW-1:0] data_reg;

always @(posedge clk) begin
    if (we) begin
        data_reg <= up_data;
    end
end


reg valid_reg;

always @(posedge clk) begin
    if (rst) begin
        valid_reg <= 1'b0;
    end else if (we) begin
        valid_reg <= up_valid;
    end
end

assign down_data  = (sel) ? data_reg  : up_data;
assign down_valid = (sel) ? valid_reg : up_valid;

//FSM BASED CONTROL
parameter BYPASS = 1'b0;
parameter SKID = 1'b1;

reg state, next;

always @(posedge clk) begin
    if (rst) begin
        up_ready <= 1'b1;
        sel <= 1'b0;
    end else begin
        up_ready <= up_ready_d;
        sel <= sel_d;
    end
end


always @(posedge clk) begin
    if (rst) begin
        state <= BYPASS;
    end else begin
        state <= next;
    end
end


always @(state or up_valid or down_ready) begin
    next = 1'bx;
    case (state)
    BYPASS:
        if (up_valid & ~down_ready) begin
            next = SKID;
        end else begin
            next = BYPASS;
        end
    SKID:
        if (down_ready) begin
            next = BYPASS;
        end else begin
            next = SKID;
        end
    endcase
end

assign we = ((state == BYPASS) & (up_valid) & (~down_ready));
assign up_ready_d = (next == BYPASS);
assign sel_d = (next == SKID);

endmodule
