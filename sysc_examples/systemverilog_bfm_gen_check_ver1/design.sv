//#############################################
//########### BEGIN TOP MODULE ################
//#############################################
module top # (
    parameter DATA_WIDTH  = 32,
    parameter DELAY_GEN   = 0,
    parameter DELAY_CHK   = 0
)
(
    input wire                    clk,
    input wire                    rst,
    output wire                   gen_down_valid,
    output wire [DATA_WIDTH-1:0]  gen_down_data,
    output reg                    gen_down_ready,
    output reg                    chk_up_valid,
    output reg  [DATA_WIDTH-1:0]  chk_up_data,
    output wire                   chk_up_ready
);

//#############################################
//############### GENERATOR ###################
//########### SYNTHESIZABLE RTL ###############
//#############################################
generator # (.DW(DATA_WIDTH), .DELAY(DELAY_GEN)) gen1
(
    .clk(clk),
    .rst(rst),
    .down_ready(gen_down_ready),
    .down_valid(gen_down_valid),
    .down_data(gen_down_data)
);

//#############################################
//############ BFM_AXI_TO_TLM #################
//######## SIMILAR-TO-SKID-BUFFER #############
//########### NOT SYNTHESIZABLE ###############
//#############################################

//Samples upstream data on the clock and wirte
//it to downstream mailbox.
//If there is no space in the mailbox then
//store sampled data into expansion register

parameter BYPASS = 1'b0;
parameter SKID = 1'b1;

reg state;
reg [DATA_WIDTH-1:0] data_reg;

always @(posedge clk) begin
    if (rst) begin
        state <= BYPASS;
        gen_down_ready <= 1;
    end 
    else begin
        if (state == BYPASS) begin
           if (gen_down_valid && gen_down_ready) begin
             if (in_mbx.try_put(gen_down_data) == 0) begin
                    data_reg <= gen_down_data;
                    state <= SKID;
                    gen_down_ready <= 0;
             end
           end
        end
        else if (state == SKID) begin
          if (in_mbx.try_put(data_reg)) begin
             state <= BYPASS;
             gen_down_ready <= 1;
          end
        end
        else begin
        end
    end
end

//#############################################
//############## TLM_MODEL ####################
//### Asyncronous (no clock, no valid/ready) ##
//########### NOT SYNTHESIZABLE ###############
//#############################################

mailbox in_mbx = new(4);
mailbox out_mbx = new(4);
bit [DATA_WIDTH-1:0] readdata;

initial begin
  forever begin
    in_mbx.get (readdata);
    out_mbx.put (readdata);
  end
end

//#############################################
//############ BFM_TLM_TO_AXI #################
//########## SIMILAR-TO-REGSLICE ##############
//########### NOT SYNTHESIZABLE ###############
//#############################################

//On the clock if we have condition
//(~down_valid or down_ready) meaning that
//pipeline drives we can pop data from
//upstream mailbox and put that data into
//the pipeline register

bit [DATA_WIDTH-1:0] readmboxdata;

always @(posedge clk) begin
    if (rst) begin
        chk_up_valid <= 0;
    end
    else begin
        //we are comparing pre-clock value here.
      if (~chk_up_valid || chk_up_ready) begin
        if (out_mbx.try_get(readmboxdata)) begin
            chk_up_valid <= 1;
            chk_up_data <= readmboxdata;
        end
          else begin
              chk_up_valid <= 0;
              //we only modify value of valid and don't modify value of data signal
          end
      end
    end
end

//#############################################
//################# CHECK #####################
//########### SYNTHESIZABLE RTL ###############
//#############################################
check # (.DW(DATA_WIDTH), .DELAY(DELAY_CHK)) chk1
(
    .clk(clk),
    .rst(rst),
    .up_ready(chk_up_ready),
    .up_valid(chk_up_valid),
    .up_data(chk_up_data)
);

endmodule
//#############################################
//########### END  TOP MODULE #################
//#############################################


module generator # (
    parameter DW    = 32,
    parameter DELAY = 0
)
(
    input wire          clk,
    input wire          rst,
    input wire          down_ready,
    output wire         down_valid,
    output wire[DW-1:0] down_data
);

wire         w_rg_sl1_up_ready;
wire         w_gen_lg1_down_valid;
wire[DW-1:0] w_gen_lg1_down_data;

generator_logic # (.DW(DW), .DELAY(DELAY)) gen_lg1
(
    .clk(clk),
    .rst(rst),
    .down_ready(w_rg_sl1_up_ready),
    .down_valid(w_gen_lg1_down_valid),
    .down_data (w_gen_lg1_down_data)
);

reg_slice # (.DW(DW)) rg_sl1
(
    .clk(clk),
    .rst(rst),
    .down_ready(down_ready),
    .up_valid  (w_gen_lg1_down_valid),
    .up_data   (w_gen_lg1_down_data),
    .down_valid(down_valid),
    .down_data (down_data),
    .up_ready(w_rg_sl1_up_ready)
);


endmodule


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

    reg[15:0]  fast_cnt;
    wire[15:0] fast_cnt_d;

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


module check # (
    parameter DW    = 32,
    parameter DELAY = 0
)
(
    input wire         clk,
    input wire         rst,
    input wire         up_valid,
    input wire[DW-1:0] up_data,
    output wire        up_ready
);

wire w_ch_lg1_up_ready;
wire w_sk_buff1_down_valid;
wire[DW-1:0] w_sk_buff1_down_data;

skid_buffer # (.DW(DW)) sk_buff1
(
    .clk(clk),
    .rst(rst),
    .up_valid  (up_valid),
    .up_data   (up_data),
    .down_ready(w_ch_lg1_up_ready),
    .up_ready  (up_ready),
    .down_valid(w_sk_buff1_down_valid),
    .down_data (w_sk_buff1_down_data)
);

check_logic # (.DW(DW), .DELAY(DELAY)) ch_lg1
(
    .clk(clk),
    .rst(rst),
    .up_valid  (w_sk_buff1_down_valid),
    .up_data   (w_sk_buff1_down_data),
    .up_ready  (w_ch_lg1_up_ready)
);

endmodule



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

    reg [DW-1:0] fast_cnt;
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
